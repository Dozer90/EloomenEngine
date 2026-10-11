#pragma once

#include "streaming/streaming.h"

using namespace sge;


// ==============================================================================
// Input (read) stream

streaming::istream::istream(const data_vector* targetBuffer)
    : buffer(targetBuffer) {
    SGE_ASSERT(targetBuffer != nullptr, "Target buffer cannot be null");
}

streaming::void_r streaming::istream::seek(size_t newPosition) SGE_NOEXCEPT {
    SGE_RETURN_ERRC_IF(newPosition >= buffer->size(), errc::buffer_overflow);
    position = newPosition;
    return {};
}

streaming::data_span_r streaming::istream::read(size_t length) {
    SGE_RETURN_ERRC_IF(position + length > buffer->size(), errc::buffer_overflow);
    const size_t spanStart = position;
    position += length;
    return data_span(buffer->data() + spanStart, length);
}

streaming::data_span_r streaming::istream::read_ln() {
    const size_t bufferSize = buffer->size();
    const size_t spanStart = position;
    bool foundNewline = false;
    bool foundEndOfFile = false;
    do {
        foundEndOfFile = position >= bufferSize;
        foundNewline = !foundEndOfFile && (*buffer)[position] == NEWLINE_CHAR;
    } while (!foundNewline && !foundEndOfFile && ++position < bufferSize);

    const size_t spanSize = position - spanStart;

    if (foundNewline) {
        ++position; // Skip the newline character
    }

    return data_span(buffer->data() + spanStart, spanSize);
}


// ==============================================================================
// Output (write) stream

streaming::ostream::ostream(bool allowExpansion)
    : allowExpansion(allowExpansion) {
    buffer.resize(OSTREAM_DEFAULT_BUFFER_SIZE);
}

streaming::ostream::ostream(size_t initialSize, bool allowExpansion)
    : buffer(initialSize), allowExpansion(allowExpansion) {}

streaming::ostream::ostream(data_vector&& initialBuffer, bool allowExpansion)
    : buffer(eastl::move(initialBuffer)), allowExpansion(allowExpansion) {
    if (buffer.empty()) {
        buffer.resize(OSTREAM_DEFAULT_BUFFER_SIZE);
    }
}

streaming::void_r streaming::ostream::seek(size_t newPosition) {
    if (newPosition == position) {
        return {};
    }

    bool memsetRequired = newPosition < position;
    if (!memsetRequired) {
        if (newPosition > buffer.size()) {
            SGE_RETURN_ERRC_IF_FUNC_FAILED(ensure_data_can_fit(newPosition - position));
            memsetRequired = true;
        }
    }

    position = newPosition;
    if (memsetRequired) {
        memset(buffer.data() + position, 0, buffer.size() - position);
    }
    return {};
}

streaming::void_r streaming::ostream::write(const data_t* data, size_t size) {
    SGE_RETURN_ERRC_IF(data == nullptr || size == 0, errc::null_data);
    SGE_RETURN_ERRC_IF_FUNC_FAILED(ensure_data_can_fit(size));
    memcpy(buffer.data() + position, data, size);
    position += size;
    return {};
}

streaming::void_r streaming::ostream::write_ln() {
    return write(&NEWLINE_CHAR, sizeof(NEWLINE_CHAR));
}

streaming::void_r streaming::ostream::write_ln(const data_t* data, size_t size) {
    SGE_RETURN_ERRC_IF_FUNC_FAILED(write(data, size));
    return write_ln();
}

streaming::void_r streaming::ostream::ensure_data_can_fit(size_t size) {
    if (position + size < buffer.size()) {
        return {};
    }
    SGE_RETURN_ERRC_IF(!allowExpansion, errc::buffer_overflow);
    while (position + size > buffer.size()) {
        buffer.resize(eastl::max(buffer.size() + 1, static_cast<size_t>(buffer.size() * OSTREAM_EXPANSION_FACTOR)));
    }
    return {};
}
streaming::iostream::iostream(size_t size, bool expand) {
    o = eastl::make_unique<ostream>(size, expand);
    i = eastl::make_unique<istream>(o->storage());
}
streaming::iostream::iostream(data_vector&& data, bool expand) {
    o = eastl::make_unique<ostream>(eastl::move(data), expand);
    i = eastl::make_unique<istream>(o->storage());
}
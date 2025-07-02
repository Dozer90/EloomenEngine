#include "utility/serialization.h"

using namespace eloo;


// ---------------------------------------------------------
// String serialization helpers

constexpr size_t serialization::get_string_data_size(size_t length, bool isWide) ELOO_NOEXCEPT {
    const size_t sizeOfCharType = isWide ? SIZE_OF_WCHAR : SIZE_OF_CHAR;
    return length * sizeOfCharType;
}

constexpr size_t serialization::get_string_hash(const data_t* data, size_t byteLength) ELOO_NOEXCEPT {
    constexpr uint64_t INITIAL_HASH = 14695981039346656037ULL; // FNV offset basis
    constexpr uint64_t FNV_PRIME = 1099511628211ULL; // FNV prime
    uint64_t hash = INITIAL_HASH;
    for (size_t i = 0; i < byteLength; ++i) {
        hash ^= data[i];
        hash *= FNV_PRIME;
    }
    return hash;
}


// ---------------------------------------------------------
// String serialization

serialization::data_span_r serialization::serialize_string(const data_t* data, size_t length, bool isWide, data_t* buffer, size_t bufferSize) {
    ELOO_RETURN_ERRC_IF(data == nullptr || length == 0, error_id::null_data);
    ELOO_RETURN_ERRC_IF(buffer == nullptr, error_id::null_buffer);

    const size_t stringDataSize = get_string_data_size(length, isWide);
    const size_t requiredBufferSize = stringDataSize + STRING_BUFFER_HEADER_SIZE;
    ELOO_RETURN_ERRC_IF(requiredBufferSize > bufferSize, error_id::buffer_overflow);

    // Serialized structure: { isWide: bool, strByteSize: size_t, strData: data_t[], expectedHash: size_t }

    const data_span output = data_span(buffer, requiredBufferSize);
    size_t bufferOffset = 0;
    serialize_to_buffer(isWide, buffer, bufferSize, bufferOffset);
    serialize_to_buffer(stringDataSize, buffer, bufferSize, bufferOffset);
    {
        memcpy(buffer + bufferOffset, data, stringDataSize);
        bufferOffset += stringDataSize;
    }
    const size_t hash = get_string_hash(data, stringDataSize);
    serialize_to_buffer(hash, buffer, bufferSize, bufferOffset);
    return output;
}

serialization::data_span_r serialization::serialize_string(const data_t* data, size_t length, bool isWide, data_t* buffer, size_t bufferSize, size_t& bufferOffset) {
    const auto r = serialize_string(data, length, isWide, buffer + bufferOffset, bufferSize - bufferOffset);
    ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
    bufferOffset += r->size();
    return *r;
}


// ---------------------------------------------------------
// String deserialization

serialization::string_variant_r serialization::deserialize_string(const data_t* buffer, size_t bufferSize) {
    ELOO_RETURN_ERRC_IF(buffer == nullptr, error_id::null_data);
    ELOO_RETURN_ERRC_IF(bufferSize < STRING_BUFFER_HEADER_SIZE, error_id::buffer_overflow);

    // Serialized structure: { isWide: bool, strByteSize: size_t, strData: data_t[], expectedHash: size_t }

    size_t bufferOffset = 0;
    const auto isWide = deserialize<bool>(buffer, bufferSize, bufferOffset);
    ELOO_RETURN_ERRC_IF_FUNC_FAILED(isWide);
    const auto strByteSize = deserialize<size_t>(buffer, bufferSize, bufferOffset);
    ELOO_RETURN_ERRC_IF_FUNC_FAILED(strByteSize);
    ELOO_RETURN_ERRC_IF(bufferOffset + *strByteSize > bufferSize, error_id::buffer_overflow);

    string_variant strVariant;
    size_t newHash = 0;
    if (*isWide) {
        eastl::wstring wstr;
        wstr = eastl::wstring(reinterpret_cast<const wchar_t*>(buffer + bufferOffset), *strByteSize / SIZE_OF_WCHAR);
        newHash = get_string_hash(wstr);
        strVariant = eastl::move(wstr);
    } else {
        eastl::string str;
        str = eastl::string(reinterpret_cast<const char*>(buffer + bufferOffset), *strByteSize / SIZE_OF_CHAR);
        newHash = get_string_hash(str);
        strVariant = eastl::move(str);
    }
    bufferOffset += *strByteSize;

    const auto expectedHash = deserialize<size_t>(buffer, bufferSize, bufferOffset);
    ELOO_RETURN_ERRC_IF_FUNC_FAILED(expectedHash);
    ELOO_RETURN_ERRC_IF(newHash != *expectedHash, error_id::string_hash_mismatch);

    return strVariant;
}

serialization::string_variant_r serialization::deserialize_string(const data_t* buffer, size_t bufferSize, size_t& bufferOffset) {
    const auto r = deserialize_string(buffer, bufferSize);
    ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);

    // We need to get the size of the string data before we know how much to advance the buffer offset.
    const auto strByteSize = deserialize<size_t>(buffer + sizeof(bool), sizeof(size_t));
    bufferOffset += STRING_BUFFER_HEADER_SIZE + *strByteSize;
    return *r;
}
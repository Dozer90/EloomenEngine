#pragma once

#include "error_codes/serialization_errc.h"

#include <EASTL/vector.h>
#include <EASTL/span.h>
#include <EASTL/variant.h>
#include <EASTL/string.h>
#include <EASTL/string_view.h>

#include <cstring>
#include <string_view>


namespace eloo::serialization {

    using data_t            = uint8_t;
    using data_vector       = eastl::vector<data_t>;
    using data_span         = eastl::span<const data_t>;
    using string_variant    = eastl::variant<eastl::monostate, eastl::string, eastl::wstring>;

    using data_span_r       = ELOO_DECLARE_RESULT_TYPE(data_span);
    using string_variant_r  = ELOO_DECLARE_RESULT_TYPE(string_variant);

    static constexpr size_t STRING_BUFFER_HEADER_SIZE = sizeof(bool) + sizeof(size_t) * 2U; // isWide + strByteSize + expectedHash
    static constexpr size_t SIZE_OF_CHAR = sizeof(char);
    static constexpr size_t SIZE_OF_WCHAR = sizeof(wchar_t);


    // ==============================================================================
    // Serialization

    //$ A basic serialize function for trivially copyable types.
    //$ arg0:   const T& obj => The object to serialize.
    //$ return: data_span_r => A span containing the serialized data or an error code.
    template <typename T>
    data_span serialize(const T& obj) {
        constexpr size_t SIZE_OF_T = sizeof(T);
        static_assert(eastl::is_trivially_copyable_v<T>, "T must be trivially copyable");
        return data_span(reinterpret_cast<const data_t*>(&obj), SIZE_OF_T);
    }

    //$ A basic serialize function for trivially copyable types.
    //$ arg0:   const T& obj => The object to serialize.
    //$ arg1:   data_t* buffer => Pointer to the buffer where the serialized data will be written.
    //$ arg2:   size_t bufferSize => The size of the buffer.
    //$ return: data_span_r => A span containing the serialized data or an error code.
    template <typename T>
    data_span_r serialize_to_buffer(const T& obj, data_t* buffer, size_t bufferSize) {
        constexpr size_t SIZE_OF_T = sizeof(T);
        ELOO_RETURN_ERRC_IF(buffer == nullptr, errc::null_buffer);
        ELOO_RETURN_ERRC_IF(SIZE_OF_T > bufferSize, errc::buffer_overflow);

        const data_span data = serialize(obj);
        memcpy(buffer, data.data(), data.size());
        return data;
    }

    //$ A basic serialize function for trivially copyable types.
    //$ arg0:   const T& obj => The object to serialize.
    //$ arg1:   data_t* buffer => Pointer to the buffer where the serialized data will be written.
    //$ arg2:   size_t bufferSize => The size of the buffer.
    //$ arg3:   size_t& bufferOffset => The current offset in the buffer where data will be written.
    //$ return: data_span_r => A span containing the serialized data or an error code.
    template <typename T>
    data_span_r serialize_to_buffer(const T& obj, data_t* buffer, size_t bufferSize, size_t& bufferOffset) {
        constexpr size_t SIZE_OF_T = sizeof(T);
        const auto r = serialize_to_buffer(obj, buffer + bufferOffset, bufferSize - bufferOffset);
        ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
        bufferOffset += SIZE_OF_T;
        return *r;
    }


    // ---------------------------------------------------------
    // String serialization helpers

    //$ Gets the size of the string data in bytes based on its length and whether it is wide or narrow.
    //$ arg0:   size_t length => The length of the string data.
    //$ arg1:   bool isWide => Whether the string is wide (wchar_t) or narrow (char).
    //$ return: size_t => The size of the string data in bytes.
    constexpr size_t get_string_data_size(size_t length, bool isWide) ELOO_NOEXCEPT;

    //$ Computes the hash of a string using the FNV-1a hash algorithm.
    //$ arg0:   const data_t* data => Pointer to the string data to hash.
    //$ arg1:   size_t byteLength => The length of the string data in bytes.
    //$ return: size_t => The computed hash of the string data.
    constexpr size_t get_string_hash(const data_t* data, size_t byteLength) ELOO_NOEXCEPT;

    //$ Computes the hash of a string using the FNV-1a hash algorithm.
    //$ arg0:   eastl::string_view strv => The string view to hash.
    //$ return: size_t => The computed hash of the string view.
    constexpr size_t get_string_hash(eastl::string_view strv) ELOO_NOEXCEPT {
        return get_string_hash(reinterpret_cast<const data_t*>(strv.data()), strv.length() * SIZE_OF_CHAR);
    }

    //$ Computes the hash of a wide string using the FNV-1a hash algorithm.
    //$ arg0:   eastl::wstring_view wstrv => The wide string view to hash.
    //$ return: size_t => The computed hash of the wide string view.
    constexpr size_t get_string_hash(eastl::wstring_view wstrv) ELOO_NOEXCEPT {
        return get_string_hash(reinterpret_cast<const data_t*>(wstrv.data()), wstrv.length() * SIZE_OF_WCHAR);
    }


    // ---------------------------------------------------------
    // String serialization

    //$ Serializes and stores a given string in a pre-allocated buffer.
    //$ arg0:   const data_t* data => Pointer to the string data to serialize.
    //$ arg1:   size_t length => The length of the string data.
    //$ arg2:   bool isWide => Whether the string is wide (wchar_t) or narrow (char).
    //$ arg3:   data_t* buffer => Pointer to the buffer where the serialized data will be written.
    //$ arg4:   size_t bufferSize => The size of the buffer.
    //$ return: data_span_r => A span containing the serialized string data or an error code.
    data_span_r serialize_string(const data_t* data, size_t length, bool isWide, data_t* buffer, size_t bufferSize);

    //$ Serializes and stores a given string in a pre-allocated buffer.
    //$ arg0:   const data_t* data => Pointer to the string data to serialize.
    //$ arg1:   size_t length => The length of the string data.
    //$ arg2:   bool isWide => Whether the string is wide (wchar_t) or narrow (char).
    //$ arg3:   data_t* buffer => Pointer to the buffer where the serialized data will be written.
    //$ arg4:   size_t bufferSize => The size of the buffer.
    //$ arg5:   size_t& bufferOffset => The current offset in the buffer where data will be written.
    //$ return: data_span_r => A span containing the serialized string data or an error code.
    data_span_r serialize_string(const data_t* data, size_t length, bool isWide, data_t* buffer, size_t bufferSize, size_t& bufferOffset);


    // ---------------------------------------------------------
    // Helpers for serializing strings

    template <typename StringT>
    concept is_string_t = requires(const StringT& str) {
        { str.data() } -> std::convertible_to<const char*>;
        { str.length() } -> std::convertible_to<size_t>;
    };

    template <typename StringT>
    concept is_wstring_t = requires(const StringT& str) {
        { str.data() } -> std::convertible_to<const wchar_t*>;
        { str.length() } -> std::convertible_to<size_t>;
    };

    template <typename StringT>
    concept is_any_string_t = is_string_t<StringT> || is_wstring_t<StringT>;


    //$ Serializes a string into a pre-allocated buffer.
    //$ arg0:   const T& str => The string to serialize.
    //$ arg1:   data_t* buffer => Pointer to the buffer where the serialized string will be written.
    //$ arg2:   size_t bufferSize => The size of the buffer.
    //$ return: data_span_r => A span containing the serialized string data or an error code.
    template <is_any_string_t T>
    data_span_r serialize_string(const T& str, data_t* buffer, size_t bufferSize) {
        constexpr bool isWide = has_wchar_data<T>;
        return serialize_string(reinterpret_cast<const data_t*>(str.data()), str.length(), isWide, buffer, bufferSize);
    }

    //$ Serializes a string into a pre-allocated buffer.
    //$ arg0:   const T& str => The string to serialize.
    //$ arg1:   data_t* buffer => Pointer to the buffer where the serialized string will be written.
    //$ arg2:   size_t bufferSize => The size of the buffer.
    //$ arg3:   size_t& bufferOffset => The current offset in the buffer where data will be written.
    //$ return: data_span_r => A span containing the serialized string data or an error code.
    template <is_any_string_t T>
    data_span_r serialize_string(const T& str, data_t* buffer, size_t bufferSize, size_t& bufferOffset) {
        constexpr bool isWide = has_wchar_data<T>;
        return serialize_string(reinterpret_cast<const data_t*>(str.data()), str.length(), isWide, buffer, bufferSize, bufferOffset);
    }

    //$ Serializes a string into a pre-allocated buffer.
    //$ arg0:   const T& str => The string to serialize.
    //$ arg1:   data_vector& buffer => The vector where the serialized string will be written.
    //$ arg2:   size_t& bufferOffset => The current offset in the vector where data will be written.
    //$ arg3:   bool allowExpansionToFit [false] => Whether to allow the buffer to expand if it is not large enough.
    //$ return: data_span_r => A span containing the serialized string data or an error code.
    template <is_any_string_t T>
    data_span_r serialize_string(const T& str, data_vector& buffer, size_t& bufferOffset, bool allowExpansionToFit = false) {
        constexpr bool isWide = has_wchar_data<T>;
        return serialize_string(reinterpret_cast<const data_t*>(str.data()), str.length(), isWide, buffer, bufferOffset, allowExpansionToFit);
    }


    // ==============================================================================
    // Deserialization

    //$ A basic deserialize function for trivially copyable types.
    //$ arg0:   const data_t* buffer => Pointer to the buffer containing the serialized data.
    //$ arg1:   size_t bufferSize => The size of the buffer.
    //$ return: std::expected<T, std::error_code> => The deserialized object or an error code.
    template <typename T>
    std::expected<T, std::error_code> deserialize(const data_t* buffer, size_t bufferSize) {
        constexpr size_t SIZE_OF_T = sizeof(T);
        static_assert(eastl::is_trivially_copyable_v<T>, "T must be trivially copyable");

        ELOO_RETURN_ERRC_IF(buffer == nullptr, errc::null_data);
        ELOO_RETURN_ERRC_IF(bufferSize < SIZE_OF_T, errc::buffer_overflow);

        T obj;
        memcpy(&obj, buffer, SIZE_OF_T);
        return obj;
    }


    //$ A basic deserialize function for trivially copyable types.
    //$ arg0:   const data_t* buffer => Pointer to the buffer containing the serialized data.
    //$ arg1:   size_t bufferSize => The size of the buffer.
    //$ arg2:   size_t& bufferOffset => The current offset in the buffer where data will be read from.
    //$ return: std::expected<T, std::error_code> => The deserialized object or an error code.
    template <typename T>
    std::expected<T, std::error_code> deserialize(const data_t* buffer, size_t bufferSize, size_t& bufferOffset) {
        const auto r = deserialize<T>(buffer + bufferOffset, bufferSize - bufferOffset);
        ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
        bufferOffset += r->size();
        return *r;
    }


    //$ A basic deserialize function for trivially copyable types.
    //$ arg0:   data_span span => The byte span containing the serialized data.
    //$ return: std::expected<T, std::error_code> => The deserialized object or an error code.
    template <typename T>
    std::expected<T, std::error_code> deserialize(const data_span& span) {
        return deserialize<T>(span.data(), span.size());
    }


    // ---------------------------------------------------------
    // String deserialization

    //$ Deserializes a string from a buffer.
    //$ arg0:   const data_t* buffer => Pointer to the buffer containing the serialized string data.
    //$ arg1:   size_t bufferSize => The size of the buffer.
    //$ return: string_variant_r => A result containing the deserialized string or an error code.
    string_variant_r deserialize_string(const data_t* buffer, size_t bufferSize);

    //$ Deserializes a string from a buffer.
    //$ arg0:   file::span_t data => The byte span containing the serialized string data.
    //$ return: eastl::string => The deserialized string.
    string_variant_r deserialize_string(const data_t* buffer, size_t bufferSize, size_t& bufferOffset);


    // ---------------------------------------------------------
    // Helpers for deserializing strings

    template <is_string_t T>
    std::expected<T, std::error_code> deserialize(const data_t* buffer, size_t bufferSize) {
        static_assert(eastl::is_same_v<T, eastl::string_view> || eastl::is_same_v<T, std::string_view>,
                      "T cannot be a string_view type, use eastl::string or std::string instead.");

        const auto r = deserialize_string(buffer, bufferSize);
        ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
        ELOO_RETURN_ERRC_IF(!eastl::holds_alternative<eastl::string>(*r), errc::string_type_mismatch);
        eastl::string str = eastl::move(eastl::get<eastl::string>(*r));
        if constexpr (eastl::is_same_v<T, eastl::string>) {
            return str;
        } else {
            return T(str.data(), str.size());
        }
    };

    template <is_wstring_t T>
    std::expected<T, std::error_code> deserialize(const data_t* buffer, size_t bufferSize) {
        static_assert(eastl::is_same_v<T, eastl::wstring_view> || eastl::is_same_v<T, std::wstring_view>,
                      "T cannot be a wstring_view type, use eastl::wstring or std::wstring instead.");

        const auto r = deserialize_string(buffer, bufferSize);
        ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
        ELOO_RETURN_ERRC_IF(!eastl::holds_alternative<eastl::wstring>(*r), errc::string_type_mismatch);
        eastl::wstring wstr = eastl::move(eastl::get<eastl::wstring>(*r));
        if constexpr (eastl::is_same_v<T, eastl::wstring>) {
            return wstr;
        } else {
            return T(wstr.data(), wstr.size());
        }
    };

    template <is_string_t T>
    std::expected<T, std::error_code> deserialize(const data_t* buffer, size_t bufferSize, size_t& bufferOffset) {
        static_assert(eastl::is_same_v<T, eastl::string_view> || eastl::is_same_v<T, std::string_view>,
                      "T cannot be a string_view type, use eastl::string or std::string instead.");

        const auto r = deserialize_string(buffer, bufferSize, bufferOffset);
        ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
        ELOO_RETURN_ERRC_IF(!eastl::holds_alternative<eastl::string>(*r), errc::string_type_mismatch);
        eastl::string str = eastl::move(eastl::get<eastl::string>(*r));
        if constexpr (eastl::is_same_v<T, eastl::string>) {
            return str;
        } else {
            return T(str.data(), str.size());
        }
    };

    template <is_wstring_t T>
    std::expected<T, std::error_code> deserialize(const data_t* buffer, size_t bufferSize, size_t& bufferOffset) {
        static_assert(eastl::is_same_v<T, eastl::wstring_view> || eastl::is_same_v<T, std::wstring_view>,
                      "T cannot be a wstring_view type, use eastl::wstring or std::wstring instead.");

        const auto r = deserialize_string(buffer, bufferSize, bufferOffset);
        ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
        ELOO_RETURN_ERRC_IF(!eastl::holds_alternative<eastl::wstring>(*r), errc::string_type_mismatch);
        eastl::wstring wstr = eastl::move(eastl::get<eastl::wstring>(*r));
        if constexpr (eastl::is_same_v<T, eastl::wstring>) {
            return wstr;
        } else {
            return T(wstr.data(), wstr.size());
        }
    };

    template <is_any_string_t T>
    std::expected<T, std::error_code> deserialize(const data_span& span) {
        return deserialize<T>(reinterpret_cast<const data_t*>(span.data()), span.size());
    }
}
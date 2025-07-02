#pragma once

#include "utility/defines.h"
#include "error_codes/streaming_errc.h"
#include "serialization/serialization.h"

#include <EASTL/vector.h>
#include <EASTL/unique_ptr.h>

namespace eloo::streaming {

    using data_t            = serialization::data_t;
    using data_span         = serialization::data_span;
    using data_vector       = serialization::data_vector;

    using void_r            = ELOO_DECLARE_RESULT_TYPE(void);
    using data_span_r       = serialization::data_span_r;

    constexpr data_t NEWLINE_CHAR = static_cast<streaming::data_t>('\n');
    constexpr size_t OSTREAM_EXPANSION_FACTOR = 1.4f; // Factor by which the buffer size will be increased when it needs to expand
    constexpr size_t OSTREAM_DEFAULT_BUFFER_SIZE = 1024; // Initial size of the stream if it is empty


    // ==============================================================================
    // Input (read) stream

    struct istream {
    protected:
        size_t position = 0;            //$ The current position in the stream
        const data_vector* buffer;      //$ The data being streamed

    public:
        istream(const data_vector* targetBuffer);

        // -------------------------------------------

        //$ Get the current position in the stream.
        //$ return: size_t => The current position in the stream.
        size_t tell() const ELOO_NOEXCEPT {
            return position;
        }

        //$ Set the current position in the stream.
        //$ arg0:   size_t newPosition => The new position to set in the stream.
        //$ return: void_r => Result of the seek operation.
        void_r seek(size_t newPosition) ELOO_NOEXCEPT;

        // -------------------------------------------

        //$ Read a specified number of bytes from the stream.
        //$ arg0:   size_t length => The number of bytes to read from the stream.
        //$ return: data_span_r => A span containing the read data or an error code.
        data_span_r read(size_t length);

        //$ Read an object of type T from the stream.
        //$ return: void_r => Result of the read operation.
        template <typename T>
        std::expected<T, std::error_code> read() {
            constexpr size_t SIZE_OF_T = sizeof(T);
            const auto r = read(SIZE_OF_T);
            ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
            return serialization::deserialize<T>(*r);
        }

        //$ Read a line from the stream until a newline character (or end of file) is encountered.
        //$ return: data_span_r => A span containing the read data (excluding the newline) or an error code.
        data_span_r read_ln();

        //$ op:     Deserialize and read an object from the stream.
        //$ return: void_r => Result of the read operation.
        template <typename T>
        void_r operator >> (T& obj) {
            const auto r = read<T>();
            ELOO_RETURN_ERRC_IF_FUNC_FAILED(r);
            obj = eastl::move(*r);
            return {};
        }

        //$ op:     Deserialize and read an object from the stream.
        //$ return: void_r => Result of the read operation.
        template <typename T>
        static void_r operator << (T& obj, istream& i) {
            return i >> obj;
        }
    };


    // ==============================================================================
    // Output (write) stream

    struct ostream {
        friend struct iostream;

    protected:
        data_vector buffer;             //$ The data being streamed
        bool allowExpansion = true;     //$ Whether to automatically resize the buffer when needed
        size_t position = 0;            //$ The current position in the stream

    public:
        ostream(bool allowExpansion = true);
        ostream(size_t initialSize, bool allowExpansion = true);
        ostream(data_vector&& initialBuffer, bool allowExpansion = true);

        // -------------------------------------------

        //$ Get the current data in the stream.
        //$ return: data_span => A span containing the current data in the stream.
        data_span data() const {
            return data_span(buffer.data(), position);
        }

        // -------------------------------------------

        //$ Get the current position in the stream.
        //$ return: size_t => The current position in the stream.
        size_t tell() const ELOO_NOEXCEPT {
            return position;
        }

        //$ Set the current position in the stream.
        //$ arg0:   size_t newPosition => The new position to set in the stream.
        //$ return: void_r => Result of the seek operation.
        //$ note:   If the new position is greater than the current size, the buffer will be resized if allowExpansion is true.
        //$ note:   If the new position is less than the current size, all data from the new position to the end of the buffer will be cleared.
        void_r seek(size_t newPosition);

        // -------------------------------------------

        //$ Pad the stream with a specified number of bytes.
        //$ arg0:   size_t size => The number of bytes to pad the stream with.
        //$ return: void_r => Result of the pad operation.
        void_r pad(size_t size) {
            return seek(tell() + size);
        }

        //$ Write raw data to the stream.
        //$ arg0:   const data_t* data => Pointer to the data to write.
        //$ arg1:   size_t size => The number of bytes to write.
        //$ return: void_r => Result of the write operation.
        void_r write(const data_t* data, size_t size);

        //$ Write raw data to the stream.
        //$ arg0:   const data_span& data => A span containing the data to write.
        //$ return: void_r => Result of the write operation.
        void_r write(const data_span& data) {
            return write(data.data(), data.size());
        }

        //$ Serialize and write an object to the stream.
        //$ arg0:   const T& obj => The object to write to the stream.
        //$ return: void_r => Result of the write operation.
        template <typename T>
        void_r write(const T& obj) {
            ELOO_RETURN_ERRC_IF_FUNC_FAILED(serialization::serialize_to_buffer(obj, buffer.data(), buffer.size(), position));
            return {};
        }

        //$ Write a newline character to the stream.
        //$ return: void_r => Result of the write operation.
        void_r write_ln();

        //$ Write raw data to the stream ending with a newline character.
        //$ arg0:   const data_t* data => Pointer to the data to write.
        //$ arg1:   size_t size => The number of bytes to write.
        //$ return: void_r => Result of the write operation.
        void_r write_ln(const data_t* data, size_t size);

        //$ op:     Serialize and write an object to the stream.
        //$ return: void_r => Result of the write operation.
        template <typename T>
        void_r operator << (const T& obj) {
            return o.write(obj);
        }

        //$ op:     Serialize and write an object to the stream followed by a newline character.
        //$ return: void_r => Result of the write operation.
        template <typename T>
        static void_r operator >> (const T& obj, ostream& o) {
            return o.write(obj);
        }

    private:
        void_r ensure_data_can_fit(size_t size);
    };


    // ==============================================================================
    // Input/Output (read/write) stream

    struct iostream {
    protected:
        eastl::unique_ptr<istream> i;  //$ The input stream for reading data
        eastl::unique_ptr<ostream> o;  //$ The output stream for writing data

    public:
        iostream(size_t initialSize = 1024, bool allowExpansion = true);
        iostream(data_vector&& initialBuffer, bool allowExpansion = true);


        // -------------------------------------------
        // Input

        //$ Get the current position in the input stream.
        //$ return: size_t => The current position in the input stream.
        size_t tellg() const ELOO_NOEXCEPT {
            return i->tell();
        }

        //$ Set the current position in the input stream.
        //$ arg0:   size_t newPosition => The new position to set in the input stream.
        //$ return: void_r => Result of the seek operation.
        void_r seekg(size_t newPosition) {
            return i->seek(newPosition);
        }

        //$ Synchronize the input stream with the output stream.
        //$ return: void_r => Result of the synchronization operation.
        void_r syncg() {
            return i->seek(o->tell());
        }


        //$ Read a specified number of bytes from the input stream.
        //$ arg0:   size_t length => The number of bytes to read from the input stream.
        //$ return: data_span_r => A result containing the read data or an error code.
        data_span_r read(size_t length) {
            return i->read(length);
        }

        //$ Read an object of type T from the input stream.
        //$ return: std::expected<T, std::error_code> => The deserialized object or an error code.
        template <typename T>
        std::expected<T, std::error_code> read() {
            return i->read<T>();
        }

        //$ Read a line from the input stream until a newline character (or end of file) is encountered.
        //$ return: data_span_r => A result containing the read line or an error code.
        data_span_r read_ln() {
            return i->read_ln();
        }


        //$ op:      Deserialize and read an object from the input stream.
        //$ return: void_r => Result of the read operation.
        template <typename T>
        void_r operator >> (T& obj) {
            return *i >> obj;
        }

        //$ op:     Deserialize and read an object from the input stream.
        //$ return: void_r => Result of the read operation.
        template <typename T>
        static void_r operator << (T& obj, iostream& io) {
            return io >> obj;
        }


        // -------------------------------------------
        // Output

        //$ Get the current position in the output stream.
        //$ return: size_t => The current position in the output stream.
        size_t tellp() const ELOO_NOEXCEPT {
            return o->tell();
        }

        //$ Set the current position in the output stream.
        //$ arg0:   size_t newPosition => The new position to set in the output stream.
        //$ return: void_r => Result of the seek operation.
        void_r seekp(size_t newPosition) {
            return o->seek(newPosition);
        }

        //$ Synchronize the output stream with the input stream.
        //$ return: void_r => Result of the synchronization operation.
        void_r syncp() {
            return o->seek(i->tell());
        }


        //$ Pad the output stream with a specified number of bytes.
        //$ arg0:   size_t size => The number of bytes to pad the output stream with.
        //$ return: void_r => Result of the pad operation.
        void_r pad(size_t size) {
            return o->pad(size);
        }

        //$ Write raw data to the output stream.
        //$ arg0:   const data_t* data => Pointer to the data to write.
        //$ arg1:   size_t size => The number of bytes to write.
        void_r write(const data_t* data, size_t size) {
            return o->write(data, size);
        }

        //$ Serialize and write an object to the output stream.
        //$ arg0:   const T& obj => The object to write to the output stream.
        //$ return: void_r => Result of the write operation.
        template <typename T>
        void_r write(const T& obj) {
            return o->write(obj);
        }

        //$ Write a newline character to the output stream.
        //$ return: void_r => Result of the write operation.
        void_r write_ln() {
            return o->write_ln();
        }


        //$ op:     Serialize and write an object to the output stream.
        //$ return: void_r => Result of the write operation.
        template <typename T>
        void_r operator << (const T& obj) {
            return *o << obj;
        }

        //$ op:     Serialize and write an object to the output stream.
        //$ return: void_r => Result of the write operation.
        template <typename T>
        static void_r operator >> (const T& obj, iostream& io) {
            return io << obj;
        }


        //$ Get the current data in the output stream.
        //$ return: eastl::span<const uint8_t> => A span containing the current data in the output stream.
        eastl::span<const uint8_t> data() const {
            return o->data();
        }
    };
}
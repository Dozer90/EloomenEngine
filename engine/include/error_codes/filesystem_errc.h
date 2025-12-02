#pragma once

/*********************************************************/
/*********************************************************/
/*                                                       */
/*                    DO NOT EDIT!!                      */
/*                                                       */
/*       THIS IS AN AUTOMATICALLY GENERATED FILE         */
/*                                                       */
/*      Instead, modify the data in the file below       */
/*                and rebuild with CMake                 */
/*                                                       */
/*********************************************************/
/*********************************************************/

// DATA SOURCE FILE: cmake/automations/errc_header_gen/data/filesystem.json

/*********************************************************/
/*********************************************************/


#include <system_error>
#include <expected>

namespace eloo::filesystem {
    enum class errc : uint8_t {
        access_denied,
        not_found,
        already_exists,
        invalid_id,
        insufficient_space,
        file_creation_failed,
        path_too_long,
        invalid_path_format,
        invalid_name,
        invalid_file_type,
        path_resolution_failed,
        invalid_root_path,
        invalid_path,
        invalid_source_path,
        invalid_dest_path,
        not_a_file,
        file_already_open,
        file_not_open,
        file_too_large,
        read_only_stream,
        write_only_stream,
        buffer_overflow,
        not_a_directory,
        already_mounted,
        not_mounted,
        already_monitored,
        not_monitored,
        no_data,
        data_size_mismatch,
        data_type_mismatch,
        unsupported_operation,
        filesystem_null,
        _count
    };

    inline std::error_code make_error_code(errc e) {
        struct category_impl:std::error_category {
            const char* name() const noexcept override { return "Filesystem"; }
            std::string message(int ev) const override {
                switch(static_cast<errc>(ev)) {
                    case errc::access_denied: return "Access to the file or directory was denied";
                    case errc::not_found: return "The specified file or directory was not found";
                    case errc::already_exists: return "The file or directory already exists";
                    case errc::invalid_id: return "The specified ID is invalid";
                    case errc::insufficient_space: return "There is not enough space to complete the operation";
                    case errc::file_creation_failed: return "Failed to create the file";
                    case errc::path_too_long: return "The specified path is too long";
                    case errc::invalid_path_format: return "The specified path format is invalid";
                    case errc::invalid_name: return "The specified directory name or file name is invalid";
                    case errc::invalid_file_type: return "The specified file type is invalid";
                    case errc::path_resolution_failed: return "Failed to resolve to an absolute path";
                    case errc::invalid_root_path: return "The specified root path is invalid";
                    case errc::invalid_path: return "The specified path is invalid";
                    case errc::invalid_source_path: return "The source path is invalid (move/copy)";
                    case errc::invalid_dest_path: return "The destination path is invalid (move/copy)";
                    case errc::not_a_file: return "The specified path is not a file";
                    case errc::file_already_open: return "The file is already open and cannot be accessed";
                    case errc::file_not_open: return "The file is not open and cannot be accessed";
                    case errc::file_too_large: return "The file exceeds the maximum allowed size";
                    case errc::read_only_stream: return "The file stream is read-only and cannot be modified";
                    case errc::write_only_stream: return "The file stream is write-only and cannot be read";
                    case errc::buffer_overflow: return "The operation would cause a buffer overflow";
                    case errc::not_a_directory: return "The specified path is not a directory";
                    case errc::already_mounted: return "The directory is already mounted";
                    case errc::not_mounted: return "The directory is not mounted";
                    case errc::already_monitored: return "The directory or file is already being monitored";
                    case errc::not_monitored: return "The directory or file is not being monitored";
                    case errc::no_data: return "No data was found in the file or directory";
                    case errc::data_size_mismatch: return "The deserialized data size does not match the expected type size";
                    case errc::data_type_mismatch: return "The deserialized data type does not match the expected type";
                    case errc::unsupported_operation: return "The operation is not supported";
                    case errc::filesystem_null: return "The filesystem interface is null or not initialized";
                    default: return "Unknown error";
                }
            }
        };
        static category_impl instance;
        return { static_cast<int>(e), instance };
    }
}

namespace std {
    template <> struct is_error_code_enum<eloo::filesystem::errc> : true_type {};
}



#if !defined (ELOO_DECLARE_RESULT_TYPE)
#define ELOO_DECLARE_RESULT_TYPE(_t) std::expected<_t, std::error_code>;
#define ELOO_RETURN_ERRC_IF(_cond, _e) if (_cond) { return std::unexpected(_e); }
#define ELOO_RETURN_ERRC_IF_FAILED(_e) if (!_e) { return std::unexpected(_e); }
#define ELOO_RETURN_ERRC_IF_FUNC_FAILED(_r) ELOO_RETURN_ERRC_IF(!(_r), _r.error())
#endif

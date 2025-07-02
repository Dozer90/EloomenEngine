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

// DATA SOURCE FILE: cmake/automations/errc_header_gen/data/streaming.json

/*********************************************************/
/*********************************************************/


#include <system_error>
#include <expected>

namespace eloo::streaming {
    enum class errc : uint8_t {
        null_data,
        buffer_overflow,
        _count
    };

    inline std::error_code make_error_code(errc e) {
        struct category_impl:std::error_category {
            const char* name() const noexcept override { return "Data Streaming"; }
            std::string message(int ev) const override {
                switch(static_cast<errc>(ev)) {
                    case errc::null_data: return "No data was provided";
                    case errc::buffer_overflow: return "Attempting to access data beyond the buffer limit";
                    default: return "Unknown error";
                }
            }
        };
        static category_impl instance;
        return { static_cast<int>(e), instance };
    }
}

namespace std {
    template <> struct is_error_code_enum<eloo::streaming::errc> : true_type {};
}



#if !defined (ELOO_DECLARE_RESULT_TYPE)
#define ELOO_DECLARE_RESULT_TYPE(_t) std::expected<_t, std::error_code>;
#define ELOO_RETURN_ERRC_IF(_cond, _e) if (_cond) { return std::unexpected(_e); }
#define ELOO_RETURN_ERRC_IF_FAILED(_e) if (!_e) { return std::unexpected(_e); }
#define ELOO_RETURN_ERRC_IF_FUNC_FAILED(_r) ELOO_RETURN_ERRC_IF(!(_r), _r.error())
#endif

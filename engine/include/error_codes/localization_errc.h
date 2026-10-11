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

// DATA SOURCE FILE: cmake/automations/errc_header_gen/data/localization.json

/*********************************************************/
/*********************************************************/


#include <system_error>
#include <expected>

namespace sge::localization {
    enum class errc : uint8_t {
        success = 0,
        temp,
        _count
    };

    inline std::error_code make_error_code(errc e) {
        struct category_impl:std::error_category {
            const char* name() const noexcept override { return "Localization"; }
            std::string message(int ev) const override {
                switch(static_cast<errc>(ev)) {
                    case errc::success: return "Success";
                    case errc::temp: return "No errors yet";
                    default: return "Unknown error";
                }
            }
        };
        static category_impl instance;
        return { static_cast<int>(e), instance };
    }
}

namespace std {
    template <> struct is_error_code_enum<sge::localization::errc> : true_type {};
}



#if !defined (SGE_DECLARE_RESULT_TYPE)
#define SGE_DECLARE_RESULT_TYPE(_t) std::expected<_t, std::error_code>;
#define SGE_RETURN_ERRC_IF(_cond, _e) if (_cond) { return std::unexpected(_e); }
#define SGE_RETURN_ERRC_IF_FAILED(_e) if ((_e) != errc::success) { return std::unexpected(_e); }
#define SGE_RETURN_ERRC_IF_FUNC_FAILED(_r) SGE_RETURN_ERRC_IF(!(_r), _r.error())
#endif

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

// DATA SOURCE FILE: cmake/automations/errc_header_gen/data/rendering.json

/*********************************************************/
/*********************************************************/


#include <system_error>
#include <expected>

namespace eloo::rendering {
    enum class errc : uint8_t {
        invalid_window,
        window_creation_failed,
        device_creation_failed,
        adapter_not_found,
        swap_chain_creation_failed,
        command_queue_creation_failed,
        command_allocator_creation_failed,
        command_list_creation_failed,
        descriptor_heap_creation_failed,
        fence_creation_failed,
        buffer_creation_failed,
        shader_compilation_failed,
        pipeline_state_creation_failed,
        root_signature_creation_failed,
        resize_failed,
        present_failed,
        unsupported_operation,
        _count
    };

    inline std::error_code make_error_code(errc e) {
        struct category_impl:std::error_category {
            const char* name() const noexcept override { return "Rendering"; }
            std::string message(int ev) const override {
                switch(static_cast<errc>(ev)) {
                    case errc::invalid_window: return "Window handle is invalid or null";
                    case errc::window_creation_failed: return "Failed to create window";
                    case errc::device_creation_failed: return "Failed to create graphics device";
                    case errc::adapter_not_found: return "No suitable graphics adapter found";
                    case errc::swap_chain_creation_failed: return "Failed to create swap chain";
                    case errc::command_queue_creation_failed: return "Failed to create command queue";
                    case errc::command_allocator_creation_failed: return "Failed to create command allocator";
                    case errc::command_list_creation_failed: return "Failed to create command list";
                    case errc::descriptor_heap_creation_failed: return "Failed to create descriptor heap";
                    case errc::fence_creation_failed: return "Failed to create fence";
                    case errc::buffer_creation_failed: return "Failed to create vertex or index buffer";
                    case errc::shader_compilation_failed: return "Shader compilation failed";
                    case errc::pipeline_state_creation_failed: return "Failed to create pipeline state";
                    case errc::root_signature_creation_failed: return "Failed to create root signature";
                    case errc::resize_failed: return "Failed to resize swap chain or buffers";
                    case errc::present_failed: return "Failed to present frame to display";
                    case errc::unsupported_operation: return "The requested operation is not supported";
                    default: return "Unknown error";
                }
            }
        };
        static category_impl instance;
        return { static_cast<int>(e), instance };
    }
}

namespace std {
    template <> struct is_error_code_enum<eloo::rendering::errc> : true_type {};
}



#if !defined (ELOO_DECLARE_RESULT_TYPE)
#define ELOO_DECLARE_RESULT_TYPE(_t) std::expected<_t, std::error_code>;
#define ELOO_RETURN_ERRC_IF(_cond, _e) if (_cond) { return std::unexpected(_e); }
#define ELOO_RETURN_ERRC_IF_FAILED(_e) if (!_e) { return std::unexpected(_e); }
#define ELOO_RETURN_ERRC_IF_FUNC_FAILED(_r) ELOO_RETURN_ERRC_IF(!(_r), _r.error())
#endif

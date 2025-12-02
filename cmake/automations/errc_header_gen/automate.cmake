# GENERATE ERROR CODE HEADERS
#
# This automation script generates error code headers from JSON files
# located in the `data` directory of the current automation script and
# places them in the `include/error_codes` directory of the engine.

set(_errc_output_dir "${CMAKE_SOURCE_DIR}/engine/include/error_codes")
file(MAKE_DIRECTORY ${_errc_output_dir})

file(GLOB_RECURSE _errc_json_files
    CONFIGURE_DEPENDS
    "${ELOO_CURRENT_AUTOMATION_DIR}/data/*.json"
)

set(GENERATED_ERRC_HEADERS)
foreach(_json_file IN LISTS _errc_json_files)
    file(READ "${_json_file}" _json)
    string(JSON CATEGORY            GET "${_json}" category)
    string(JSON _error_code_count   LENGTH "${_json}" codes)

    set(ENUM_LIST "")
    set(SWITCH_LIST "")
    math(EXPR _last_index "${_error_code_count} - 1")
    foreach(i RANGE 0 ${_last_index})
        string(JSON _id             GET "${_json}" codes ${i} id)
        string(JSON _msg            GET "${_json}" codes ${i} msg)
        list(APPEND ENUM_LIST "        ${_id},")
        list(APPEND SWITCH_LIST "                    case errc::${_id}: return \"${_msg}\"\;")
    endforeach()
    string(JOIN "\n" ENUM_LIST ${ENUM_LIST})
    string(JOIN "\n" SWITCH_LIST ${SWITCH_LIST})

    get_filename_component(NAMESPACE ${_json_file} NAME)
    string(REPLACE ".json" "" NAMESPACE ${NAMESPACE})
    set(_new_errc_h "${_errc_output_dir}/${NAMESPACE}_errc.h")

    list(APPEND GENERATED_ERRC_HEADERS ${_new_errc_h})

    file(RELATIVE_PATH ELOO_DATA_SOURCE_FILE "${CMAKE_SOURCE_DIR}" "${_json_file}")

    configure_file(
        "${ELOO_CURRENT_AUTOMATION_DIR}/errc.h.in"
        "${_new_errc_h}"
        @ONLY
    )
endforeach()

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${_errc_json_files})
# DIRECTX 12


###########################################
# Package Dependencies

target_link_libraries(EloomEngine PRIVATE Microsoft::DirectX-Headers)


###########################################
# Custom Commands

add_custom_command(
    TARGET EloomEngine
    COMMENT "Copy OpenAL DLL"
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:EloomEngine>
    VERBATIM)
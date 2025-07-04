file(GLOB_RECURSE TOOLS_SRC CONFIGURE_DEPENDS
    *.cpp
    *.h
)

add_executable(Tools ${TOOLS_SRC})

target_include_directories(Tools PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Framework
    ${CMAKE_SOURCE_DIR}/Script
)

target_link_libraries(Tools PRIVATE Framework Script)

set_target_properties(Tools PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FMOD_DLL ${CMAKE_SOURCE_DIR}/_Data/_Libraries/FMOD/lib/x64/fmod.dll)
    else()
        set(FMOD_DLL ${CMAKE_SOURCE_DIR}/_Data/_Libraries/FMOD/lib/x86/fmod.dll)
    endif()
    add_custom_command(TARGET Tools POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FMOD_DLL} $<TARGET_FILE_DIR:Tools>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/_Data
                $<TARGET_FILE_DIR:Tools>/../_Data
    )
endif()
file(GLOB_RECURSE CLIENT_SRC CONFIGURE_DEPENDS
    *.cpp
    *.h
)

add_executable(Client ${CLIENT_SRC})

target_include_directories(Client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Framework
    ${CMAKE_SOURCE_DIR}/Script
)

target_link_libraries(Client PRIVATE Framework Script)

set_target_properties(Client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(MSVC)
    set_target_properties(Client PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY
            "${CMAKE_SOURCE_DIR}/_Build/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>"
    )
endif()


if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FMOD_DLL ${CMAKE_SOURCE_DIR}/_Data/_Libraries/FMOD/lib/x64/fmod.dll)
    else()
        set(FMOD_DLL ${CMAKE_SOURCE_DIR}/_Data/_Libraries/FMOD/lib/x86/fmod.dll)
    endif()
    add_custom_command(TARGET Client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FMOD_DLL} $<TARGET_FILE_DIR:Client>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/_Data
                $<TARGET_FILE_DIR:Client>/../_Data
    )
endif()
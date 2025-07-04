file(GLOB_RECURSE FRAMEWORK_SRC CONFIGURE_DEPENDS
    *.cpp
    *.h
)

add_library(Framework STATIC ${FRAMEWORK_SRC})

target_include_directories(Framework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/_Data/_Libraries/DirectX/Include
)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(DIRECTX_LIB_DIR ${CMAKE_SOURCE_DIR}/_Data/_Libraries/DirectX/Lib/x64)
    else()
        set(DIRECTX_LIB_DIR ${CMAKE_SOURCE_DIR}/_Data/_Libraries/DirectX/Lib/x86)
    endif()
    target_link_directories(Framework PUBLIC ${DIRECTX_LIB_DIR})
    target_link_libraries(Framework PUBLIC d3d11 dxgi)
endif()
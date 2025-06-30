file(GLOB_RECURSE SCRIPT_SRC CONFIGURE_DEPENDS
    *.cpp
    *.h
)

add_library(Script STATIC ${SCRIPT_SRC})

target_include_directories(Script PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Framework
)

target_link_libraries(Script PUBLIC Framework)
include_directories(${RMONITOR_INCLUDE_DIRS})
link_libraries(${RMONITOR_LIBRARIES})

file(GLOB_RECURSE RMTEST_UNIT_SOURCES unit/*.cpp)

add_executable(rmonitor_test
    ${RMTEST_UNIT_SOURCES}
)

target_include_directories(rmonitor_test PUBLIC
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include
)

target_link_libraries(rmonitor_test PUBLIC
    gtest
    gtest_main
)

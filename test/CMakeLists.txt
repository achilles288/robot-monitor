include_directories(${RMONITOR_INCLUDE_DIRS})

file(GLOB_RECURSE RMTEST_UNIT_SOURCES unit/station/*.cpp)

add_executable(rmonitor_test
    ${RMTEST_UNIT_SOURCES}
)

target_include_directories(rmonitor_test PUBLIC
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include
)

target_link_libraries(rmonitor_test PUBLIC
    gtest
    gtest_main
    rmonitor
)




file(GLOB_RECURSE RMTEST_CLIENT_UNIT_SOURCES unit/client/*.cpp)

add_executable(rmonitor_client_test
    ${RMTEST_CLIENT_UNIT_SOURCES}
)

target_include_directories(rmonitor_client_test PUBLIC
    ${PROJECT_SOURCE_DIR}/external/googletest/googletest/include
)

target_link_libraries(rmonitor_client_test PUBLIC
    gtest
    gtest_main
    rmonitor-client
)




add_executable(rmonitor_systest1
    system1/station/app.rc
    system1/station/wxglade_out.cpp
    system1/station/wxglade_out.h
    system1/station/wxglade_user.cpp
)

target_compile_definitions(rmonitor_systest1 PUBLIC
    ${RMONITOR_DEFINITIONS}
)

target_link_libraries(rmonitor_systest1
    ${RMONITOR_LIBRARIES}
)


add_executable(rmonitor_systest2
    system2/station/app.rc
    system2/station/wxglade_out.cpp
    system2/station/wxglade_out.h
    system2/station/wxglade_user.cpp
)

target_compile_definitions(rmonitor_systest2 PUBLIC
    ${RMONITOR_DEFINITIONS}
)

target_link_libraries(rmonitor_systest2
    ${RMONITOR_LIBRARIES}
)


if(WIN32)
set_target_properties(rmonitor_systest2 PROPERTIES
    LINK_FLAGS /SUBSYSTEM:WINDOWS
)
set_target_properties(rmonitor_systest1 PROPERTIES
    LINK_FLAGS /SUBSYSTEM:WINDOWS
)
endif()

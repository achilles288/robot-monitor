add_executable(systest_imu
    app.rc
    wxglade_out.cpp
    monitor.cpp
    section.cpp
    magnetometer.cpp
    orientation.cpp
)

target_compile_definitions(systest_imu PUBLIC
    ${RM_DEFINITIONS}
)

target_link_libraries(systest_imu PUBLIC
    ${RM_LIBRARIES}
    ${RMG_LIBRARIES}
)

if(MSVC)
set_target_properties(systest_imu PROPERTIES
    LINK_FLAGS /SUBSYSTEM:WINDOWS
)
endif()

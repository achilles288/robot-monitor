add_library(rmonitor SHARED
    attribute.cpp
    builtin_call.cpp
    call.cpp
    client.cpp
    client_com.cpp
    echo.cpp
    encryption.cpp
    serial.cpp
    serial_list.cpp
    timerbase.cpp
    widget.cpp
    robotmonitor.hpp
    rm/attribute.hpp
    rm/call.hpp
    rm/client.hpp
    rm/echo.hpp
    rm/encryption.hpp
    rm/timerbase.hpp
    rm/widget.hpp
)

target_include_directories(rmonitor PUBLIC
    ${serial_INCLUDE_DIRS}
)

target_link_libraries(rmonitor PUBLIC
    ${serial_LIBRARIES}
)

set_target_properties(rmonitor PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)




add_library(rmonitor-wx SHARED
    button.cpp
    checkbox.cpp
    echobox.cpp
    gauge.cpp
    icon.c
    serial_wx.cpp
    slider.cpp
    spinctrl.cpp
    stattext.cpp
    textctrl.cpp
    timer.cpp
    rm/button.hpp
    rm/checkbox.hpp
    rm/echobox.hpp
    rm/gauge.hpp
    rm/icon.h
    rm/slider.hpp
    rm/spinctrl.hpp
    rm/stattext.hpp
    rm/textctrl.hpp
    rm/timer.hpp
)

target_compile_definitions(rmonitor-wx PUBLIC
    ${wxWidgets_DEFINITIONS}
)

target_include_directories(rmonitor-wx PUBLIC
    ${serial_INCLUDE_DIRS}
    ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(rmonitor-wx PUBLIC
    ${wxWidgets_LIBRARIES}
    rmonitor
)

set_target_properties(rmonitor-wx PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

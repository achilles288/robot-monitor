add_library(rmonitor SHARED
    attribute.cpp
    call.cpp
    client.cpp
    client_com.cpp
    echo.cpp
    encryption.cpp
    request.cpp
    serial.cpp
    serial_list.cpp
    sync.cpp
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
    rm/serial/serial.h
    rm/serial/v8stdint.h
    serial/serial.cpp
    serial/impl/unix.cpp
    serial/impl/unix.h
    serial/impl/win.cpp
    serial/impl/win.h
    serial/impl/list_ports/list_ports_linux.cpp
    serial/impl/list_ports/list_ports_osx.cpp
    serial/impl/list_ports/list_ports_win.cpp
)

target_link_libraries(rmonitor PUBLIC
    setupapi
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
    icon.cpp
    radiobox.cpp
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
    rm/icon.hpp
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

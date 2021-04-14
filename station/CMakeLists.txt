add_library(rmonitor SHARED
    attribute.cpp
    client.cpp
    encryption.cpp
    widget.cpp
    robotmonitor.hpp
    rm/attribute.hpp
    rm/call.hpp
    rm/client.hpp
    rm/encryption.hpp
    rm/widget.hpp
)

target_link_libraries(rmonitor PUBLIC
    serial
)

set_target_properties(rmonitor PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)




add_library(rmonitor-wx SHARED
    button.cpp
    checkbox.cpp
    gauge.cpp
    slider.cpp
    spinctrl.cpp
    stattext.cpp
    textctrl.cpp
    rm/button.hpp
    rm/checkbox.hpp
    rm/gauge.hpp
    rm/slider.hpp
    rm/spinctrl.hpp
    rm/stattext.hpp
    rm/textctrl.hpp
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

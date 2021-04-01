add_library(rmonitor SHARED
    client.cpp
    encryption.cpp
    robotmonitor.hpp
    rm/attribute.hpp
    rm/call.hpp
    rm/client.hpp
    rm/encryption.hpp
    rm/widget.hpp
)

set_target_properties(rmonitor PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

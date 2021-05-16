add_library(rmonitor-client SHARED
    src/attribute_set.c
    src/attribute_set_bool.c
    src/attribute_set_char.c
    src/attribute_set_float.c
    src/attribute_set_int.c
    src/attribute_set_string.c
    src/connection.c
    src/data.c
)

set_target_properties(rmonitor-client PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

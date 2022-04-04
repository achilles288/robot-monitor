#
# The version of client firmware for testing purpose
#
add_library(rmonitor_client STATIC
    ../src/call.c
    ../src/connection.c
    ../src/input.c
    ../src/output.c
    ../src/request.c
    ../src/string.c
    ../src/sync.c
    virtual_connection.c
)

target_include_directories(rmonitor_client PUBLIC
    ${PROJECT_SOURCE_DIR}/client/src
)


#
# To test the RX and TX of the client firmware manually
#
add_executable(rmonitor_client_test
    main.c
)

target_include_directories(rmonitor_client_test PUBLIC
    ${PROJECT_SOURCE_DIR}/client/src
)

target_link_libraries(rmonitor_client_test PUBLIC
    rmonitor_client
)

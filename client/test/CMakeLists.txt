#
# The version of client firmware for testing purpose
#
add_library(rmonitor_client STATIC
    ../src/rm_call.c
    ../src/rm_connection.c
    ../src/rm_input.c
    ../src/rm_output.c
    ../src/rm_request.c
    ../src/rm_string.c
    ../src/rm_sync.c
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

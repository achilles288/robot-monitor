cmake_minimum_required(VERSION 3.10)

project("Robot Monitor" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)


# Robot Monitor documentation
add_subdirectory(docs)


if(UNIX)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<0:>)
else()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<0:>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)

link_directories(${CMAKE_BINARY_DIR}/lib)

set(CMAKE_INSTALL_FULL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
set(CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)




#
# Preparing external dependencies.
#
if(WIN32)
set(wxWidgets_ROOT_DIR C:/wxWidgets-3.1.4)
set(wxWidgets_CONFIGURATION mswu)
endif()
find_package(wxWidgets 3.1 COMPONENTS base core)




# Compile flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
add_definitions(
    -Wall
    -Wno-unused-function
    -Wno-unused-label
    -Wno-unused-value
    -Wno-unused-variable
)
else()
add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    /wd26451
    /wd4098
    /wd4217
    /wd4244
    /wd4251
    /wd4267
    /wd4273
)
endif()

# Compile Robot Monitor
add_subdirectory(station)


# 
# Compile definitions, include directories and libraries
# 
set(RMONITOR_DEFINITIONS
    ${wxWidgets_DEFINITIONS}
)

set(RMONITOR_INCLUDE_DIRS
    ${wxWidgets_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/station
)

set(RMONITOR_LIBRARIES
    rmonitor
    rmonitor-wx
    ${wxWidgets_LIBRARIES}
)




set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "compile gtest with MD" FORCE)
add_subdirectory(external/googletest)
add_subdirectory(test)

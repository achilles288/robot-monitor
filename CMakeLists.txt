cmake_minimum_required(VERSION 3.10)

project("Robot Monitor" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)




if(UNIX)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<0:>)
else()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<0:>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)

link_directories(${CMAKE_BINARY_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/external/lib)

set(CMAKE_INSTALL_FULL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
set(CMAKE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)




#
# Preparing external dependencies.
#
if(UNIX)
find_package(PkgConfig REQUIRED)
pkg_check_modules(RMG REQUIRED rmg)

else()
set(wxWidgets_ROOT_DIR C:/wxWidgets-3.1.6)
set(wxWidgets_CONFIGURATION mswu)

set(RMG_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/include)
set(RMG_LIBRARIES rmgbase rmgwx)

file(COPY
    external/bin
    DESTINATION ${CMAKE_BINARY_DIR}
)
endif()
find_package(wxWidgets 3.1 COMPONENTS base core gl)




configure_file(
    ${PROJECT_SOURCE_DIR}/station/rm/config.h.in
    ${PROJECT_SOURCE_DIR}/station/rm/config.h
)




# 
# Compile flags
# 
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

# 
# Compile Robot Monitor
# 
add_subdirectory(client/test)
add_subdirectory(station)




# 
# Compile definitions, include directories and libraries
# 
set(RM_DEFINITIONS
    ${wxWidgets_DEFINITIONS}
)

set(RM_INCLUDE_DIRS
    ${serial_INCLUDE_DIRS}
    ${wxWidgets_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/station
)

set(RM_LIBRARIES
    rmonitor
    rmonitor-wx
    ${wxWidgets_LIBRARIES}
)




include_directories(station)

add_subdirectory(test/imu)

/**
 * @file serial.hpp
 * @brief Serial ports to communicate with client devices
 * 
 * Function that lists the available serial ports.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_SERIAL_H__
#define __RM_SERIAL_H__ ///< Header guard

#ifndef RM_API
#ifdef _WIN32
#ifdef RM_EXPORT
#define RM_API __declspec(dllexport) ///< API
#else
#define RM_API __declspec(dllimport) ///< API
#endif
#else
#define RM_API ///< API
#endif
#endif


#include <serial/serial.h>


/**
 * @brief Structure that describes a serial device
 */
struct RM_API rmSerialPortInfo {
    char port[16]; ///< Address to the serial port
    char hardware_id[23]; ///< Hardware ID
    char description[63]; ///< Human readable description of serial device
};


/**
 * @brief A list of available serial ports
 * 
 * Made to look like an STL vector of rmSerialPortInfo but only functions for
 * reading.
 */
class RM_API rmSerialPortList {
  private:
    rmSerialPortInfo* ports = nullptr;
    size_t portCount = 0;
    
  public:
    /**
     * @brief Default constructor
     */
    rmSerialPortList() = default;
    
    /**
     * @brief Destructor
     */
    ~rmSerialPortList();
    
    /**
     * @brief Copy constructor (deleted)
     * 
     * @param list Source
     */
    rmSerialPortList(const rmSerialPortList& list) = delete;
    
    /**
     * @brief Move constructor
     * 
     * @param list Source
     */
    rmSerialPortList(rmSerialPortList&& list) noexcept = default;
    
    /**
     * @brief Copy assignment (deleted)
     * 
     * @param list Source
     */
    rmSerialPortList& operator=(const rmSerialPortList& list) = delete;
    
    /**
     * @brief Return the number of elements
     * 
     * @return This is the number of actual objects held in the vector, which
     *         is not necessarily equal to its storage capacity.
     */
    size_t size() const;
    
    /**
     * @brief Returns a reference to the element at position n in the vector
     *        container
     * 
     * @param i Array index
     * 
     * @return The element at the specified position in the vector
     */
    rmSerialPortInfo const& operator [] (size_t i) const;
    
    /**
     * @brief Adds a new element at the end of the vector, after its current
     *        last element
     * 
     * @param val Value to be copied or moved to the new element
     */
    void push_back(const rmSerialPortInfo& val);
};


/**
 * @brief Class that provides a portable serial port interface.
 */
class RM_API rmSerialPort {
  private:
    serial::Serial mySerial;
    
  public:
    /**
     * @brief Connects to a device via RS-232 serial
     * 
     * @param port The address of the serial port, which would be something
     *             like 'COM1' on Windows and '/dev/ttyACM0' on Linux.
     * @param baud Baudrate
     */
    rmSerialPort(const char* port, uint32_t baud);
    
    /**
     * @brief Checks if the serial port if open
     * 
     * @return True if the serial port is opened and false otherwise
     */
    bool isOpen();
    
    /**
     * @brief Closes the serial port
     */
    void close();
    
    /**
     * @brief Reads a character from the serial port
     * 
     * @return A character from the RX buffer. '\0' if there is nothing to
     *         read.
     */
    char read();
    
    /**
     * @brief Writes a string to the serial port
     * 
     * @param msg A null-terminating string
     */
    void write(const char* msg);
    
    /**
     * @brief Gets a list of devices available on the serial ports
     * 
     * @return An array-like structure containing all the available devices.
     */
    static rmSerialPortList listPorts();
    
    /**
     * @brief Sets a callback function on serial port detected
     * 
     * The callback is triggered whenever the number of elements from
     * listPorts() is changed. This is when a new port is detected or when an
     * existing port is missing.
     * 
     * @param func The callback function
     */
    static void setOnPortDetected(void (*func)());
};



#endif

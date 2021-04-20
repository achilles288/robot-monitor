/**
 * @file serial.cpp
 * @brief Serial ports to communicate with client devices
 * 
 * Function that lists the available serial ports.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/serial.hpp"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <mutex>


/**
 * @brief Default constructor
 */
rmSerialPort::rmSerialPort()
             :mySerial("", 9600, serial::Timeout::simpleTimeout(5000))
{}

/**
 * @brief Connects to a device via RS-232 serial
 * 
 * @param port The address of the serial port, which would be something like
 *             'COM1' on Windows and '/dev/ttyACM0' on Linux.
 * @param baud Baudrate
 */
rmSerialPort::rmSerialPort(const char* port, uint32_t baud)
             :mySerial(port, baud, serial::Timeout::simpleTimeout(5000))
{}

/**
 * @param port The address of the serial port, which would be something like
 *             'COM1' on Windows and '/dev/ttyACM0' on Linux.
 * @param baud Baudrate
 */
void rmSerialPort::connect(const char* port, uint32_t baud) {
    try {
        if(mySerial.isOpen())
            mySerial.close();
        mySerial.setPort(port);
        mySerial.setBaudrate(baud);
        mySerial.open();
    }
    catch(std::exception& e) {}
}

/**
 * @brief Closes the serial port
 */
void rmSerialPort::disconnect() {
    try {
        mySerial.close();
    }
    catch(std::exception& e) {}
}

/**
 * @brief Checks if the serial port if open
 * 
 * @return True if the serial port is opened and false otherwise
 */
bool rmSerialPort::isConnected() { return mySerial.isOpen(); }

/**
 * @brief Reads a character from the serial port
 * 
 * @return A character from the RX buffer. '\0' if there is nothing to
 *         read.
 */
char rmSerialPort::read() {
    uint8_t c = 0;
    try {
        if(mySerial.available())
            mySerial.read(&c, 1);
    }
    catch(std::exception& e) {
        disconnect();
    }
    return (char) c;
}

/**
 * @brief Writes a string to the serial port
 * 
 * @param msg A null-terminating string
 */
void rmSerialPort::write(const char* msg) {
    try {
        mySerial.write(msg);
    }
    catch(std::exception& e) {
        disconnect();
    }
}

/**
 * @brief Gets a list of devices available on the serial ports
 * 
 * @return An array-like structure containing all the available devices.
 */
rmSerialPortList rmSerialPort::listPorts() {
    rmSerialPortList myPortList;
    auto ports = serial::list_ports();
    for(auto it=ports.begin(); it!=ports.end(); it++) {
        rmSerialPortInfo portInfo;
        if(it->hardware_id == "n/a")
            continue;
        strncpy(portInfo.port, it->port.c_str(), 15);
        strncpy(portInfo.hardware_id, it->hardware_id.c_str(), 23);
        strncpy(portInfo.description, it->description.c_str(), 63);
        portInfo.port[15] = '\0';
        portInfo.hardware_id[23] = '\0';
        portInfo.description[63] = '\0';
        myPortList.push_back(portInfo);
    }
    return myPortList;
}




static void (*callback)() = nullptr;
static std::thread thread;
static std::mutex m;

static void endPortDetection() {
    m.lock();
    callback = nullptr;
    m.unlock();
    if(thread.joinable())
        thread.join();
}

static void portDetectionThread() {
    size_t count = 0;
    do {
        m.lock();
        auto ports = serial::list_ports();
        if(ports.size() != count) {
            callback();
            count = ports.size();
        }
        m.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } while(true);
}

/**
 * @brief Sets a callback function on serial port detected
 * 
 * The callback is triggered whenever the number of elements from
 * listPorts() is changed. This is when a new port is detected or when an
 * existing port is missing.
 * 
 * @param func The callback function
 */
void rmSerialPort::setOnPortDetected(void (*func)()) {
    m.lock();
    if(callback == nullptr)
        thread = std::thread(&portDetectionThread);
    callback = func;
    
    static bool atexitAdded = false;
    if(!atexitAdded) {
        atexit(endPortDetection);
        atexitAdded = true;
    }
    m.unlock();
}

/**
 * @brief Sets a callback function on serial port detecteds
 * 
 * @param func The event function
 * @param h Owner instance of the event function
 */
extern void rmSetOnPortDetectedWx(void (wxEvtHandler::*func)(wxEvent&),
                                  wxEvtHandler* h);

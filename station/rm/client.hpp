/**
 * @file client.hpp
 * @brief The client device connected to the station
 * 
 * The client provides a set of attributes and obeys the calls invoked by the
 * station. The main part of the communication takes place in this class.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CLIENT_H__
#define __RM_CLIENT_H__ ///< Header guard

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


#include "attribute.hpp"
#include "call.hpp"
#include "encryption.hpp"
#include "widget.hpp"

#include <cstdint>
#include <cstdio>


#define RM_CONNECTION_USB  1 ///< USB interface
#define RM_CONNECTION_UART 2 ///< RX TX connection
#define RM_CONNECTION_I2C  3 ///< I2C interface
#define RM_CONNECTION_SPI  4 ///< SPI interface
#define RM_CONNECTION_SELF 5 ///< Connection within the same device


/**
 * @brief The client device connected to the station
 * 
 * The client provides a set of attributes and obeys the calls invoked by the
 * station. The main part of the communication takes place in this class.
 */
class RM_API rmClient {
  private:
    char name[32] = "Unknown Device";
    uint8_t key[RM_PUBLIC_KEY_SIZE];
    bool useEncryption = false;
    rmAttribute** attributes = nullptr;
    size_t attrCount = 0;
    rmCall** calls = nullptr;
    size_t callCount = 0;
    rmWidget** widgets = nullptr;
    size_t widgetCount = 0;
    bool connected = false;
    int8_t connectionMethod = 0;
    char port[16] = {0};
    int baudRate = 0;
    FILE* rx_fp = NULL;
    FILE* tx_fp = NULL;
    size_t rx_fp_pos = 0;
    
    bool appendAttribute(rmAttribute* attr);
    
  public:
    /**
     * @brief Default constructor
     */
    rmClient() = default;
    
    /**
     * @brief Destructor
     */
    ~rmClient();
    
    /**
     * @brief Copy constructor (deleted)
     * 
     * @param client Source
     */
    rmClient(const rmClient& client) = delete;
    
    /**
     * @brief Move constructor
     * 
     * @param client Source
     */
    rmClient(rmClient&& client) noexcept = default;
    
    /**
     * @brief Copy assignment (deleted)
     * 
     * @param client Source
     */
    rmClient& operator=(const rmClient& client) = delete;
    
    /**
     * @brief Move assignment
     * 
     * @param client Source
     */
    rmClient& operator=(rmClient&& client) noexcept = default;
    
    /**
     * @brief Gets the name of the client device
     * 
     * @return The device name
     */
    const char* getDeviceName() const;
    
    /**
     * @brief Creates an attribute in the map structure
     * 
     * @param key Unique name of the attribute with maximum 11 characters
     * @param t Data type of the value stored
     * 
     * @return The newly created attribute. Null if the attribute with the same
     *         name already exists or the creation is invalid.
     */
    rmAttribute* createAttribute(const char* key, int8_t t);
    
    /**
     * @brief Creates an attribute in the map structure
     * 
     * @param key Unique name of the attribute with maximum 11 characters
     * @param t Data type of the value stored
     * @param lower Lower bound value. The type of the lower and upper should
     *              be of the same type as t.
     * @param upper Upper bound value
     * 
     * @return The newly created attribute. Null if the attribute with the same
     *         name already exists or the creation is invalid.
     */
    rmAttribute* createAttribute(const char* key, int8_t t, int32_t lower,
                                 int32_t upper);
    
    /**
     * @brief Creates an attribute in the map structure
     * 
     * @param key Unique name of the attribute with maximum 11 characters
     * @param t Data type of the value stored
     * @param lower Lower bound value. The type of the lower and upper should
     *              be of the same type as t.
     * @param upper Upper bound value
     * 
     * @return The newly created attribute. Null if the attribute with the same
     *         name already exists or the creation is invalid.
     */
    rmAttribute* createAttribute(const char* key, int8_t t, float lower,
                                 float upper);
    
    /**
     * @brief Looks for an attribute by name
     * 
     * @param key Unique name
     * 
     * @return Requested attribute. Null if the request is unavailable.
     */
    rmAttribute* getAttribute(const char* key) const;
    
    /**
     * @brief Removes an attribute from the map by name
     * 
     * @param key Unique name
     */
    void removeAttribute(const char* key);
    
    /**
     * @brief Creates a call in the map structure
     * 
     * @param key Unique name of the call with maximum 11 characters
     * @param func The callback function. The callback should have two
     *             parameters, an integer representing the number of extra
     *             tokens and the array of strings.
     * 
     * @return The newly created call. Null if the call with the same name
     *         already exists or the creation is invalid.
     */
    rmCall* createCall(const char* key, void (*func)(int, char**));
    
    /**
     * @brief Looks for a call by name
     * 
     * @param key Unique name
     * 
     * @return Requested call. Null if the request is unavailable.
     */
    rmCall* getCall(const char* key) const;
    
    /**
     * @brief Removes a call from the map by name
     * 
     * @param key Unique name
     */
    void removeCall(const char* key);
    
    /**
     * @brief Appends a widget to the list
     * 
     * @param widget The widget
     */
    void appendWidget(rmWidget* widget);
    
    /**
     * @brief Removes a widget from the list
     * 
     * @param widget The widget
     */
    void removeWidget(rmWidget* widget);
    
    /**
     * @brief Connects to a USB device
     * 
     * @param port Port
     * @param baud Baud rate
     * @param crypt Enable encryption
     */
    void connectUSB(const char* port, int baud, bool crypt=false);
    
    /**
     * @brief Connects to the host device itself
     * 
     * This method is used to communicate with different programs or different
     * systems within the same program. This is intented to be used in
     * automated testing.
     * 
     * @param rx The file which the station listens to
     * @param tx The file where the station writes messages
     * @param crypt Enable encryption
     */
    void connectSelf(const char* rx, const char* tx, bool crypt=false);
    
    /**
     * @brief Disconnects the current connection
     */
    void disconnect();
    
    /**
     * @brief Checks if the client is connected
     * 
     * @return True if the client is connected
     */
    bool isConnected() const;
    
    /**
     * @brief Sends a message to the client device
     * 
     * @param msg Message string
     * @param crypt True to encrypt the message if the connection supports it
     */
    void sendMessage(const char* msg, bool crypt=true) const;
    
    /**
     * @brief Sends the value of attribute to the client
     * 
     * @param attr The attribute
     */
    void sendAttribute(rmAttribute* attr) const;
    
    /**
     * @brief Reads a message from the client device
     * 
     * @return Message string
     */
    const char* readMessage() const;
    
    /**
     * @brief Reads a message that is not decrypted from the client device
     * 
     * @return Message string the is not yet decrypted
     */
    const char* readMessageRaw() const;
};

#endif

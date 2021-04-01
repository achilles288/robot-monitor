/**
 * @file client.cpp
 * @brief The client device connected to the station
 * 
 * The client provides a set of attributes and obeys the calls invoked by the
 * station. The main part of the communication takes place in this class.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/client.hpp"

#include <cstdlib>



/**
 * @brief Destructor
 */
rmClient::~rmClient() {
    disconnect();
    if(attributes != nullptr)
        free(attributes);
    if(calls != nullptr)
        free(calls);
    if(widgets != nullptr)
        free(widgets);
    if(rx_fp != NULL)
        fclose(rx_fp);
    if(tx_fp != NULL)
        fclose(tx_fp);
}

/**
 * @brief Gets the name of the client device
 * 
 * @return The device name
 */
const char* rmClient::getDeviceName() const { return name; }

/**
 * @brief Creates an attribute in the map structure
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 * @param lower Lower bound value. The type of the lower and upper should be
 *              of the same type as t.
 * @param upper Upper bound value
 * 
 * @return The newly created attribute. Null if the attribute with the same
 *         name already exists or the creation is invalid.
 */
rmAttribute* rmClient::createAttribute(const char* key, int8_t t, void* lower,
                                       void* upper)
{
    return nullptr;
}

/**
 * @brief Looks for an attribute by name
 * 
 * @param key Unique name
 * 
 * @return Requested attribute. Null if the request is unavailable.
 */
rmAttribute* rmClient::getAttribute(const char* key) const {
    return nullptr;
}

/**
 * @brief Removes an attribute from the map by name
 * 
 * @param key Unique name
 */
void rmClient::removeAttribute(const char* key) {
    
}

/**
 * @brief Creates a call in the map structure
 * 
 * @param key Unique name of the call with maximum 11 characters
 * @param func The callback function. The callback should have two parameters,
 *        an integer representing the number of extra tokens and the array of
 *        strings.
 * 
 * @return The newly created call. Null if the call with the same name
 *         already exists or the creation is invalid.
 */
rmCall* rmClient::createCall(const char* key, void (*func)(int, char**)) {
    return nullptr;
}

/**
 * @brief Looks for a call by name
 * 
 * @param key Unique name
 * 
 * @return Requested call. Null if the request is unavailable.
 */
rmCall* rmClient::getCall(const char* key) const {
    return nullptr;
}

/**
 * @brief Removes a call from the map by name
 * 
 * @param key Unique name
 */
void rmClient::removeCall(const char* key) {
    
}

/**
 * @brief Appends a widget to the list
 * 
 * @param widget The widget
 */
void rmClient::appendWidget(rmWidget* widget) {
    
}
    
/**
 * @brief Removes a widget from the list
 * 
 * @param widget The widget
 */
void rmClient::removeWidget(rmWidget* widget) {
    
}

/**
 * @brief Connects to a USB device
 * 
 * @param port Port
 * @param baud Baud rate
 * @param crypt Enable encryption
 */
void rmClient::connectUSB(const char* port, int baud, bool crypt) {
    
}

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
void rmClient::connectSelf(const char* rx, const char* tx, bool crypt) {
    
}

/**
 * @brief Disconnects the current connection
 */
void rmClient::disconnect() {
    
}

/**
 * @brief Checks if the client is connected
 * 
 * @return True if the client is connected
 */
bool rmClient::isConnected() const { return connected; }

/**
 * @brief Sends a message to the client device
 * 
 * @param msg Message string
 * @param crypt True to encrypt the message if the connection supports it
 */
void rmClient::sendMessage(const char* msg, bool crypt) const {
    
}

/**
 * @brief Reads a message from the client device
 * 
 * @return Message string
 */
const char* rmClient::readMessage() const {
    return nullptr;
}

/**
 * @brief Reads a message that is not decrypted from the client device
 * 
 * @return Message string the is not yet decrypted
 */
const char* rmClient::readMessageRaw() const {
    return nullptr;
}

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


class rmClient;


#include "rm/client.hpp"

#include <cstring>



/**
 * @brief Destructor
 */
rmClient::~rmClient() {
    disconnect();
    if(attributes != nullptr) {
        for(size_t i=0; i<attrCount; i++)
            delete attributes[i];
        delete attributes;
    }
    if(calls != nullptr) {
        for(size_t i=0; i<callCount; i++)
            delete calls[i];
        delete calls;
    }
    if(widgets != nullptr)
        delete widgets;
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




bool rmClient::appendAttribute(rmAttribute* attr) {
    rmAttribute** newArr = new rmAttribute*[attrCount + 1];
    for(size_t i=0; i<attrCount; i++)
        newArr[i] = attributes[i];
    newArr[attrCount++] = attr;
    
    if(attributes != nullptr)
        delete attributes;
    attributes = newArr;
    return true;
}

/**
 * @brief Creates an attribute in the map structure
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 * 
 * @return The newly created attribute. Null if the attribute with the same
 *         name already exists or the creation is invalid.
 */
rmAttribute* rmClient::createAttribute(const char* key, int8_t t) {
    rmAttribute* attr = new rmAttribute(key, t);
    bool valid = appendAttribute(attr);
    if(valid)
        return attr;
    else
        return nullptr;
}

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
rmAttribute* rmClient::createAttribute(const char* key, int8_t t,
                                       int32_t lower, int32_t upper)
{
    rmAttribute* attr = new rmAttribute(key, t, lower, upper);
    bool valid = appendAttribute(attr);
    if(valid)
        return attr;
    else
        return nullptr;
}

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
rmAttribute* rmClient::createAttribute(const char* key, int8_t t, float lower,
                                       float upper)
{
    rmAttribute* attr = new rmAttribute(key, t, lower, upper);
    bool valid = appendAttribute(attr);
    if(valid)
        return attr;
    else
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
    for(size_t i=0; i<attrCount; i++) {
        if(strcmp(attributes[i]->getName(), key) == 0)
            return attributes[i];
    }
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
    rmWidget** newArr = new rmWidget*[widgetCount + 1];
    for(size_t i=0; i<widgetCount; i++)
        newArr[i] = widgets[i];
    newArr[widgetCount++] = widget;
    
    if(widgets != nullptr)
        delete widgets;
    widgets = newArr;
}
    
/**
 * @brief Removes a widget from the list
 * 
 * @param widget The widget
 */
void rmClient::removeWidget(rmWidget* widget) {
    for(size_t i=0; i<widgetCount; i++) {
        if(widgets[i] == widget) {
            for(size_t j=i+1; j<widgetCount; j++)
                widgets[j - 1] = widgets[j];
            widgetCount--;
            return;
        }
    }
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

#include <iostream>

/**
 * @brief Sends a message to the client device
 * 
 * @param msg Message string
 * @param crypt True to encrypt the message if the connection supports it
 */
void rmClient::sendMessage(const char* msg, bool crypt) const {
    std::cout << msg << std::endl;
}

/**
 * @brief Sends the value of attribute to the client
 * 
 * @param attr The attribute
 */
void rmClient::sendAttribute(rmAttribute* attr) const {
    char msg[160];
    const char* name = attr->getName();
    int8_t type = attr->getType();
    if(type == RM_ATTRIBUTE_BOOL) {
        if(attr->getValue().b)
            snprintf(msg, 159, "set %s true", name);
        else
            snprintf(msg, 159, "set %s false", name);
    }
    else if(type == RM_ATTRIBUTE_CHAR) {
        snprintf(msg, 159, "set %s %c", name, attr->getValue().c);
    }
    else if(type == RM_ATTRIBUTE_INT) {
        snprintf(msg, 159, "set %s %d", name, attr->getValue().i);
    }
    else if(type == RM_ATTRIBUTE_FLOAT) {
        snprintf(msg, 159, "set %s %f", name, attr->getValue().f);
    }
    else if(type == RM_ATTRIBUTE_STRING) {
        snprintf(msg, 159, "set %s \"%s\"", name, attr->getValue().s);
    }
    msg[159] = '\0';
    sendMessage(msg);
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

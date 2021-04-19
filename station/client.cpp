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

#include "rm/builtin_call.hpp"

#include <chrono>
#include <cstring>
#include <iostream>




/**
 * @brief Default constructor
 */
rmClient::rmClient() {
    appendCall(new rmBuiltinCallSet(this));
    appendCall(new rmBuiltinCallEcho(this));
    appendCall(new rmBuiltinCallError(this));
}

/**
 * @brief Destructor
 */
rmClient::~rmClient() {
    mutex1.lock();
    widgetCount = 0;
    mutex1.unlock();
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
}

/**
 * @brief Gets the name of the client device
 * 
 * @return The device name
 */
const char* rmClient::getDeviceName() const { return name; }




int rmClient::binarySearch1(int low, int high, const char* key) const {
    while(true) {
        if(low >= high) {
            if(strcmp(key, attributes[low]->getName()) > 0)
                return low + 1;
            else
                return low;
        }
        int mid = low + (high - low) / 2;
        int cmp = strcmp(key, attributes[mid]->getName());
        if(cmp == 0)
            return mid;
        else if(cmp < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }
}

bool rmClient::appendAttribute(rmAttribute* attr) {
    size_t pos = 0;
    if(attrCount > 0) {
        pos = binarySearch1(0, attrCount - 1, attr->getName());
        if(pos < attrCount) {
            if(strcmp(attr->getName(), attributes[pos]->getName()) == 0)
                return false;
        }
    }
    
    rmAttribute** newArr = new rmAttribute*[attrCount + 1];
    for(size_t i=0; i<pos; i++) {
        newArr[i] = attributes[i];
    }
    for(size_t i=pos; i<attrCount; i++) {
        newArr[i + 1] = attributes[i];
    }
    newArr[pos] = attr;
    attrCount++;
    
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
    mutex1.lock();
    rmAttribute* attr = new rmAttribute(key, t);
    bool valid = appendAttribute(attr);
    mutex1.unlock();
    if(valid)
        return attr;
    else {
        delete attr;
        return nullptr;
    }
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
    mutex1.lock();
    rmAttribute* attr = new rmAttribute(key, t, lower, upper);
    bool valid = appendAttribute(attr);
    mutex1.unlock();
    if(valid)
        return attr;
    else {
        delete attr;
        return nullptr;
    }
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
    mutex1.lock();
    rmAttribute* attr = new rmAttribute(key, t, lower, upper);
    bool valid = appendAttribute(attr);
    mutex1.unlock();
    if(valid)
        return attr;
    else {
        delete attr;
        return nullptr;
    }
}

/**
 * @brief Looks for an attribute by name
 * 
 * @param key Unique name
 * 
 * @return Requested attribute. Null if the request is unavailable.
 */
rmAttribute* rmClient::getAttribute(const char* key) {
    mutex1.lock();
    if(attributes != nullptr) {
        size_t pos = binarySearch1(0, attrCount - 1, key);
        if(pos < attrCount) {
            if(strcmp(attributes[pos]->getName(), key) == 0) {
                mutex1.unlock();
                return attributes[pos];
            }
        }
    }
    mutex1.unlock();
    return nullptr;
}

/**
 * @brief Removes an attribute from the map by name
 * 
 * @param key Unique name
 */
void rmClient::removeAttribute(const char* key) {
    mutex1.lock();
    if(attrCount == 0) {
        mutex1.unlock();
        return;
    }
    size_t pos = binarySearch1(0, attrCount - 1, key);
    if(strcmp(attributes[pos]->getName(), key) != 0) {
        mutex1.unlock();
        return;
    }
    delete attributes[pos];
    
    rmAttribute** newArr = new rmAttribute*[attrCount - 1];
    for(size_t i=0; i<=pos; i++) {
        newArr[i] = attributes[i];
    }
    for(size_t i=pos+1; i<attrCount; i++) {
        newArr[i - 1] = attributes[i];
    }
    attrCount--;
    
    if(attributes != nullptr)
        delete attributes;
    attributes = newArr;
    mutex1.unlock();
}




int rmClient::binarySearch2(int low, int high, const char* key) const {
    while(true) {
        if(low >= high) {
            if(strcmp(key, calls[low]->getName()) > 0)
                return low + 1;
            else
                return low;
        }
        int mid = low + (high - low) / 2;
        int cmp = strcmp(key, calls[mid]->getName());
        if(cmp == 0)
            return mid;
        else if(cmp < 0)
            high = mid - 1;
        else
            low = mid + 1;
    }
}

bool rmClient::appendCall(rmCall* call) {
    size_t pos = 0;
    if(callCount > 0) {
        pos = binarySearch2(0, callCount - 1, call->getName());
        if(pos < callCount) {
            if(strcmp(call->getName(), calls[pos]->getName()) == 0)
                return false;
        }
    }
    
    rmCall** newArr = new rmCall*[callCount + 1];
    for(size_t i=0; i<pos; i++) {
        newArr[i] = calls[i];
    }
    for(size_t i=pos; i<callCount; i++) {
        newArr[i + 1] = calls[i];
    }
    newArr[pos] = call;
    callCount++;
    
    if(calls != nullptr)
        delete calls;
    calls = newArr;
    return true;
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
    mutex1.lock();
    rmCall* call = new rmCall(key, func);
    bool valid = appendCall(call);
    mutex1.unlock();
    if(valid)
        return call;
    else {
        delete call;
        return nullptr;
    }
}

/**
 * @brief Looks for a call by name
 * 
 * @param key Unique name
 * 
 * @return Requested call. Null if the request is unavailable.
 */
rmCall* rmClient::getCall(const char* key) {
    mutex1.lock();
    if(calls != nullptr) {
        size_t pos = binarySearch2(0, callCount - 1, key);
        if(pos < callCount) {
            if(strcmp(calls[pos]->getName(), key) == 0) {
                mutex1.unlock();
                return calls[pos];
            }
        }
    }
    mutex1.unlock();
    return nullptr;
}

/**
 * @brief Removes a call from the map by name
 * 
 * @param key Unique name
 */
void rmClient::removeCall(const char* key) {
    mutex1.lock();
    if(callCount == 0) {
        mutex1.unlock();
        return;
    }
    size_t pos = binarySearch2(0, callCount - 1, key);
    if(strcmp(calls[pos]->getName(), key) != 0) {
        mutex1.unlock();
        return;
    }
    delete calls[pos];
    
    rmCall** newArr = new rmCall*[callCount - 1];
    for(size_t i=0; i<=pos; i++) {
        newArr[i] = calls[i];
    }
    for(size_t i=pos+1; i<callCount; i++) {
        newArr[i - 1] = calls[i];
    }
    callCount--;
    
    if(calls != nullptr)
        delete calls;
    calls = newArr;
    mutex1.unlock();
}

/**
 * @brief Appends a widget to the list
 * 
 * @param widget The widget
 */
void rmClient::appendWidget(rmWidget* widget) {
    mutex1.lock();
    rmWidget** newArr = new rmWidget*[widgetCount + 1];
    for(size_t i=0; i<widgetCount; i++)
        newArr[i] = widgets[i];
    newArr[widgetCount++] = widget;
    
    if(widgets != nullptr)
        delete widgets;
    widgets = newArr;
    mutex1.unlock();
}

/**
 * @brief Removes a widget from the list
 * 
 * @param widget The widget
 */
void rmClient::removeWidget(rmWidget* widget) {
    mutex1.lock();
    for(size_t i=0; i<widgetCount; i++) {
        if(widgets[i] == widget) {
            for(size_t j=i+1; j<widgetCount; j++)
                widgets[j - 1] = widgets[j];
            widgetCount--;
            mutex1.unlock();
            return;
        }
    }
    mutex1.unlock();
}

/**
 * @brief Checks the connection and processes the incoming messages
 */
void rmClient::onIdle() {
    char cmd[256];
    char* tokens[4];
    uint8_t i = 0;
    uint8_t tokenCount = 0;
    bool space = false;
    rmCall* call = nullptr;
    
    char c = read();
    while(c != '\0') {
        if(i == 255)
            c = '\n';
        
        switch(c) {
          case ' ':
            if(!space) {
                cmd[i++] = '\0';
                space = true;
            }
            break;
            
          case '\n':
            cmd[i] = '\0';
            call = getCall(cmd);
            if(call != nullptr)
                call->invoke(tokenCount, tokens);
            i = 0;
            tokenCount = 0;
            space = false;
            break;
            
          default:
            if(space) {
                tokens[tokenCount++] = cmd + i;
                space = false;
            }
            cmd[i++] = c;
        }
        c = read();
    }
}

/**
 * @brief The function of the extra thread for checking the connection status
 */
void rmConnectionThread(rmClient* client) {
    bool running = true;
    
    do {
        client->mutex1.lock();
        if(client->connectionMethod == RM_CONNECTION_SERIAL) {
            if(!client->mySerial->isOpen()) {
                client->mutex1.unlock();
                client->echo("Port disconnected", 1);
                break;
            }
        }
        client->mutex1.unlock();
        
        client->onIdle();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        
        client->mutex1.lock();
        running = !client->thread1End;
        client->mutex1.unlock();
    } while(running);
    
    client->mutex1.lock();
    if(client->myEcho != nullptr) {
        client->myEcho->setEnabled(false);
    }
    for(size_t i=0; i<client->widgetCount; i++) {
        client->widgets[i]->setEnabled(false);
    }
    switch(client->connectionMethod) {
      case RM_CONNECTION_SERIAL:
        client->mySerial->close();
        delete client->mySerial;
        client->mySerial = nullptr;
    }
    client->mutex1.unlock();
}

void rmClient::startConnection() {
    /*char cmd[256];
    char* tokens[4];
    uint8_t i = 0;
    uint8_t tokenCount = 0;
    bool space = false;
    sendMessage("connect", false);
    bool connectionOk = false;
    long t = 0;
    
    do {
        if(connectionMethod == RM_CONNECTION_SERIAL) {
            if(!mySerial->isOpen()) {
                echo("Port disconnected", 1);
                break;
            }
        }
        
        char c = read();
        while(c != '\0') {
            if(i == 255)
                c = '\n';
            
            switch(c) {
              case ' ':
                if(!space) {
                    cmd[i++] = '\0';
                    space = true;
                }
                break;
                
              case '\n':
                cmd[i] = '\0';
                if(strcmp(cmd, "connect")) {
                    connectionOk = true;
                }
                i = 0;
                tokenCount = 0;
                space = false;
                break;
                
              default:
                if(space) {
                    tokens[tokenCount++] = cmd + i;
                    space = false;
                }
                cmd[i++] = c;
            }
            if(connectionOk)
                break;
            c = read();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        t += 20;
        if(t > 5000) {
            echo("Client device not responding", 1);
            break;
        }
    } while(true);
    */
    //if(connectionOk) {
    if(1) {
        connected = true;
        if(myEcho != nullptr) {
            myEcho->setEnabled(false);
        }
        for(size_t i=0; i<widgetCount; i++)
            widgets[i]->setEnabled(true);
        thread1 = new std::thread(&rmConnectionThread, this);
    }
}

/**
 * @brief Connects to a device via RS-232 serial
 * 
 * @param port The address of the serial port, which would be something like
 *             'COM1' on Windows and '/dev/ttyACM0' on Linux.
 * @param baud Baudrate
 * @param crypt Enable encryption
 */
void rmClient::connectSerial(const char* port, uint32_t baud, bool crypt) {
    disconnect();
    mySerial = new rmSerialPort(port, baud);
    if(mySerial->isOpen()) {
        connectionMethod = RM_CONNECTION_SERIAL;
        startConnection();
    }
    else {
        std::cout << "Cannot open port " << port << std::endl;
        delete mySerial;
        mySerial = nullptr;
    }
}

/**
 * @brief Disconnects the current connection
 */
void rmClient::disconnect() {
    if(thread1 != nullptr) {
        mutex1.lock();
        thread1End = true;
        mutex1.unlock();
        thread1->join();
        delete thread1;
        thread1 = nullptr;
    }
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
void rmClient::sendMessage(const char* msg, bool crypt) {
    mutex1.lock();
    switch(connectionMethod) {
      case RM_CONNECTION_SERIAL:
        if(mySerial != nullptr && mySerial->isOpen())
            mySerial->write(msg);
        break;
    }
    mutex1.unlock();
}

char rmClient::read() {
    mutex1.lock();
    char c = '\0';
    switch(connectionMethod) {
      case RM_CONNECTION_SERIAL:
        if(mySerial != nullptr && mySerial->isOpen())
            c = mySerial->read();
        break;
    }
    mutex1.unlock();
    return c;
}

/**
 * @brief Sends the value of attribute to the client
 * 
 * @param attr The attribute
 */
void rmClient::sendAttribute(rmAttribute* attr) {
    char msg[160];
    const char* name = attr->getName();
    switch(attr->getType()) {
      case RM_ATTRIBUTE_BOOL:
        if(attr->getValue().b)
            snprintf(msg, 159, "set %s true\n", name);
        else
            snprintf(msg, 159, "set %s false\n", name);
        break;
        
      case RM_ATTRIBUTE_CHAR:
        snprintf(msg, 159, "set %s %c\n", name, attr->getValue().c);
        break;
        
      case RM_ATTRIBUTE_INT:
        snprintf(msg, 159, "set %s %d\n", name, attr->getValue().i);
        break;
        
      case RM_ATTRIBUTE_FLOAT:
        snprintf(msg, 159, "set %s %f\n", name, attr->getValue().f);
        break;
        
      case RM_ATTRIBUTE_STRING:
        snprintf(msg, 159, "set %s \"%s\"\n", name, attr->getValue().s);
        break;
        
      default:
        msg[0] = '\0';
    }
    msg[159] = '\0';
    sendMessage(msg);
}

/**
 * @brief Sets the printer for echoing messages
 * 
 * @param printer The echo object
 */
void rmClient::setEcho(rmEcho* printer) {
    mutex1.lock();
    myEcho = printer;
    mutex1.unlock();
}

/**
 * @brief Echos the messages
 * 
 * Intended to be called from 'echo' command by the client device. This command
 * is usually to print normal output messages other than attribute changes and
 * functional callbacks.
 * 
 * @param msg The message
 * @param status The status code. Status code other than 0 may print red
 *         messages.
 */
void rmClient::echo(const char* msg, int status) {
    mutex1.lock();
    if(myEcho != nullptr)
        myEcho->echo(msg, status);
    else
        std::cout << msg << std::endl;
    mutex1.unlock();
}

/**
 * @file client_com.cpp
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
#define RM_NO_WX


#include "rm/client.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>


static std::vector<rmClient*> clients;
static std::thread thread;
static std::mutex m;


#define PROCESS_DEFAULT   0b00
#define PROCESS_STARTED   0b01
#define PROCESS_SEPERATOR 0b11


/**
 * @brief Checks the connection and processes the incoming messages
 */
void rmClient::onIdle() {
    rmCall* call;
    char c = read();
    while(c != '\0') {
        if(rx_i == 255)
            c = '\n';
        
        if(rx_flag & PROCESS_STARTED) {
            rmCall* call;
            switch(c) {
              case ' ':
                rx_cmd[rx_i++] = '\0';
                rx_flag = PROCESS_SEPERATOR;
                break;
                
              case '\n':
                rx_cmd[rx_i] = '\0';
                call = getCall(rx_cmd);
                if(call != NULL)
                    call->invoke(rx_tokenCount, rx_tokens);
                rx_flag = PROCESS_DEFAULT;
                break;
                
              default:
                if(rx_flag == PROCESS_SEPERATOR) {
                    if(rx_tokenCount == 8)
                        break;
                    rx_tokens[rx_tokenCount++] = &rx_cmd[rx_i];
                    rx_flag = PROCESS_STARTED;
                }
                rx_cmd[rx_i++] = c;
            }
        }
        else if(c == '$') {
            rx_i = 0;
            rx_tokenCount = 0;
            rx_flag = PROCESS_STARTED;
        }
        c = read();
    }
}

static void connectionThread() {
    do {
        m.lock();
        if(clients.size() == 0) {
            m.unlock();
            break;
        }
        auto vec = clients;
        m.unlock();
        
        for(auto it=vec.begin(); it!=vec.end(); it++) {
            if((*it)->isConnected() == false) {
                (*it)->echo("Port disconnected", 1);
                m.lock();
                auto it2 = std::find(clients.begin(), clients.end(), *it);
                clients.erase(it2);
                m.unlock();
                (*it)->onDisconnected();
            }
            (*it)->onIdle();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    } while(true);
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
            if(!mySerial->isConnected()) {
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
        m.lock();
        if(myEcho != nullptr) {
            myEcho->setEnabled(true);
        }
        for(size_t i=0; i<widgetCount; i++)
            widgets[i]->setEnabled(true);
        m.unlock();
        
        if(timer != nullptr) {
            timer->appendClient(this);
        }
        else {
            m.lock();
            if(clients.size() == 0)
                thread = std::thread(&connectionThread);
            clients.push_back(this);
            m.unlock();
        }
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
    mySerial.connect(port, baud);
    
    if(mySerial.isConnected()) {
        startConnection();
    }
    else {
        char buff[33];
        snprintf(buff, 32, "Cannot open port %s", port);
        buff[32] = '\0';
        echo(buff, 1);
    }
}

/**
 * @brief Connects to a device via RS-232 serial
 * 
 * @param portInfo The serial port information for the device
 * @param baud Baudrate
 * @param crypt Enable encryption
 */
void rmClient::connectSerial(rmSerialPortInfo portInfo, uint32_t baud,
                             bool crypt)
{
    disconnect();
    mySerial.connect(portInfo, baud);
    
    if(mySerial.isConnected()) {
        startConnection();
    }
    else {
        char buff[34];
        snprintf(buff, 33, "Cannot open port %s", portInfo.port);
        buff[33] = '\0';
        echo(buff, 1);
    }
}

/**
 * @brief Disconnects the current connection
 */
void rmClient::disconnect() {
    if(timer != nullptr) {
        timer->removeClient(this);
        onDisconnected();
    }
    else {
        m.lock();
        auto it = std::find(clients.begin(), clients.end(), this);
        if(it != clients.end())
            clients.erase(it);
        if(clients.size() == 0 && thread.joinable()) {
            m.unlock();
            thread.join();
        }
        m.unlock();
    }
}

/**
 * @brief Function triggers on disconnected
 */
void rmClient::onDisconnected() {
    m.lock();
    if(myEcho != nullptr) {
        myEcho->setEnabled(false);
    }
    for(size_t i=0; i<widgetCount; i++) {
        widgets[i]->setEnabled(false);
    }
    mySerial.disconnect();
    m.unlock();
}

/**
 * @brief Checks if the client is connected
 * 
 * @return True if the client is connected
 */
bool rmClient::isConnected() {
    m.lock();
    bool b = mySerial.isConnected();
    m.unlock();
    return b;
}

/**
 * @brief Sends a message to the client device
 * 
 * @param msg Message string
 * @param crypt True to encrypt the message if the connection supports it
 */
void rmClient::sendMessage(const char* msg, bool crypt) {
    m.lock();
    mySerial.write(msg);
    m.unlock();
}

char rmClient::read() {
    m.lock();
    char c = '\0';
    c = mySerial.read();
    m.unlock();
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
        snprintf(msg, 159, "set %s %d\n", name, attr->getValue().b);
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
    m.lock();
    myEcho = printer;
    m.unlock();
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
    m.lock();
    if(myEcho != nullptr)
        myEcho->echo(msg, status);
    else
        std::cout << msg << std::endl;
    m.unlock();
}

/**
 * @brief Sets the timer to handle the onIdle() function
 * 
 * This is used to replace the default extra thread that runs parallel with the
 * main thread.
 * 
 * @param t The timer object
 */
void rmClient::setTimer(rmTimerBase* t) { timer = t; }

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
#define RM_NO_WX


#include "rm/client.hpp"

#include <cstring>


static std::mutex m;

void rmCallbackSet (int argc, char *argv[], rmClient* cli);
void rmCallbackEcho(int argc, char *argv[], rmClient* cli);
void rmCallbackWarn(int argc, char *argv[], rmClient* cli);
void rmCallbackErr (int argc, char *argv[], rmClient* cli);


/**
 * @brief Default constructor
 */
rmClient::rmClient() {
    appendCall(new rmBuiltinCall("set", rmCallbackSet, this));
    appendCall(new rmBuiltinCall("echo", rmCallbackEcho, this));
    appendCall(new rmBuiltinCall("warn", rmCallbackWarn, this));
    appendCall(new rmBuiltinCall("err", rmCallbackErr, this));
}

/**
 * @brief Destructor
 */
rmClient::~rmClient() {
    m.lock();
    widgetCount = 0;
    m.unlock();
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
rmAttribute* rmClient::createAttribute(const char* key, rmAttributeDataType t)
{
    m.lock();
    rmAttribute* attr = new rmAttribute(key, t);
    bool valid = appendAttribute(attr);
    m.unlock();
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
rmAttribute* rmClient::createAttribute(const char* key, rmAttributeDataType t,
                                       float lower, float upper)
{
    m.lock();
    rmAttribute* attr = new rmAttribute(key, t, lower, upper);
    bool valid = appendAttribute(attr);
    m.unlock();
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
    m.lock();
    if(attributes != nullptr) {
        size_t pos = binarySearch1(0, attrCount - 1, key);
        if(pos < attrCount) {
            if(strcmp(attributes[pos]->getName(), key) == 0) {
                m.unlock();
                return attributes[pos];
            }
        }
    }
    m.unlock();
    return nullptr;
}

/**
 * @brief Removes an attribute from the map by name
 * 
 * @param key Unique name
 */
void rmClient::removeAttribute(const char* key) {
    m.lock();
    if(attrCount == 0) {
        m.unlock();
        return;
    }
    size_t pos = binarySearch1(0, attrCount - 1, key);
    if(strcmp(attributes[pos]->getName(), key) != 0) {
        m.unlock();
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
    m.unlock();
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
    m.lock();
    rmCall* call = new rmCall(key, func);
    bool valid = appendCall(call);
    m.unlock();
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
    m.lock();
    if(calls != nullptr) {
        size_t pos = binarySearch2(0, callCount - 1, key);
        if(pos < callCount) {
            if(strcmp(calls[pos]->getName(), key) == 0) {
                m.unlock();
                return calls[pos];
            }
        }
    }
    m.unlock();
    return nullptr;
}

/**
 * @brief Removes a call from the map by name
 * 
 * @param key Unique name
 */
void rmClient::removeCall(const char* key) {
    m.lock();
    if(callCount == 0) {
        m.unlock();
        return;
    }
    size_t pos = binarySearch2(0, callCount - 1, key);
    if(strcmp(calls[pos]->getName(), key) != 0) {
        m.unlock();
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
    m.unlock();
}

/**
 * @brief Appends a widget to the list
 * 
 * @param widget The widget
 */
void rmClient::appendWidget(rmWidget* widget) {
    m.lock();
    rmWidget** newArr = new rmWidget*[widgetCount + 1];
    for(size_t i=0; i<widgetCount; i++)
        newArr[i] = widgets[i];
    newArr[widgetCount++] = widget;
    
    if(widgets != nullptr)
        delete widgets;
    widgets = newArr;
    m.unlock();
}

/**
 * @brief Removes a widget from the list
 * 
 * @param widget The widget
 */
void rmClient::removeWidget(rmWidget* widget) {
    m.lock();
    for(size_t i=0; i<widgetCount; i++) {
        if(widgets[i] == widget) {
            for(size_t j=i+1; j<widgetCount; j++)
                widgets[j - 1] = widgets[j];
            widgetCount--;
            m.unlock();
            return;
        }
    }
    m.unlock();
}


void rmCallbackSet(int argc, char *argv[], rmClient* cli) {
    if(argc != 2)
        return;
    
    rmAttribute* attr = cli->getAttribute(argv[0]);
    if(attr != nullptr) {
        rmAttributeData prev = attr->getValue();
        attr->setValue(argv[1]);
        if(attr->getValue().i != prev.i) {
            rmAttributeNotifier* noti = attr->getNotifier();
            if(noti != nullptr)
                noti->triggerCallback();
        }
    }
}

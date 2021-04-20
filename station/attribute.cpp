/**
 * @file attribute.cpp
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be a string, a number or a blob.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/attribute.hpp"

#include <cmath>
#include <cstdlib>
#include <cstring>


/**
 * @brief Default constructor
 */
rmAttribute::rmAttribute() {
    data.s = nullptr;
    lowerBound.f = NAN;
    upperBound.f = NAN;
}

/**
 * @brief Constructs an attribute with a name, type and boundaries
 * 
 * The constructor declares the new attribute on the specified client.
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 */
rmAttribute::rmAttribute(const char* key, int8_t t)
            :rmAttribute()
{
    strncpy(name, key, 11);
    name[11] = '\0';
    type = t;
}

/**
 * @brief Constructs an attribute with a name, type and boundaries
 * 
 * The constructor declares the new attribute on the specified client.
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 * @param lower Lower bound value. The type of the lower and upper should be of
 *              the same type as t.
 * @param upper Upper bound value
 */
rmAttribute::rmAttribute(const char* key, int8_t t, int32_t lower,
                         int32_t upper)
            :rmAttribute(key, t)
{
    setBoundary(lower, upper);
}

/**
 * @brief Constructs an attribute with a name, type and boundaries
 * 
 * The constructor declares the new attribute on the specified client.
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 * @param lower Lower bound value. The type of the lower and upper should be of
 *              the same type as t.
 * @param upper Upper bound value
 */
rmAttribute::rmAttribute(const char* key, int8_t t, float lower, float upper)
            :rmAttribute(key, t)
{
    setBoundary(lower, upper);
}

/**
 * @brief Destructor
 */
rmAttribute::~rmAttribute() {
    if(type == RM_ATTRIBUTE_STRING && data.s != nullptr)
        delete data.s;
}

/**
 * @brief Gets the attribute name
 * 
 * @return The name of the attribute
 */
const char* rmAttribute::getName() const { return name; }

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the client device.
 * 
 * @param value The boolean value
 */
void rmAttribute::setValue(bool value) {
    if(type == RM_ATTRIBUTE_BOOL ||
       type == RM_ATTRIBUTE_CHAR ||
       type == RM_ATTRIBUTE_INT)
    {
        data.i = (int32_t) value;
    }
    else if(type == RM_ATTRIBUTE_STRING) {
        const char* str = value ? "1" : "0";
        if(data.s != nullptr) {
            if(strcmp(data.s, str) == 0)
                return;
            delete data.s;
        }
        data.s = new char[6];
        strcpy(data.s, str);
    }
}

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the client device.
 * 
 * @param value The character value
 */
void rmAttribute::setValue(char value) {
    if(type == RM_ATTRIBUTE_BOOL ||
       type == RM_ATTRIBUTE_CHAR)
    {
        data.i = (int32_t) value;
    }
    else if(type == RM_ATTRIBUTE_INT) {
        data.i = (int32_t) value;
        if(!std::isnan(lowerBound.f)) {
            if(data.i < lowerBound.i)
                data.i = lowerBound.i;
        }
        if(!std::isnan(upperBound.f)) {
            if(data.i > upperBound.i)
                data.i = upperBound.i;
        }
    }
    else if(type == RM_ATTRIBUTE_STRING) {
        char* str = new char[2];
        str[0] = value;
        str[1] = '\0';
        if(data.s != nullptr) {
            if(strcmp(data.s, str) == 0) {
                delete str;
                return;
            }
            delete data.s;
        }
        data.s = str;
    }
}

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the client device.
 * 
 * @param value The integer value
 */
void rmAttribute::setValue(int32_t value) {
    switch(type) {
      case RM_ATTRIBUTE_BOOL:
        data.b = (bool) value;
        break;
        
      case RM_ATTRIBUTE_CHAR:
        data.c = (char) value;
        break;
        
      case RM_ATTRIBUTE_INT:
        data.i = value;
        if(!std::isnan(lowerBound.f)) {
            if(data.i < lowerBound.i)
                data.i = lowerBound.i;
        }
        if(!std::isnan(upperBound.f)) {
            if(data.i > upperBound.i)
                data.i = upperBound.i;
        }
        break;
        
      case RM_ATTRIBUTE_FLOAT:
        data.f = (float) value;
        if(!std::isnan(lowerBound.f)) {
            if(data.f < lowerBound.f)
                data.f = lowerBound.f;
        }
        if(!std::isnan(upperBound.f)) {
            if(data.f > upperBound.f)
                data.f = upperBound.f;
        }
        break;
        
      case RM_ATTRIBUTE_STRING:
        char *str = new char[12];
        snprintf(str, 11, "%d", value);
        str[11] = '\0';
        if(data.s != nullptr) {
            if(strcmp(data.s, str) == 0) {
                delete str;
                return;
            }
            delete data.s;
        }
        data.s = str;
    }
}

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the client device.
 * 
 * @param value The floating point value
 */
void rmAttribute::setValue(float value) {
    switch(type) {
      case RM_ATTRIBUTE_BOOL:
        data.b = !std::isnan(value);
        break;
        
      case RM_ATTRIBUTE_INT:
        data.i = (int32_t) value;
        if(!std::isnan(lowerBound.f)) {
            if(data.i < lowerBound.i)
                data.i = lowerBound.i;
        }
        if(!std::isnan(upperBound.f)) {
            if(data.i > upperBound.i)
                data.i = upperBound.i;
        }
        break;
        
      case RM_ATTRIBUTE_FLOAT:
        data.f = value;
        if(!std::isnan(lowerBound.f)) {
            if(data.f < lowerBound.f)
                data.f = lowerBound.f;
        }
        if(!std::isnan(upperBound.f)) {
            if(data.f > upperBound.f)
                data.f = upperBound.f;
        }
        break;
        
      case RM_ATTRIBUTE_STRING:
        char *str = new char[16];
        snprintf(str, 15, "%f", value);
        str[15] = '\0';
        if(data.s != nullptr) {
            if(strcmp(data.s, str) == 0) {
                delete str;
                return;
            }
            delete data.s;
        }
        data.s = str;
    }
}

/**
 * @brief Sets the value
 * 
 * Calling this function triggers the set command to the client device.
 * 
 * @param value The string value or blob. The array is copied.
 */
void rmAttribute::setValue(const char* value) {
    switch(type) {
      case RM_ATTRIBUTE_BOOL:
        if(strcmp(value, "0") == 0)
            data.b = false;
        else if(strcmp(value, "1") == 0)
            data.b = true;
        break;
        
      case RM_ATTRIBUTE_CHAR:
        if(value != nullptr)
            data.c = value[0];
        break;
        
      case RM_ATTRIBUTE_INT:
        data.i = atoi(value);
        if(!std::isnan(lowerBound.f)) {
            if(data.i < lowerBound.i)
                data.i = lowerBound.i;
        }
        if(!std::isnan(upperBound.f)) {
            if(data.i > upperBound.i)
                data.i = upperBound.i;
        }
        break;
        
      case RM_ATTRIBUTE_FLOAT:
        data.f = atof(value);
        if(!std::isnan(lowerBound.f)) {
            if(data.f < lowerBound.f)
                data.f = lowerBound.f;
        }
        if(!std::isnan(upperBound.f)) {
            if(data.f > upperBound.f)
                data.f = upperBound.f;
        }
        break;
        
      case RM_ATTRIBUTE_STRING:
        if(data.s != NULL) {
            if(strcmp(value, data.s) == 0)
                return;
            delete data.s;
        }
        size_t len = strnlen(value, 127);
        data.s = new char[len + 1];
        memcpy(data.s, value, len);
        data.s[len] = '\0';
    }
}
    
/**
 * @brief Gets the value
 * 
 * @return The value in a pointer type which is to be casted to the
 *         appropriate type
 */
rmAttributeData rmAttribute::getValue() const { return data; }
    
/**
 * @brief Gets string value
 * 
 * @return Any type of attribute value parsed into a string
 */
std::string rmAttribute::getValueString() const {
    char buff[16];
    switch(type) {
      case RM_ATTRIBUTE_BOOL:
        return data.b ? "1" : "0";
        
      case RM_ATTRIBUTE_CHAR:
        buff[0] = data.c;
        buff[1] = '\0';
        return std::string(buff);
        
      case RM_ATTRIBUTE_INT:
        snprintf(buff, 11, "%d", data.i);
        buff[11] = '\0';
        return std::string(buff);
        
      case RM_ATTRIBUTE_FLOAT:
        snprintf(buff, 15, "%f", data.f);
        buff[15] = '\0';
        return std::string(buff);
        
      case RM_ATTRIBUTE_STRING:
        return std::string(data.s);
        
      default:
        return std::string();
    }
}

/**
 * @brief Gets the data type of the value
 * 
 * @return The data type number
 */
int8_t rmAttribute::getType() const { return type; }

/**
 * @brief Sets the boundary
 * 
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
void rmAttribute::setBoundary(int32_t lower, int32_t upper) {
    switch(type) {
      case RM_ATTRIBUTE_INT:
        lowerBound.i = lower;
        upperBound.i = upper;
        break;
        
      case RM_ATTRIBUTE_FLOAT:
        lowerBound.f = (float) lower;
        upperBound.f = (float) upper;
    }
}

/**
 * @brief Sets the boundary
 * 
 * @param lower Lower bound value
 * @param upper Upper bound value
 */
void rmAttribute::setBoundary(float lower, float upper) {
    switch(type) {
      case RM_ATTRIBUTE_INT:
        lowerBound.i = (int32_t) lower;
        upperBound.i = (int32_t) upper;
        break;
        
      case RM_ATTRIBUTE_FLOAT:
        lowerBound.f = lower;
        upperBound.f = upper;
    }
}

/**
 * @brief Gets the lower bound value
 * 
 * @return Lower bound value which is an integer or a float. If there is no
 *         boundary or the request is invalid, returns a NAN.
 */
rmAttributeData rmAttribute::getLowerBound() const { return lowerBound; }

/**
 * @brief Gets the upper bound value
 * 
 * @return Upper bound value which is an integer or a float. If there is no
 *         boundary or the request is invalid, returns a NAN.
 */
rmAttributeData rmAttribute::getUpperBound() const { return upperBound; }

/**
 * @brief Sets the notifier object for this attribute
 * 
 * @param noti The attribute notifier
 */
void rmAttribute::setNotifier(rmAttributeNotifier* noti) { notifier = noti; }

/**
 * @brief Gets the notifier object of this attribute
 * 
 * @return The associated widget. Returns null if it doesn't have.
 */
rmAttributeNotifier* rmAttribute::getNotifier() const { return notifier; }




/**
 * @brief Triggers the member or custom callback
 * 
 * When the custom callback is set, the member function which is triggered at
 * the same instance is not called.
 */
void rmAttributeNotifier::triggerCallback() {
    if(customCallback != nullptr)
        customCallback();
    else
        onAttributeChange();
}

/**
 * @brief Triggers on attribute value change
 * 
 * Triggers when the value of attribute is changed by the client device's
 * reports.
 */
void rmAttributeNotifier::onAttributeChange() {}

/**
 * @brief Sets the custom function to be called on value change
 * 
 * When the custom callback is set, the member function which is triggered at
 * the same instance is not called.
 * 
 * @param func The function that is not a member of a class
 */
void rmAttributeNotifier::setCustomCallback(void (*func)()) {
    customCallback = func;
}

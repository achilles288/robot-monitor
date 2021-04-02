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

#include <cstdlib>
#include <cstring>


/**
 * @brief Constructs an attribute with a name, type and boundaries
 * 
 * The constructor declares the new attribute on the specified client.
 * 
 * @param key Unique name of the attribute with maximum 11 characters
 * @param t Data type of the value stored
 */
rmAttribute::rmAttribute(const char* key, int8_t t) {
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
 * @param value The string value or blob. The array is copied.
 */
void rmAttribute::setValue(const char* value) {
    if(type == RM_ATTRIBUTE_BOOL) {
        if(strcmp(value, "false") == 0)
            data.b = false;
        else if(strcmp(value, "true") == 0)
            data.b = true;
    }
    else if(type == RM_ATTRIBUTE_CHAR) {
        if(value != NULL)
            data.c = value[0];
    }
    else if(type == RM_ATTRIBUTE_INT) {
        data.i = atoi(value);
    }
    else if(type == RM_ATTRIBUTE_FLOAT) {
        data.f = atof(value);
    }
    else if(type == RM_ATTRIBUTE_STRING) {
        size_t len = strnlen(value, 127);
        if(data.s != nullptr)
            free(data.s);
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
    if(type == RM_ATTRIBUTE_INT) {
        lowerBound.i = lower;
        upperBound.i = upper;
    }
    else if(type == RM_ATTRIBUTE_FLOAT) {
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
    if(type == RM_ATTRIBUTE_INT) {
        lowerBound.i = (int32_t) lower;
        upperBound.i = (int32_t) upper;
    }
    else if(type == RM_ATTRIBUTE_FLOAT) {
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
 * @brief Sets the widget associated with this attribute
 * 
 * @param w The widget
 */
void rmAttribute::setWidget(rmWidget* w) { widget = w; }

/**
 * @brief Gets the widget associated with this attribute
 * 
 * @return The associated widget. Returns null if it doesn't have.
 */
rmWidget* rmAttribute::getWidget() const { return widget; }

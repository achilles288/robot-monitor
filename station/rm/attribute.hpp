/**
 * @file attribute.hpp
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be a string, a number or a blob.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_ATTRIBUTE_H__
#define __RM_ATTRIBUTE_H__ ///< Header guard

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


#include <string>


/**
 * @brief Represents different data types for use withing the library
 */
enum rmAttributeDataType {
    RM_ATTRIBUTE_BOOL, ///< Boolean data type
    RM_ATTRIBUTE_CHAR, ///< A single character
    RM_ATTRIBUTE_INT, ///< Integer data
    RM_ATTRIBUTE_FLOAT, ///< Floating point data type
    RM_ATTRIBUTE_STRING ///< String data type
};


/**
 * @brief The data type of the rmAttribute's value
 * 
 * Since the stored value has multiple possible data types, this union
 * structure is suitable.
 */
union rmAttributeData {
    bool b; ///< Boolean data type
    char c; ///< A single character
    int i; ///< Integer data type
    float f; ///< Floating point data type
    char* s; ///< String data type
};


class rmAttributeNotifier;


/**
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be a string, a number or a blob.
 */
class RM_API rmAttribute {
  private:
    rmAttributeNotifier* notifier = nullptr;
    char name[12] = {0};
    rmAttributeData data;
    rmAttributeDataType type = RM_ATTRIBUTE_STRING;
    float lowerBound;
    float upperBound;
    
  public:
    /**
     * @brief Default constructor
     */
    rmAttribute();
    
    /**
     * @brief Constructs an attribute with a name, type and boundaries
     * 
     * The constructor declares the new attribute on the specified client.
     * 
     * @param key Unique name of the attribute with maximum 11 characters
     * @param t Data type of the value stored
     */
    rmAttribute(const char* key, rmAttributeDataType t);
    
    /**
     * @brief Constructs an attribute with a name, type and boundaries
     * 
     * The constructor declares the new attribute on the specified client.
     * 
     * @param key Unique name of the attribute with maximum 11 characters
     * @param t Data type of the value stored
     * @param lower Lower bound value. The type of the lower and upper should
     *              be of the same type as t.
     * @param upper Upper bound value
     */
    rmAttribute(const char* key, rmAttributeDataType t, float lower,
                float upper);
    
    /**
     * @brief Destructor
     */
    ~rmAttribute();
    
    /**
     * @brief Copy constructor (deleted)
     * 
     * @param attr Source
     */
    rmAttribute(const rmAttribute& attr) = delete;
    
    /**
     * @brief Move constructor
     * 
     * @param attr Source
     */
    rmAttribute(rmAttribute&& attr) noexcept = default;
    
    /**
     * @brief Copy assignment (deleted)
     * 
     * @param attr Source
     */
    rmAttribute& operator=(const rmAttribute& attr) = delete;
    
    /**
     * @brief Move assignment
     * 
     * @param attr Source
     */
    rmAttribute& operator=(rmAttribute&& attr) noexcept = default;
    
    /**
     * @brief Gets the attribute name
     * 
     * @return The name of the attribute
     */
    const char* getName() const;
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The boolean value
     */
    void setValue(bool value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The character value
     */
    void setValue(char value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The integer value
     */
    void setValue(int value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The floating point value
     */
    void setValue(float value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The string value or blob. The array is copied.
     */
    void setValue(const char* value);
    
    /**
     * @brief Gets the value
     * 
     * @return The value in a pointer type which is to be casted to the
     *         appropriate type
     */
    rmAttributeData getValue() const;
    
    /**
     * @brief Gets string value
     * 
     * @return Any type of attribute value parsed into a string
     */
    std::string getValueString() const;
    
    /**
     * @brief Gets the data type of the value
     * 
     * @return The data type number
     */
    rmAttributeDataType getType() const;
    
    /**
     * @brief Sets the boundary
     * 
     * @param lower Lower bound value
     * @param upper Upper bound value
     */
    void setBoundary(float lower, float upper);
    
    /**
     * @brief Gets the lower bound value
     * 
     * @return Lower bound value as a floating point number. If there is no
     *         boundary or the request is invalid, returns NAN.
     */
    float getLowerBound() const;
    
    /**
     * @brief Gets the upper bound value
     * 
     * @return Upper bound value as a floating point number. If there is no
     *         boundary or the request is invalid, returns NAN.
     */
    float getUpperBound() const;
    
    /**
     * @brief Sets the notifier object for this attribute
     * 
     * @param noti The attribute notifier
     */
    void setNotifier(rmAttributeNotifier* noti);
    
    /**
     * @brief Gets the notifier object of this attribute
     * 
     * @return The associated widget. Returns null if it doesn't have.
     */
    rmAttributeNotifier* getNotifier() const;
};


/**
 * @brief Class that handles the events that triggers upon the change of
 *        attribute
 */
class RM_API rmAttributeNotifier {
  private:
    void (*customCallback)() = nullptr;
    
  protected:
    rmAttribute* attribute = nullptr; ///< The attribute to track
    
  public:
    /**
     * @brief Default constructor
     */
    rmAttributeNotifier() = default;
    
    /**
     * @brief Triggers the member or custom callback
     */
    void triggerCallback();
    
    /**
     * @brief Triggers on attribute value change
     * 
     * Triggers when the value of attribute is changed by the client device's
     * reports.
     */
    virtual void onAttributeChange();
    
    /**
     * @brief Sets the custom function to be called on value change
     * 
     * When the custom callback is set, the member function which is triggered
     * at the same instance is not called.
     * 
     * @param func The function that is not a member of a class
     */
    void setCustomCallback(void (*func)());
};

#endif

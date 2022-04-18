/**
 * @file call.hpp
 * @brief Calls that invoke on the client's request
 * 
 * The callback functions take the parameters only as an integer that
 * represents number of extra tokens and the array of strings which are those
 * tokens. This is similar to "int main(int argc, char* argv[])". The tokens
 * strings are to be converted into appropriate data types within the callback.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_CALL_H__
#define __RM_CALL_H__ ///< Header guard

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


class rmClient;


/**
 * @brief Calls that invoke on the client's request
 * 
 * The callback functions take the parameters only as an integer that
 * represents number of extra tokens and the array of strings which are those
 * tokens. The tokens strings are to be converted into appropriate data types
 * within the callback.
 */
class RM_API rmCall {
  private:
    char name[12] = {0};
    void (*callback)(int, char**) = nullptr;
    
  public:
    /**
     * @brief Default constructor
     */
    rmCall() = default;
    
    /**
     * @brief Default constructor
     */
    virtual ~rmCall() = default;
    
    /**
     * @brief Constructs a call with a name and a function pointer
     * 
     * @param key Unique name of the call with maximum 11 characters
     * @param func The callback function. The callback should have two
     *             parameters, an integer representing the number of extra
     *             tokens and the array of strings.
     */
    rmCall(const char* key, void (*func)(int, char**));
    
    /**
     * @brief Gets the call name
     * 
     * @return The name of the call
     */
    const char* getName() const;
    
    /**
     * @brief Invokes the callback of the object
     * 
     * @param argc Argument count
     * @param argv Tokens
     */
    virtual void invoke(int argc, char* argv[]);
};


/**
 * @breif The built in call objects with a privilege to access the client
 */
class RM_API rmBuiltinCall: public rmCall {
  private:
    rmClient* client = nullptr;
    void (*callback2)(int, char**, rmClient*) = nullptr;
    
  public:
    /**
     * @brief Constructs a call with a name and a function pointer
     * 
     * @param key Unique name of the call with maximum 11 characters
     * @param func The callback function. The callback should have two
     *             parameters, an integer representing the number of extra
     *             tokens and the array of strings.
     * @param cli The client instance which the callback has access to
     */
    rmBuiltinCall(const char* key, void (*func)(int, char**, rmClient*),
                  rmClient* cli);
    
    /**
     * @brief Invokes the callback of the object
     * 
     * @param argc Argument count
     * @param argv Tokens
     */
    void invoke(int argc, char *argv[]) override;
};

#endif

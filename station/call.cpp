/**
 * @file call.cpp
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


#define RM_EXPORT


#include "rm/call.hpp"

#include <cstring>


/**
 * @brief Constructs a call with a name and a function pointer
 * 
 * @param key Unique name of the call with maximum 11 characters
 * @param func The callback function. The callback should have two
 *             parameters, an integer representing the number of extra
 *             tokens and the array of strings.
 */
rmCall::rmCall(const char* key, void (*func)(int, char**)) {
    strncpy(name, key, 11);
    name[11] = '\0';
    callback = func;
}

/**
 * @brief Gets the attribute name
 * 
 * @return The name of the attribute
 */
const char* rmCall::getName() const { return name; }

/**
 * @brief Invokes the callback of the object
 * 
 * @param argc Argument count
 * @param argv Tokens
 */
void rmCall::invoke(int argc, char* argv[]) { callback(argc, argv); }




/**
 * @brief Constructs a call with a name and a function pointer
 * 
 * @param key Unique name of the call with maximum 11 characters parameters,
 * @param func The callback function. The callback should have two an integer
 *             representing the number of extra tokens and the array of
 *             strings.
 * @param cli The client instance which the callback has access to
 */
rmBuiltinCall::rmBuiltinCall(const char* key,
                             void (*func)(int, char**, rmClient*),
                             rmClient* cli)
{
    strncpy(name, key, 11);
    name[11] = '\0';
    client = cli;
    callback2 = func;
}

/**
 * @brief Invokes the callback of the object
 * 
 * @param argc Argument count
 * @param argv Tokens
 */
void rmBuiltinCall::invoke(int argc, char *argv[]) override {
    callback2(argc, argv, client);
}

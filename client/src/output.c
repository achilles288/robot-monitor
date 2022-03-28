/**
 * @file output.c
 * @brief Attribute to string conversion functions for the output messages
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/attribute.h"

#include "connection_private.h"

#include <stdio.h>


/**
 * @brief Sends the key and the present data of the attribute to the station
 * 
 * The key and the data is sent as an update command through a connection
 * which is USB or an RF device. This command updates the client's data on the
 * station side.
 * 
 * @param attr The attribute
 */
void rmOutputAttributeUpdate(rmOutputAttribute *attr) {
    char msg[128];
    char* str = rmOutputAttributeGetStringData(attr);
    snprintf(msg, 127, "$set %s %s\n", attr->name, str);
    rmSendMessage(msg);
}


/**
 * @brief Converts the output attribute data to a string
 * 
 * @param attr The attribute
 * 
 * @return String representation of the attribute data. The string returned
 *         must be used or copied before the next function call since it is
 *         allocated in a temporary memory.
 */
char* rmOutputAttributeGetStringData(rmOutputAttribute *attr) {
    return "0.00";
}

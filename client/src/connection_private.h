/**
 * @file connection_private.h
 * @brief Functions that handle the connection to the station
 * 
 * RX and TX buffering and interrupt handling. The processing of input messages
 * also takes place here.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


#define RM_RX_BUFFER_SIZE 256
#define RM_TX_BUFFER_SIZE 512


extern char rmRxBuffer[];
extern uint8_t rmRxHead;
extern uint8_t rmRxTail;

extern char rmTxBuffer[];
extern uint8_t rmTxHead;
extern uint8_t rmTxTail;

extern bool rmTxOn;

extern void (*rmSendMessage)(const char*);

extern char (*rmRead)();


#ifdef __cplusplus
}
#endif

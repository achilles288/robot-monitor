/**
 * @file usbd_cdc_private.h
 * @brief Connection to USB communication device class
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#include <stdint.h>


#include "usbd_def.h"


#ifdef __cplusplus
extern "C" {
#endif

#define __IO volatile

#define CDC_DATA_HS_MAX_PACKET_SIZE 512U


typedef struct {
    uint32_t data[CDC_DATA_HS_MAX_PACKET_SIZE / 4U];
    uint8_t  CmdOpCode;
    uint8_t  CmdLength;
    uint8_t  *RxBuffer;
    uint8_t  *TxBuffer;
    uint32_t RxLength;
    uint32_t TxLength;
    
    __IO uint32_t TxState;
    __IO uint32_t RxState;
} USBD_CDC_HandleTypeDef;


typedef struct _USBD_CDC_Itf {
    int8_t (*Init)();
    int8_t (*DeInit)();
    int8_t (*Control)(uint8_t cmd, uint8_t *pbuf, uint16_t length);
    int8_t (*Receive)(uint8_t *Buf, uint32_t *Len);
} USBD_CDC_ItfTypeDef;


uint8_t USBD_CDC_SetRxBuffer(USBD_HandleTypeDef* pdev, uint8_t* pbuff);

uint8_t USBD_CDC_ReceivePacket(USBD_HandleTypeDef* pdev);

uint8_t USBD_CDC_SetTxBuffer(USBD_HandleTypeDef* pdev, uint8_t* pbuff,
                             uint16_t length);

uint8_t USBD_CDC_TransmitPacket(USBD_HandleTypeDef* pdev);


extern USBD_HandleTypeDef hUsbDeviceFS;

#ifdef __cplusplus
}
#endif

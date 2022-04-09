/**
 * @file usbd_cdc.c
 * @brief Connection to USB communication device class
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#ifdef __arm__

#include "../rm/hal/usbd_cdc.h"
#include "usbd_cdc_private.h"

#include "../connection_private.h"
#include "../time.h"


static int8_t rmUSBDReceive(uint8_t* Buf, uint32_t* Len) {
    for(uint8_t i=0; i<*Len; i++) {
        uint8_t j = (rmRxHead + 1) % RM_RX_BUFFER_SIZE;
        if(j == rmRxTail)
            break;
        rmRxBuffer[rmRxHead] = Buf[i];
        rmRxHead = j;
    }
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return 0U;
}


static void rmUSBDTransmit() {
    USBD_CDC_HandleTypeDef *hcdc
        = (USBD_CDC_HandleTypeDef*) hUsbDeviceFS.pClassData;
    if(hcdc->TxState != 0)
        return;
    
    char buffer[128];
    uint8_t count = 0;
    while(count < 128) {
        if(rmTxTail == rmTxHead)
            break;
        buffer[count] = rmTxBuffer[rmTxTail];
        rmTxTail = (rmTxTail + 1) % RM_TX_BUFFER_SIZE;
        count++;
    }
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t*) buffer, count);
    USBD_CDC_TransmitPacket(&hUsbDeviceFS);
}


static void rmUSBDSendMessage(const char* msg) {
    char c;
    while((c = *msg++) != '\0') {
        uint8_t i = (rmTxHead + 1) % RM_TX_BUFFER_SIZE;
        
        // Waits for the TX to reduce the buffer
        if(i == rmTxTail) {
            uint32_t t1 = _rmGetTime();
            while(i == rmTxTail) {
                if(_rmGetTime() - t1 > 10)
                    rmTxTail = rmTxHead;
            }
        }
        
        rmTxBuffer[rmTxHead] = c;
        rmTxHead = i;
    }
    rmUSBDTransmit();
}

/**
 * @brief Initializes the USB device connection
 */
void rmConnectUSBD() {
    USBD_CDC_ItfTypeDef* fops = (USBD_CDC_ItfTypeDef*) hUsbDeviceFS.pUserData;
    fops->Receive = rmUSBDReceive;
    _rmSendMessage = rmUSBDSendMessage;
    _rmConnectionIdle = rmUSBDTransmit;
}

#endif

/**
 * @file usbd_cdc.h
 * @brief Connection to USB communication device class
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_USBD_CDC_H__
#define __RM_USBD_CDC_H__ ///< Header guard

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initializes the USB connection
 */
void rmConnectUSBD();


#ifdef __cplusplus
}
#endif

#endif

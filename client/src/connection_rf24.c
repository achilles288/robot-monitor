/**
 * @file connection_rf24.c
 * @brief Communication with two RF24 modules
 * 
 * Connects the client and station via two RF24 modules.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/connection_rf24.h"

#include <stdbool.h>

#include "nRF24L01.h"
#ifdef __AVR
#include <util/delay_basic.h>
#endif

#ifndef _BV
#define _BV(I) (1 << I)
#endif

#define ADDR_LEN 5
#define PAYLOAD_SIZE 32


static rmPin cePin;
static uint8_t status;
static uint8_t config_reg;


static inline void csDelay() {
    // 5 microseconds
    #if defined(__AVR)
    _delay_loop_1(5 * (rmFCPU / 3e6));
    #elif defined(__arm__)
    uint8_t ticks = 5 * rmFCPU / 6e6;
    while(ticks-- > 0) {
        asm("nop");
    }
    #endif
}

static inline void txDelay() {
    // 280 microseconds
    #if defined(__AVR)
    _delay_loop_2(280 * (rmFCPU / 4e6));
    #elif defined(__arm__)
    uint16_t ticks = 280 * rmFCPU / 6e6;
    while(ticks-- > 0) {
        asm("nop");
    }
    #endif
}

static inline void beginTransaction() {
    #if defined(__AVR)
    *rmSPCR_mem = rmSPCR;
    *rmSPSR_mem = (*rmSPSR_mem & 0xFE) | rmSPSR;
    *(rmSPISlavePin.io) &= ~(rmSPISlavePin.bitmask);
    #elif defined(__arm__)
    HAL_GPIO_WritePin(rmSPISlavePin.port, rmSPISlavePin.bitmask, 0);
    #endif
    csDelay();
}

static inline void endTransaction() {
    #if defined(__AVR)
    *(rmSPISlavePin.io) |= rmSPISlavePin.bitmask;
    #elif defined(__arm__)
    HAL_GPIO_WritePin(rmSPISlavePin.port, rmSPISlavePin.bitmask, 1);
    #endif
    csDelay();
}

static void writeRegister_cmd(uint8_t reg) {
    beginTransaction();
    #if defined(__AVR)
    status = rmSPITransfer(W_REGISTER | reg);
    #elif defined(__arm__)
    uint8_t c = W_REGISTER | reg;
    HAL_SPI_TransmitReceive(rmSPIHandler, &c, &status, 1, 10);
    #endif
    endTransaction();
}

static void writeRegister(uint8_t reg, uint8_t data) {
    beginTransaction();
    #if defined(__AVR)
    status = rmSPITransfer(W_REGISTER | reg);
    rmSPITransfer(data);
    #elif defined(__arm__)
    uint8_t c = W_REGISTER | reg;
    HAL_SPI_TransmitReceive(rmSPIHandler, &c, &status, 1, 10);
    HAL_SPI_Transmit(rmSPIHandler, &data, 1, 10);
    #endif
    endTransaction();
}

static void writeRegister_n(uint8_t reg, const uint8_t* buf, uint8_t len) {
    beginTransaction();
    #if defined(__AVR)
    status = rmSPITransfer(W_REGISTER | reg);
    while(len--)
        rmSPITransfer(*buf++);
    #elif defined(__arm__)
    uint8_t c = W_REGISTER | reg;
    HAL_SPI_TransmitReceive(rmSPIHandler, &c, &status, 1, 10);
    HAL_SPI_Transmit(rmSPIHandler, buf, len, 100);
    #endif
    endTransaction();
}

static uint8_t readRegister(uint8_t reg) {
    uint8_t res;
    beginTransaction();
    #if defined(__AVR)
    status = rmSPITransfer(R_REGISTER | reg);
    res = rmSPITransfer(0xFF);
    #elif defined(__arm__)
    uint8_t c = R_REGISTER | reg;
    HAL_SPI_TransmitReceive(rmSPIHandler, &c, &status, 1, 10);
    HAL_SPI_Receive(rmSPIHandler, &res, 1, 10);
    #endif
    endTransaction();
    return res;
}

static inline uint8_t getStatus() {
    writeRegister_cmd(RF24_NOP);
    return status;
}

static void sendMessage(const char *msg) {
    #if defined(__AVR)
    *(cePin.io) &= ~(cePin.bitmask);
    #elif defined(__arm__)
    HAL_GPIO_WritePin(cePin.port, cePin.bitmask, 0);
    #endif
    txDelay();
    config_reg &= ~_BV(PRIM_RX);
    writeRegister(NRF_CONFIG, config_reg);
    writeRegister(EN_RXADDR, readRegister(EN_RXADDR) | _BV(ERX_P0));
    
    bool blank = false;
    while(!blank) {
        uint8_t c;
        beginTransaction();
        #if defined(__AVR)
        status = rmSPITransfer(W_TX_PAYLOAD);
        #elif defined(__arm__)
        c = W_TX_PAYLOAD;
        HAL_SPI_TransmitReceive(rmSPIHandler, &c, &status, 1, 10);
        #endif
        for(uint8_t i=0; i<PAYLOAD_SIZE; i++) {
            c = 0x00;
            if(!blank) {
                if(*msg == '\0')
                    blank = true;
                else
                    c = (uint8_t) *msg++;
            }
            #if defined(__AVR)
            rmSPITransfer(c);
            #elif defined(__arm__)
            HAL_SPI_Transmit(rmSPIHandler, &c, 1, 10);
            #endif
        }
        endTransaction();
        
        #if defined(__AVR)
        *(cePin.io) |= cePin.bitmask;
        uint16_t ticks = 800;
        while(!(getStatus() & (_BV(TX_DS) | _BV(MAX_RT)))) {
            if(!(--ticks))
                return;
        }
        #elif defined(__arm__)
        HAL_GPIO_WritePin(cePin.port, cePin.bitmask, 1);
        #endif
        
        writeRegister(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
        // Max retries exceeded
        if(status & _BV(MAX_RT))
            writeRegister_cmd(FLUSH_TX);
    }
    
    config_reg |= _BV(PRIM_RX);
    writeRegister(NRF_CONFIG, config_reg);
    writeRegister(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
    writeRegister(EN_RXADDR, readRegister(EN_RXADDR) & ~_BV(ERX_P0));
}

static char read() {
    if(rmRxTail == rmRxHead) {
        if(((getStatus() >> RX_P_NO) & 0x07) == 1) {
            bool blank = false;
            uint8_t len = PAYLOAD_SIZE;
            uint8_t c;
            beginTransaction();
            #if defined(__AVR)
            status = rmSPITransfer(R_RX_PAYLOAD);
            #elif defined(__arm__)
            c = R_RX_PAYLOAD;
            HAL_SPI_TransmitReceive(rmSPIHandler, &c, &status, 1, 10);
            #endif
            while(len--) {
                if(!blank) {
                    c = rmSPITransfer(0xFF);
                    if(c == 0)
                        blank = true;
                    rmRxBuffer[rmRxHead] = (char) c;
                    rmRxHead = (rmRxHead + 1) % RM_RX_BUFFER_SIZE;
                }
                else {
                    #if defined(__AVR)
                    rmSPITransfer(0xFF);
                    #elif defined(__arm__)
                    c = 0xFF;
                    HAL_SPI_TransmiT(rmSPIHandler, &c, &status, 1, 10);
                    #endif
                }
            }
            endTransaction();
            writeRegister(NRF_STATUS, _BV(RX_DR));
            c = rmRxBuffer[rmRxTail];
            rmRxTail = (rmRxTail + 1) % RM_RX_BUFFER_SIZE;
            return c;
        }
        else
            return '\0';
    }
    else {
        char c = rmRxBuffer[rmRxTail];
        rmRxTail = (rmRxTail + 1) % RM_RX_BUFFER_SIZE;
        return c;
    }
}

/**
 * @breif Configures the RF24 module
 * 
 * @param addr1 The writing pipe address with length of 5 bytes
 * @param addr2 The reading pipe address with length of 5 bytes
 */
void rmRF24Setup(const uint8_t* addr1, const uint8_t* addr2, rmPin ce,
                 rmPin csn)
{
    #if defined(__AVR)
    *(csn.mode) |= csn.bitmask;
    *(csn.io) |= csn.bitmask;
    *(ce.mode) |= ce.bitmask;
    *(ce.io) &= ~(ce.bitmask);
    #elif defined(__arm__)
    HAL_GPIO_WritePin(cePin.port, cePin.bitmask, 0);
    #endif
    rmSPISlavePin = csn;
    cePin = ce;
    
    #if defined(__AVR)
    // 5 milliseconds
    uint16_t ticks = 5 * (rmFCPU / 8e3);
    _delay_loop_2(ticks);
    _delay_loop_2(ticks);
    #elif defined(__arm__)
    HAL_Delay(5);
    #endif
    
    writeRegister(SETUP_RETR, (5 << ARD) | 15);
    uint8_t setup = readRegister(RF_SETUP);
    setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));
    setup |= 0x00; // RF24_1MBPS
    writeRegister(RF_SETUP, setup);
    
    uint8_t before_toggle = readRegister(FEATURE);
    beginTransaction();
    #if defined(__AVR)
    rmSPITransfer(ACTIVATE);
    rmSPITransfer(0x73);
    #elif defined(__arm__)
    uint8_t buf[2];
    buf[0] = ACTIVATE;
    buf[1] = 0x73;
    HAL_SPI_Transmit(rmSPIHandler, buf, 2, 10);
    #endif
    endTransaction();
    uint8_t after_toggle = readRegister(FEATURE);
    if(after_toggle) {
        if(before_toggle == after_toggle) {
            rmSPITransfer(ACTIVATE);
            rmSPITransfer(0x73);
        }
        writeRegister(FEATURE, 0);
    }
    
    writeRegister(DYNPD, 0);
    writeRegister(EN_AA, 0x3F);
    writeRegister(EN_RXADDR, 0x03);
    writeRegister(RX_PW_P0, PAYLOAD_SIZE);
    writeRegister(SETUP_AW, (ADDR_LEN - 2) % 4);
    writeRegister(RF_CH, 76);
    writeRegister(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));

    // Flush buffers
    writeRegister_cmd(FLUSH_RX);
    writeRegister_cmd(FLUSH_TX);
    
    // Power up
    writeRegister(NRF_CONFIG, (_BV(EN_CRC) | _BV(CRCO)));
    config_reg = readRegister(NRF_CONFIG);
    if(!(config_reg & _BV(PWR_UP))) {
        config_reg |= _BV(PWR_UP);
        writeRegister(NRF_CONFIG, config_reg);
        #if defined(__AVR)
        // 5 milliseconds
        _delay_loop_2(ticks);
        _delay_loop_2(ticks);
        #elif defined(__arm__)
        HAL_Delay(5);
        #endif
    }
    
    setup &= 0xF8;
    setup |= 0x00; // RF24_PA_MIN
    writeRegister(RF_SETUP, setup);
    
    writeRegister_n(RX_ADDR_P0, addr1, ADDR_LEN);
    writeRegister_n(TX_ADDR, addr1, ADDR_LEN);
    writeRegister_n(RX_ADDR_P1, addr2, ADDR_LEN);
    config_reg |= _BV(PRIM_RX);
    
    writeRegister(NRF_CONFIG, config_reg);
    writeRegister(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
    #if defined(__AVR)
    *(cePin.io) |= cePin.bitmask;
    #elif defined(__arm__)
    HAL_GPIO_WritePin(cePin.port, cePin.bitmask, 1);
    #endif
    
    rmSendMessage = &sendMessage;
    rmRead = &read;
}

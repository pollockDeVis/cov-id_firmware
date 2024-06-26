
#ifndef _MLX90614_SMBus_Driver_H_
#define _MLX90614_SMBus_Driver_H_

#include <stdint.h>

    void MLX90614_SMBusInit(uint8_t sda_gpio, uint8_t scl_gpio, int freq);
    int MLX90614_SMBusRead(uint8_t slaveAddr,uint8_t readAddress, uint16_t *data);
    int MLX90614_SMBusWrite(uint8_t slaveAddr,uint8_t writeAddress, uint16_t data);
    uint8_t Calculate_PEC (uint8_t initPEC, uint8_t newData);
    int MLX90614_SMBusWrite_mod(uint8_t slaveAddr, uint8_t writeAddress, uint16_t data); //a is address //v is data

#endif

#ifndef L3G_h
#define L3G_h

#include "i2c.h"

// device types

#define L3G_DEVICE_AUTO 0
#define L3G4200D_DEVICE 1
#define L3GD20_DEVICE   2

// device addresses
#define L3G4200D_ADDRESS_SA0_LOW  0xD0
#define L3G4200D_ADDRESS_SA0_HIGH 0xD2
#define L3GD20_ADDRESS_SA0_LOW    0xD4
#define L3GD20_ADDRESS_SA0_HIGH   0xD6

// SA0 states

#define L3G_SA0_LOW  0
#define L3G_SA0_HIGH 1
#define L3G_SA0_AUTO 2

// register addresses

#define L3G_WHO_AM_I      0x0F

#define L3G_CTRL_REG1     0x20
#define L3G_CTRL_REG2     0x21
#define L3G_CTRL_REG3     0x22
#define L3G_CTRL_REG4     0x23
#define L3G_CTRL_REG5     0x24
#define L3G_REFERENCE     0x25
#define L3G_OUT_TEMP      0x26
#define L3G_STATUS_REG    0x27

#define L3G_OUT_X_L       0x28
#define L3G_OUT_X_H       0x29
#define L3G_OUT_Y_L       0x2A
#define L3G_OUT_Y_H       0x2B
#define L3G_OUT_Z_L       0x2C
#define L3G_OUT_Z_H       0x2D

#define L3G_FIFO_CTRL_REG 0x2E
#define L3G_FIFO_SRC_REG  0x2F

#define L3G_INT1_CFG      0x30
#define L3G_INT1_SRC      0x31
#define L3G_INT1_THS_XH   0x32
#define L3G_INT1_THS_XL   0x33
#define L3G_INT1_THS_YH   0x34
#define L3G_INT1_THS_YL   0x35
#define L3G_INT1_THS_ZH   0x36
#define L3G_INT1_THS_ZL   0x37
#define L3G_INT1_DURATION 0x38


#define L3G_GAIN .07 //degrees per second
#define L3G_UNCERTAINTY 5.25 //.07*75
extern volatile uint8_t I2CMasterBuffer[I2C_PORT_NUM][BUFSIZE];
extern volatile uint8_t I2CSlaveBuffer[I2C_PORT_NUM][BUFSIZE];
extern volatile uint32_t I2CReadLength[I2C_PORT_NUM];
extern volatile uint32_t I2CWriteLength[I2C_PORT_NUM];




void L3G_init(int PORT_USED)
{
	  unsigned int i;
	  I2CWriteLength[PORT_USED] = 3;
	  I2CReadLength[PORT_USED] = 0;
	  I2CMasterBuffer[PORT_USED][0] = L3GD20_ADDRESS_SA0_HIGH;
	  I2CMasterBuffer[PORT_USED][1] = L3G_CTRL_REG1;		/* address */
	  I2CMasterBuffer[PORT_USED][2] = 0x0F;		/* Data0 */
	  I2CEngine( PORT_USED );
	  for ( i = 0; i < 0x200000; i++ );	/* Delay after write */
	  I2CWriteLength[PORT_USED] = 3;
	  I2CReadLength[PORT_USED] = 0;
	  I2CMasterBuffer[PORT_USED][0] = L3GD20_ADDRESS_SA0_HIGH;
	  I2CMasterBuffer[PORT_USED][1] = L3G_CTRL_REG4;		/* address */
	  I2CMasterBuffer[PORT_USED][2] = 0x20;		/* Data0 */
	  I2CEngine( PORT_USED );
	  for ( i = 0; i < 0x200000; i++ );	/* Delay after write */
}

sensor_read L3G_read(int PORT_USED)
{
	  sensor_read gyro;
	  I2CWriteLength[PORT_USED] = 2;
	  I2CReadLength[PORT_USED] = 6;
	  I2CMasterBuffer[PORT_USED][0] = L3GD20_ADDRESS_SA0_HIGH;
	  I2CMasterBuffer[PORT_USED][1] = L3G_OUT_X_L | (1 << 7);		/* address */
	  I2CMasterBuffer[PORT_USED][2] = L3GD20_ADDRESS_SA0_HIGH | RD_BIT;
	  I2CEngine( PORT_USED );
	  gyro.y = (int16_t)(I2CSlaveBuffer[PORT_USED][0]|I2CSlaveBuffer[PORT_USED][1]<<8);
	  gyro.x = (int16_t)(I2CSlaveBuffer[PORT_USED][2]|I2CSlaveBuffer[PORT_USED][3]<<8);
	  gyro.z = (int16_t)(I2CSlaveBuffer[PORT_USED][4]|I2CSlaveBuffer[PORT_USED][5]<<8);
	  return gyro;
}

unsigned char L3G_readReg(int PORT_USED, unsigned char Reg)
{
	  I2CWriteLength[PORT_USED] = 2;
	  I2CReadLength[PORT_USED] = 1;
	  I2CMasterBuffer[PORT_USED][0] = L3GD20_ADDRESS_SA0_HIGH;
	  I2CMasterBuffer[PORT_USED][1] = Reg;		/* address */
	  I2CMasterBuffer[PORT_USED][2] = L3GD20_ADDRESS_SA0_HIGH | RD_BIT;
	  I2CEngine( PORT_USED );
	  return (unsigned char)I2CSlaveBuffer[PORT_USED][0];
}

void L3G_writeReg(int PORT_USED, unsigned char Reg, unsigned char Value)
{
	  I2CWriteLength[PORT_USED] = 3;
	  I2CReadLength[PORT_USED] = 0;
	  I2CMasterBuffer[PORT_USED][0] = L3GD20_ADDRESS_SA0_HIGH;
	  I2CMasterBuffer[PORT_USED][1] = Reg;		/* address */
	  I2CMasterBuffer[PORT_USED][2] = Value;
	  I2CEngine( PORT_USED );
}

#endif

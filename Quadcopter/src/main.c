/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"
//#include <redlib.h>


#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "timer.h"
#include <stdio.h>
#include "LPCKalman.h"
#include "i2c.h"
#include "L3G.h"
#include "LSM303.h"
#include "type.h"
#include <math.h>
#include "LPC_Serial.h"

extern uint32_t timer0_m0_counter, timer1_m0_counter;
extern uint32_t timer0_m1_counter, timer1_m1_counter;
extern volatile uint8_t I2CMasterBuffer[I2C_PORT_NUM][BUFSIZE];
extern volatile uint8_t I2CSlaveBuffer[I2C_PORT_NUM][BUFSIZE];
extern volatile uint32_t I2CReadLength[I2C_PORT_NUM];
extern volatile uint32_t I2CWriteLength[I2C_PORT_NUM];

#define SYS_PERIOD 5 //task manager period (mS)
#define GYRO_ACC_INTERVAL 4 //to run every 4 cycles (4 * 5 = 20 mS .. 50 Hz)
#define MAG_INTERVAL 20 //to run every 20 cycles (20 * 5 = 100 mS .. 10 Hz)
#define PORT_USED 2 //connected to I2C port 2
#define PI 3.14159265359
int main(void) {
	uint32_t i, gyro_acc_time,mag_time;
	sensor_read gyro, acc, mag;
	double x2,y2,z2, Ax,Ay,Az, tiltX,tiltY, Cx,Cy,Cz;
	kalman_state* yAxis, *xAxis, *zAxis;
	matrix2df* F,*H,*R, *meas, *Q;
	char buffer[256];
	//-------------------- Initialize Kalman Filter---------------
	yAxis = malloc(sizeof(kalman_state));
	xAxis = malloc(sizeof(kalman_state));
	zAxis = malloc(sizeof(kalman_state));
	meas = mat_Alloc(1,1);
	yAxis->x = mat_AllocZero(2,1);
	yAxis->P = mat_AllocZero(2,2);
	yAxis->P->array[0][0] = 1000;
	//yAxis->P->array[0][1] = .05;
	yAxis->P->array[1][1] = 1000;
	xAxis->x = mat_AllocZero(2,1);
	xAxis->P = mat_AllocZero(2,2);
	xAxis->P->array[0][0] = 1000;
	//xAxis->P->array[0][1] = 1000;
	//xAxis->P->array[1][0] = 1000;
	xAxis->P->array[1][1] = 1000;
	zAxis->x = mat_AllocZero(2,1);
	zAxis->P = mat_AllocZero(2,2);
	zAxis->P->array[0][0] = 1000;
	//zAxis->P->array[0][1] = 1000;
	//zAxis->P->array[1][0] = 1000;
	zAxis->P->array[1][1] = 1000;
	F = mat_Alloc(2,2);
	H = mat_Alloc(1,2);
	R = mat_Alloc(1,1);
	Q = mat_AllocZero(2,2);
	F->array[0][0] =1;
	F->array[0][1] =.02; //Time that has passed
	F->array[1][1] =1;
	F->array[1][0] =0;
	Q->array[0][0] =1;
	Q->array[1][1] =1;
	/* SystemClockUpdate() updates the SystemFrequency variable */
	SystemClockUpdate();

	Serial_Init();
	I2C2Init( );			/* initialize I2c2 */
	LSM303_initAcc(PORT_USED);
	LSM303_initMag(PORT_USED);
	L3G_init(PORT_USED);
	init_timer_mS( 0 );
	enable_timer( 0 );

    for ( i = 0; i < BUFSIZE; i++ )
    {
    	I2CSlaveBuffer[PORT_USED][i] = 0x00;
    }

	i=0;
	gyro_acc_time=mag_time=0;
	while (1)
	{
		if ( (gyro_acc_time >= GYRO_ACC_INTERVAL) )
		{
			//printf("AngleY: %f, Uncertainty: %f\n",state->x->array[0][0],state->P->array[0][0]);
			gyro = L3G_read(PORT_USED);
			acc = LSM303_readAcc(PORT_USED);
			F->array[0][1] =.05; //Time that has passed
			x2 = acc.x*acc.x;
			y2 = acc.y*acc.y;
			z2 = acc.z*acc.z;
			Ax = 180/PI*atan2(acc.x,sqrt(y2+z2));
			Ay = 180/PI*atan2(acc.y,sqrt(x2+z2));
			H->array[0][0] =1; //Set to measure position
			H->array[0][1] =0;
			if((x2+y2+z2)>250000 && (x2+y2+z2)<2250000)
			R->array[0][0]=1;
			else
			R->array[0][0]=90;
			meas->array[0][0]=Ax; //choose measurement
			kal_MeasurementUpdate(xAxis,meas,R,H);
			meas->array[0][0]=Ay; //choose measurement
			kal_MeasurementUpdate(yAxis,meas,R,H);
			H->array[0][0] =0; //Set to measure velocity
			H->array[0][1] =1;
			R->array[0][0] =.1; //Set meaurement uncertainty;
			meas->array[0][0]=-1*L3G_GAIN*((double)gyro.x-30); //choose measurement
			kal_MeasurementUpdate(xAxis,meas,R,H);
			meas->array[0][0]=L3G_GAIN*((double)gyro.y-30); //choose measurement
			kal_MeasurementUpdate(yAxis,meas,R,H);
			meas->array[0][0]=L3G_GAIN*((double)gyro.z-30); //choose measurement
			kal_MeasurementUpdate(zAxis,meas,R,H);
			kal_PredictionUpdate(xAxis,F,Q);
			kal_PredictionUpdate(yAxis,F,Q);
			kal_PredictionUpdate(zAxis,F,Q);
			//printf("Gyro: %f, Velocity: %f, Acc: %f, Angle: %f \n", L3G_GAIN*((double)gyro.x-30),xAxis->x->array[1][0], Ax,xAxis->x->array[0][0] );
			gyro_acc_time -= GYRO_ACC_INTERVAL;
		}
		if ( (mag_time >= MAG_INTERVAL) )
		{
			mag = LSM303_readMag(PORT_USED);
			Cx = ((double)mag.x - Mag_MinX) / (Mag_MaxX - Mag_MinX) * 2 - 1.0;
			Cy = ((double)mag.y - Mag_MinY) / (Mag_MaxY - Mag_MinY) * 2 - 1.0;
			Cz = ((double)mag.z - Mag_MinZ) / (Mag_MaxZ - Mag_MinZ) * 2 - 1.0;
			tiltX = Cy*cos(-PI/180*yAxis->x->array[0][0])+Cz*sin(-PI/180*yAxis->x->array[0][0]);
			tiltY = Cy*sin(PI/180*xAxis->x->array[0][0])*sin(-PI/180*yAxis->x->array[0][0])
					+Cx*cos(PI/180*xAxis->x->array[0][0])
					-Cz*sin(PI/180*xAxis->x->array[0][0])*cos(-PI/180*yAxis->x->array[0][0]);
			Az = 180/PI*atan2(tiltY,tiltX);
			if(Az<0)
				Az+=360;
			H->array[0][0] =1; //Set to measure position
			H->array[0][1] =0;
			R->array[0][0] =5; //Set meaurement uncertainty;
			meas->array[0][0]=Az;
			kal_MeasurementUpdate(zAxis,meas,R,H);
			//kal_PredictionUpdate(zAxis,F,Q);
			//printf("pitch: %f, roll: %f, yaw: %f\n",yAxis->x->array[0][0],xAxis->x->array[0][0],Az);
			sprintf(buffer,"!ANG:%f,%f,%f\n",-xAxis->x->array[0][0],yAxis->x->array[0][0],-zAxis->x->array[0][0]-85);
			Serial_Print(buffer);
			mag_time -= MAG_INTERVAL;
		}
		if(timer0_m0_counter>SYS_PERIOD)
		{
			gyro_acc_time++;
			mag_time++;
			timer0_m0_counter -= SYS_PERIOD;
		}
	}

	return 0 ;
}

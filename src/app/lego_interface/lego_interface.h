/*
 * lego_interface.h
 *
 *  Created on: 23/04/2010
 *      Author: iha
 */

#ifndef LEGO_INTERFACE_H_
#define LEGO_INTERFACE_H_

void init_lego_interface(void);
INT16S touch_sensor(INT8U sensor_no);
INT16S rotation_sensor(INT8U sensor_no);
INT16S light_sensor(INT8U sensor_no);

void motor_speed(INT8U motor_no, INT8S speed);
void brake_motor(INT8U motor_no);

#endif /* LEGO_INTERFACE_H_ */

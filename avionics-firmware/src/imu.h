#ifndef IMU_H
#define IMU_H

extern bool imu_quaternion_update;
extern float imu_quaternion[4];

void imu_start(void);

#endif /* IMU_H */

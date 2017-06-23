#ifndef IMU_H
#define IMU_H

#include "types.h"

extern bool imu_overwrite;
extern imu_raw_t imu_raw_overwrite_data;

void imu_start(void);

#endif /* IMU_H */

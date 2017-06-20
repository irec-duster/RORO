#include <stdint.h>
#include <ch.h>
#include <hal.h>
#include "main.h"

#define DIFF_PRESS_HIGH_RANGE_SENSOR_ADDR   0x58
#define DIFF_PRESS_LOW_RANGE_SENSOR_ADDR    0x28

#define PRESSURE_SENSOR_STATUS_NORMAL       0b00
#define PRESSURE_SENSOR_STATUS_COMMAND      0b01
#define PRESSURE_SENSOR_STATUS_STALE_DATA   0b10
#define PRESSURE_SENSOR_STATUS_DIAGNOSTIC   0b11

/* HSCMRRN001PD2A3, +/- 1psi */
#define PRESSURE_SENSOR1_MAX     6894.76f   /* [Pa] */
#define PRESSURE_SENSOR1_MIN     -6894.76f  /* [Pa] */

/* SSCMRNN015PG5A3 0 - 15 psi*/
#define PRESSURE_SENSOR2_MAX     103421.f    /* [Pa] */
#define PRESSURE_SENSOR2_MIN     0.f         /* [Pa] */

int diff_pressure_low_res(float *p_press, float *p_temp)
{
    uint8_t buf[4];
    msg_t ret;
    i2cAcquireBus(&I2CD2);
    ret = i2cMasterReceiveTimeout(&I2CD2, DIFF_PRESS_LOW_RANGE_SENSOR_ADDR,
                                     buf, sizeof(buf), TIME_INFINITE);
    i2cReleaseBus(&I2CD2);

    if (ret != MSG_OK) {
        return -1;
    }
    uint16_t press = (((uint16_t)buf[0] << 8) | (uint16_t)buf[1]) & 0x3fff;
    uint16_t temp = (((uint16_t)buf[2] << 3) | ((uint16_t)buf[1] >> 5)) & 0x07ff;
    uint8_t status = buf[0] >> 6;

    if (status != PRESSURE_SENSOR_STATUS_NORMAL && status != PRESSURE_SENSOR_STATUS_STALE_DATA) {
        return -1;
    }

    /* Pressure transfer function */
    *p_press = ((float)press - 1638) * (PRESSURE_SENSOR1_MAX - PRESSURE_SENSOR1_MIN)
                / (14745 - 1638) + PRESSURE_SENSOR1_MIN;

    /* Temperature transfer function */
    *p_temp = ((float)temp / 2047 * 200) - 50;

    return 0;
}

int diff_pressure_hi_res(float *p_press, float *p_temp)
{
    uint8_t buf[4];
    msg_t ret;
    i2cAcquireBus(&I2CD2);
    ret = i2cMasterReceiveTimeout(&I2CD2, DIFF_PRESS_HIGH_RANGE_SENSOR_ADDR,
                                     buf, sizeof(buf), TIME_INFINITE);
    i2cReleaseBus(&I2CD2);

    if (ret != MSG_OK) {
        return -1;
    }
    uint16_t press = (((uint16_t)buf[0] << 8) | (uint16_t)buf[1]) & 0x3fff;
    uint16_t temp = (((uint16_t)buf[2] << 3) | ((uint16_t)buf[1] >> 5)) & 0x07ff;
    uint8_t status = buf[0] >> 6;

    if (status != PRESSURE_SENSOR_STATUS_NORMAL && status != PRESSURE_SENSOR_STATUS_STALE_DATA) {
        return -1;
    }

    /* Pressure transfer function */
    *p_press = ((float)press - 1638) * (PRESSURE_SENSOR2_MAX - PRESSURE_SENSOR2_MIN)
                / (14745 - 1638) + PRESSURE_SENSOR2_MIN;

    /* Temperature transfer function */
    *p_temp = ((float)temp / 2047 * 200) - 50;

    return 0;
}

#define PITOT_INVALID_VALUE -42000.0f

float pitot_press_hi_res = PITOT_INVALID_VALUE;
float pitot_temp_hi_res = PITOT_INVALID_VALUE;
float pitot_press_low_res = PITOT_INVALID_VALUE;
float pitot_temp_low_res = PITOT_INVALID_VALUE;

static THD_WORKING_AREA(pitot_thread, 2000);
void pitot_main(void *arg)
{
    (void)arg;
    chRegSetThreadName("pitot");

    /* differential pressure I2C2 init */
    static const I2CConfig i2c_cfg = {
        .op_mode = OPMODE_I2C,
        .clock_speed = 400000,
        .duty_cycle = FAST_DUTY_CYCLE_2
    };
    i2cStart(&I2CD2, &i2c_cfg);

    /* power up sensors */
    palClearPad(GPIOB, GPIOB_DIF_P_SENS_EN_N);
    chThdSleepMilliseconds(10);

    while (1) {
        float p, t;
        if (!diff_pressure_hi_res(&p, &t)) {
            chSysLock();
            pitot_press_hi_res = p;
            pitot_temp_hi_res = t;
            chSysUnlock();
        } else {
            chSysLock();
            pitot_press_hi_res = PITOT_INVALID_VALUE;
            pitot_temp_hi_res = PITOT_INVALID_VALUE;
            chSysUnlock();
        }

        if (!diff_pressure_low_res(&p, &t)) {
            chSysLock();
            pitot_press_low_res = p;
            pitot_temp_low_res = t;
            chSysUnlock();
        } else {
            chSysLock();
            pitot_press_low_res = PITOT_INVALID_VALUE;
            pitot_temp_low_res = PITOT_INVALID_VALUE;
            chSysUnlock();
        }

        chThdSleepMilliseconds(100);
    }
}

void pitot_start(void)
{
    chThdCreateStatic(&pitot_thread, sizeof(pitot_thread), PITOT_PRIO, pitot_main, NULL);
}

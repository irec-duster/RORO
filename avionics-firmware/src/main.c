#include <ch.h>
#include <hal.h>
#include <chprintf.h>

BaseSequentialStream *xbee;

static THD_WORKING_AREA(heartbeat_thread, 200);
void heartbeat_main(void *arg)
{
    (void) arg;
    while (true) {
        palSetPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(80);
        palClearPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(80);
        palSetPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(80);
        palClearPad(GPIOC, GPIOC_LED2);
        chThdSleepMilliseconds(760);
    }
}

#define DIFF_PRESS_HIGH_RANGE_SENSOR_ADDR   0x58
#define DIFF_PRESS_LOW_RANGE_SENSOR_ADDR    0x28

#define PRESSURE_SENSOR_STATUS_NORMAL       0b00
#define PRESSURE_SENSOR_STATUS_COMMAND      0b01
#define PRESSURE_SENSOR_STATUS_STALE_DATA   0b10
#define PRESSURE_SENSOR_STATUS_DIAGNOSTIC   0b11

#define PRESSURE_SENSOR_MAX     6894.76f    /* [Pa] */
#define PRESSURE_SENSOR_MIN     -6894.76f   /* [Pa] */

int differential_pressure_read(float *p_press, float *p_temp)
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
    *p_press = ((float)press - 1638) * (PRESSURE_SENSOR_MAX - PRESSURE_SENSOR_MIN)
                / (14745 - 1638) + PRESSURE_SENSOR_MIN;

    /* Temperature transfer function */
    *p_temp = ((float)temp / 2047 * 200) - 50;

    return 0;
}

int main(void)
{
    /* System initialization */
    halInit();
    chSysInit();

    chThdCreateStatic(&heartbeat_thread, sizeof(heartbeat_thread), NORMALPRIO,
                      heartbeat_main, NULL);

    /* XBee UART init */
    static const SerialConfig xbee_serial_conf = {
        57600,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
    };
    sdStart(&SD3, &xbee_serial_conf);
    xbee = (BaseSequentialStream *)&SD3;

    /* I2C2 init */
    static const I2CConfig i2c_cfg = {
        .op_mode = OPMODE_I2C,
        .clock_speed = 400000,
        .duty_cycle = FAST_DUTY_CYCLE_2
    };
    i2cStart(&I2CD2, &i2c_cfg);

    /* power up sensors */
    palClearPad(GPIOB, GPIOB_DIF_P_SENS_EN_N);
    chThdSleepMilliseconds(10);

    while (true) {
        float p, t;
        int err;

        err = differential_pressure_read(&p, &t);
        if (!err) {
            chprintf(xbee, "pressure %f, temperature %f\n", p, t);
        } else {
            chprintf(xbee, "pressure read error\n");
        }

        chThdSleepMilliseconds(100);
    }
}

#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <string.h>
#include "main.h"
#include "xbee.h"

#define IMU_START_BYTE          0xF7
#define IMU_START_BYTE_W_HEADER 0xF9

/* Data access functions using type punning
 * assumes little endian architecture
 * data sent/recieved as big endian
 */

static void push_byte(uint8_t **write_p, uint8_t byte)
{
    **write_p = byte;
    *write_p += 1;
}

static void push_float(uint8_t **write_p, float f)
{
    union {
        float f;
        uint8_t b[4];
    } data;
    data.f = f;
    push_byte(write_p, data.b[3]);
    push_byte(write_p, data.b[2]);
    push_byte(write_p, data.b[1]);
    push_byte(write_p, data.b[0]);
}

static void push_int32(uint8_t **write_p, int32_t i)
{
    union {
        int32_t i;
        uint8_t b[4];
    } data;
    data.i = i;
    push_byte(write_p, data.b[3]);
    push_byte(write_p, data.b[2]);
    push_byte(write_p, data.b[1]);
    push_byte(write_p, data.b[0]);
}

static void push_uint32(uint8_t **write_p, uint32_t i)
{
    union {
        uint32_t i;
        uint8_t b[4];
    } data;
    data.i = i;
    push_byte(write_p, data.b[3]);
    push_byte(write_p, data.b[2]);
    push_byte(write_p, data.b[1]);
    push_byte(write_p, data.b[0]);
}

static uint8_t checksum(uint8_t *start, uint8_t *end)
{
    uint8_t sum = 0;
    uint8_t *p = start;
    while (p < end) {
        sum += *p;
        p++;
    }
    return sum;
}

static uint8_t pop_byte(uint8_t **read_p)
{
    return *(*read_p)++;
}

static float pop_float(uint8_t **read_p)
{
    union {
        float f;
        uint8_t b[4];
    } data;
    data.b[3] = pop_byte(read_p);
    data.b[2] = pop_byte(read_p);
    data.b[1] = pop_byte(read_p);
    data.b[0] = pop_byte(read_p);
    return data.f;
}

static int32_t pop_int32(uint8_t **read_p)
{
    union {
        int32_t i;
        uint8_t b[4];
    } data;
    data.b[3] = pop_byte(read_p);
    data.b[2] = pop_byte(read_p);
    data.b[1] = pop_byte(read_p);
    data.b[0] = pop_byte(read_p);
    return data.i;
}

static uint32_t pop_uint32(uint8_t **read_p)
{
    union {
        uint32_t i;
        uint8_t b[4];
    } data;
    data.b[3] = pop_byte(read_p);
    data.b[2] = pop_byte(read_p);
    data.b[1] = pop_byte(read_p);
    data.b[0] = pop_byte(read_p);
    return data.i;
}

/* IMU commands */
#define IMU_CMD_GET_ORIENTATION_QUATERNION  0x00
#define IMU_CMD_GET_RAW_ACC                 0x42

/* IMU header bitfield */
#define IMU_RESP_HEADER_SUCCESS             0x01 // 1byte, 0 on success
#define IMU_RESP_HEADER_TIMESTAMP           0x02 // 4byte, microseconds
#define IMU_RESP_HEADER_CMD_ECHO            0x04 // 1byte
#define IMU_RESP_HEADER_CHECKSUM            0x08 // 1byte, addititve checksum over data
#define IMU_RESP_HEADER_LOGICAL_ID          0x10 // 1byte
#define IMU_RESP_HEADER_SERIALNO            0x20 // 4byte
#define IMU_RESP_HEADER_DATA_LEN            0x40 // 1byte

static uint8_t response_header_length(uint8_t bitfield)
{
    uint8_t len = 0;
    if (bitfield & IMU_RESP_HEADER_SUCCESS) {
        len += 1;
    }
    if (bitfield & IMU_RESP_HEADER_TIMESTAMP) {
        len += 4;
    }
    if (bitfield & IMU_RESP_HEADER_CMD_ECHO) {
        len += 1;
    }
    if (bitfield & IMU_RESP_HEADER_CHECKSUM) {
        len += 1;
    }
    if (bitfield & IMU_RESP_HEADER_LOGICAL_ID) {
        len += 1;
    }
    if (bitfield & IMU_RESP_HEADER_SERIALNO) {
        len += 4;
    }
    if (bitfield & IMU_RESP_HEADER_DATA_LEN) {
        len += 1;
    }
    return len;
}


SerialDriver *imu = NULL;
static void imu_uart_init(void)
{
    static const SerialConfig imu_serial_conf = {
        115200,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
    };
    sdStart(&SD2, &imu_serial_conf);
    imu = &SD2;
}

void imu_command(uint8_t cmd, uint8_t *arg, size_t arg_len)
{
    static uint8_t buf[100];
    uint8_t *wp = &buf[0];

    push_byte(&wp, IMU_START_BYTE);
    push_byte(&wp, cmd);

    if (arg != NULL && arg_len > 0) {
        memcpy(wp, arg, arg_len);
        wp += arg_len;
    }

    push_byte(&wp, checksum(&buf[0] + 1, wp));
    ptrdiff_t len = wp - &buf[0];

    sdWrite(imu, buf, len);
}


void imu_read_acc(float *acc)
{
    imu_command(IMU_CMD_GET_RAW_ACC, NULL, 0);

    uint8_t buf[12];
    if (sdReadTimeout(imu, buf, sizeof(buf), MS2ST(100)) != sizeof(buf)) {
        return;
    }
    uint8_t *rp = &buf[0];
    acc[0] = pop_float(&rp);
    acc[1] = pop_float(&rp);
    acc[2] = pop_float(&rp);
}

void imu_read_quaternion(float *q)
{
    imu_command(IMU_CMD_GET_ORIENTATION_QUATERNION, NULL, 0);

    uint8_t buf[16];
    if (sdReadTimeout(imu, buf, sizeof(buf), MS2ST(100)) != sizeof(buf)) {
        return;
    }
    uint8_t *rp = &buf[0];
    q[0] = pop_float(&rp);
    q[1] = pop_float(&rp);
    q[2] = pop_float(&rp);
    q[3] = pop_float(&rp);
}

static THD_WORKING_AREA(imu_thread, 2000);
void imu_main(void *arg)
{
    (void)arg;

    palClearPad(GPIOD, GPIOD_IMU_EN_N);
    chThdSleepMilliseconds(100);

    // response header config: no response header
    uint8_t buf[4];
    uint8_t *wp = buf;
    push_uint32(&wp, 0x00);
    imu_command(221, buf, sizeof(buf));

    while (1) {
        float acc[3];
        imu_read_acc(acc);
        // chprintf(debug, "%f %f %f\n", acc[0], acc[1], acc[2]);

        float q[4];
        imu_read_quaternion(q);
        chprintf(xbee, "%f, %f, %f, %f\n", q[0], q[1], q[2], q[3]);

        chThdSleepMilliseconds(100);
    }
}

void imu_start(void)
{
    imu_uart_init();

    chThdCreateStatic(&imu_thread, sizeof(imu_thread), IMU_PRIO, imu_main, NULL);
}

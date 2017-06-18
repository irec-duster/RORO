#include <ch.h>
#include <hal.h>

BaseSequentialStream *imu = NULL;
static void imu_uart_init(void)
{
    static const SerialConfig imu_serial_conf = {
        115200,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
    };
    sdStart(&SD2, &imu_serial_conf);
    imu = (BaseSequentialStream *)&SD2;
}

static void push_byte(uint8_t **write_p, uint8_t byte)
{
    **write_p = byte;
    *write_p += 1;
}

static void push_float(uint8_t **write_p, float f)
{
    /* access bytes using type punning, assumes little endian architecture */
    union {
        float f;
        uint8_t b[4];
    } data;
    data.f = f;
    /* write as big endian */
    push_byte(write_p, data.b[3]);
    push_byte(write_p, data.b[2]);
    push_byte(write_p, data.b[1]);
    push_byte(write_p, data.b[0]);
}

static void push_int32(uint8_t **write_p, int32_t i)
{
    /* access bytes using type punning, assumes little endian architecture */
    union {
        int32_t i;
        uint8_t b[4];
    } data;
    data.i = i;
    /* write as big endian */
    push_byte(write_p, data.b[3]);
    push_byte(write_p, data.b[2]);
    push_byte(write_p, data.b[1]);
    push_byte(write_p, data.b[0]);
}

static void push_uint32(uint8_t **write_p, uint32_t i)
{
    /* access bytes using type punning, assumes little endian architecture */
    union {
        uint32_t i;
        uint8_t b[4];
    } data;
    data.i = i;
    /* write as big endian */
    push_byte(write_p, data.b[3]);
    push_byte(write_p, data.b[2]);
    push_byte(write_p, data.b[1]);
    push_byte(write_p, data.b[0]);
}

static uint8_t packet_checksum(uint8_t *start, uint8_t *end)
{
    uint8_t sum = 0;
    uint8_t *p = start;
    while (p < end) {
        sum += *p;
        p++;
    }
    return sum;
}

#define HEADER_SUCCESS 0x01 // 1byte, 0 on success
#define HEADER_TIMESTAMP 0x02 // 4byte, microseconds
#define HEADER_CMD_ECHO 0x04 // 1byte
#define HEADER_CHECKSUM 0x08 // 1byte, addititve checksum over data
#define HEADER_LOGICAL_ID 0x10 // 1byte
#define HEADER_SERIALNO 0x20 // 4byte
#define HEADER_DATA_LEN 0x40 // 1byte

static uint8_t response_header_length(uint8_t bitfield)
{
    uint8_t len = 0;
    if (bitfield & HEADER_SUCCESS) {
        len += 1;
    }
    if (bitfield & HEADER_TIMESTAMP) {
        len += 4;
    }
    if (bitfield & HEADER_CMD_ECHO) {
        len += 1;
    }
    if (bitfield & HEADER_CHECKSUM) {
        len += 1;
    }
    if (bitfield & HEADER_LOGICAL_ID) {
        len += 1;
    }
    if (bitfield & HEADER_SERIALNO) {
        len += 4;
    }
    if (bitfield & HEADER_DATA_LEN) {
        len += 1;
    }
    return len;
}

void imu_init(void)
{
    imu_uart_init();
}

void imu_start(void)
{
    palClearPad(GPIOD, GPIOD_IMU_EN_N);
    chThdSleepMilliseconds(100);

}

/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F4_DISCOVERY
#define BOARD_NAME                  "EPFL RORO Avionics"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                25000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F405xx

/*
 * IO pins assignments.
 */
#define GPIOA_TP1                   0U // PA0 TP1
#define GPIOA_V_BAT1                1U // PA1 V_BAT1 (ADC123) IN1
#define GPIOA_V_BAT2                2U // PA2 V_BAT2 (ADC123) IN2
#define GPIOA_V_BAT3                3U // PA3 V_BAT3 (ADC123) IN3
#define GPIOA_V_BAT4                4U // PA4 V_BAT4 (ADC123) IN4
#define GPIOA_AUX_IO1               5U // PA5 AUX_IO1
#define GPIOA_AUX_IO2               6U // PA6 AUX_IO2
#define GPIOA_AUX_IO3               7U // PA7 AUX_IO3
#define GPIOA_AUX_IO4               8U // PA8 AUX_IO4
#define GPIOA_AUX_IO5               9U // PA9 AUX_IO5
#define GPIOA_AUX_IO6               10U // PA10 AUX_IO6
#define GPIOA_OTG_FS_DM             11U // PA11 OTG_FS_DM
#define GPIOA_OTG_FS_DP             12U // PA12 OTG_FS_DP
#define GPIOA_SWDIO                 13U // PA13 SWDIO
#define GPIOA_SWCLK                 14U // PA14 SWCLK
#define GPIOA_PIN15                 15U // PA15 NC (JTDI)

#define GPIOB_ABS_P_SENS_EN_N       0U // PB0 ABS_P_SENS_ENn
#define GPIOB_DIF_P_SENS_EN_N       1U // PB1 DIF_P_SENS_ENn
#define GPIOB_BOOT1                 2U // PB2 BOOT1
#define GPIOB_SWO                   3U // PB3 SWO
#define GPIOB_PIN4                  4U // PB4 NC (NJRST)
#define GPIOB_PIN5                  5U // PB5 NC
#define GPIOB_USART1_TX             6U // PB6 USART1_TX (RS232_3)
#define GPIOB_USART1_RX             7U // PB7 USART1_RX (RS232_3)
#define GPIOB_GPS_TIMEPULSE         8U // PB8 GPS_TIMEPULSE
#define GPIOB_IMU_INT0              9U // PB9 IMU_INT0
#define GPIOB_I2C2_SCL              10U // PB10 I2C2_SCL (MS5637, DP, GNSS)
#define GPIOB_I2C2_SDA              11U // PB11 I2C2_SDA (MS5637, DP, GNSS)
#define GPIOB_SPI2_NSS              12U // PB12 SPI2_NSS (flash)
#define GPIOB_SPI2_SCK              13U // PB13 SPI2_SCK (flash)
#define GPIOB_SPI2_MISO             14U // PB14 SPI2_MISO (flash)
#define GPIOB_SPI2_MOSI             15U // PB15 SPI2_MOSI (flash)

#define GPIOC_LED1                  0U // PC0 LED1, red
#define GPIOC_LED2                  1U // PC1 LED2, green
#define GPIOC_LED3                  2U // PC2 LED3, blue
#define GPIOC_GPS_EXTINT            3U // PC3 GPS_EXTINT
#define GPIOC_XBEE_RSSI             4U // PC4 XBEE_RSSI, RX signal strength indicator
#define GPIOC_GPS_EN_N              5U // PC5 GPS_ENn
#define GPIOC_UART6_TX              6U // PC6 UART6_TX (GPS)
#define GPIOC_UART6_RX              7U // PC7 UART6_RX (GPS)
#define GPIOC_GPS_RESET_N           8U // PC8 GPS_RESETn
#define GPIOC_GPS_SWITCH            9U // PC9 GPS_SWITCH
#define GPIOC_USART4_TX             10U // PC10 USART4_TX (RS232_1)
#define GPIOC_USART4_RX             11U // PC11 USART4_RX (RS232_1)
#define GPIOC_PIN12                 12U // PC12 NC
#define GPIOC_PIN13                 13U // PC13 NC
#define GPIOC_PIN14                 14U // PC14 NC (OSC32_IN)
#define GPIOC_PIN15                 15U // PC15 NC (OSC32_OUT)

#define GPIOD_FLASH_WP              0U // PD0 FLASH_WP
#define GPIOD_FLASH_HOLD            1U // PD1 FLASH_HOLD
#define GPIOD_IMU_EN_N              2U // PD2 IMU_ENn
#define GPIOD_PIN3                  3U // PD3 NC
#define GPIOD_PIN4                  4U // PD4 NC
#define GPIOD_UART2_TX              5U // PD5 UART2_TX (RS232_2)
#define GPIOD_UART2_RX              6U // PD6 UART2_RX (RS232_2)
#define GPIOD_PIN7                  7U // PD7 NC
#define GPIOD_USART3_TX             8U // PD8 USART3_TX (XBEE)
#define GPIOD_USART3_RX             9U // PD9 USART3_RX (XBEE)
#define GPIOD_PIN10                 10U // PD10 NC
#define GPIOD_USART3_CTS            11U // PD11 USART3_CTS (XBEE)
#define GPIOD_USART3_RTS            12U // PD12 USART3_RTS (XBEE)
#define GPIOD_XBEE_RESET_N          13U // PD13 XBEE_RESET
#define GPIOD_BUZZER                14U // PD14 BUZZER
#define GPIOD_PIN15                 15U // PD15 NC

#define GPIOE_PIN0                  0U // PE0 NC
#define GPIOE_PIN1                  1U // PE1 NC
#define GPIOE_PIN2                  2U // PE2 NC
#define GPIOE_PIN3                  3U // PE3 NC
#define GPIOE_PIN4                  4U // PE4 NC
#define GPIOE_SERVO_1               5U // PE5 SERVO_1 TIM9_CH1
#define GPIOE_SERVO_2               6U // PE6 SERVO_2 TIM9_CH2
#define GPIOE_PIN7                  7U // PE7 NC
#define GPIOE_PIN8                  8U // PE8 NC
#define GPIOE_SERVO_3               9U // PE9 SERVO_3 TIM1_CH1
#define GPIOE_PIN10                 10U // PE10 NC
#define GPIOE_SERVO_4               11U // PE11 SERVO_4 TIM1_CH2
#define GPIOE_PIN12                 12U // PE12 NC
#define GPIOE_SERVO_5               13U // PE13 SERVO_5 TIM1_CH3
#define GPIOE_SERVO_6               14U // PE14 SERVO_6 TIM1_CH4
#define GPIOE_PIN15                 15U // PE15 NC

#define GPIOH_OSC_IN                0U // PH0 OSC_IN
#define GPIOH_OSC_OUT               1U // PH1 OSC_OUT

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_TP1) | \
                                     PIN_MODE_ANALOG(GPIOA_V_BAT1) | \
                                     PIN_MODE_ANALOG(GPIOA_V_BAT2) | \
                                     PIN_MODE_ANALOG(GPIOA_V_BAT3) | \
                                     PIN_MODE_ANALOG(GPIOA_V_BAT4) | \
                                     PIN_MODE_INPUT(GPIOA_AUX_IO1) | \
                                     PIN_MODE_INPUT(GPIOA_AUX_IO2) | \
                                     PIN_MODE_INPUT(GPIOA_AUX_IO3) | \
                                     PIN_MODE_INPUT(GPIOA_AUX_IO4) | \
                                     PIN_MODE_INPUT(GPIOA_AUX_IO5) | \
                                     PIN_MODE_INPUT(GPIOA_AUX_IO6) | \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) | \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) | \
                                     PIN_MODE_INPUT(GPIOA_PIN15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_TP1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_V_BAT1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_V_BAT2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_V_BAT3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_V_BAT4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_AUX_IO1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_AUX_IO2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_AUX_IO3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_AUX_IO4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_AUX_IO5) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_AUX_IO6) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN15))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_TP1) | \
                                     PIN_OSPEED_HIGH(GPIOA_V_BAT1) | \
                                     PIN_OSPEED_HIGH(GPIOA_V_BAT2) | \
                                     PIN_OSPEED_HIGH(GPIOA_V_BAT3) | \
                                     PIN_OSPEED_HIGH(GPIOA_V_BAT4) | \
                                     PIN_OSPEED_HIGH(GPIOA_AUX_IO1) | \
                                     PIN_OSPEED_HIGH(GPIOA_AUX_IO2) | \
                                     PIN_OSPEED_HIGH(GPIOA_AUX_IO3) | \
                                     PIN_OSPEED_HIGH(GPIOA_AUX_IO4) | \
                                     PIN_OSPEED_HIGH(GPIOA_AUX_IO5) | \
                                     PIN_OSPEED_HIGH(GPIOA_AUX_IO6) | \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) | \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) | \
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO) | \
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK) | \
                                     PIN_OSPEED_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_TP1) | \
                                     PIN_PUPDR_FLOATING(GPIOA_V_BAT1) | \
                                     PIN_PUPDR_FLOATING(GPIOA_V_BAT2) | \
                                     PIN_PUPDR_FLOATING(GPIOA_V_BAT3) | \
                                     PIN_PUPDR_FLOATING(GPIOA_V_BAT4) | \
                                     PIN_PUPDR_PULLUP(GPIOA_AUX_IO1) | \
                                     PIN_PUPDR_PULLUP(GPIOA_AUX_IO2) | \
                                     PIN_PUPDR_PULLUP(GPIOA_AUX_IO3) | \
                                     PIN_PUPDR_PULLUP(GPIOA_AUX_IO4) | \
                                     PIN_PUPDR_PULLUP(GPIOA_AUX_IO5) | \
                                     PIN_PUPDR_PULLUP(GPIOA_AUX_IO6) | \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) | \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) | \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO) | \
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK) | \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_TP1) | \
                                     PIN_ODR_HIGH(GPIOA_V_BAT1) | \
                                     PIN_ODR_HIGH(GPIOA_V_BAT2) | \
                                     PIN_ODR_HIGH(GPIOA_V_BAT3) | \
                                     PIN_ODR_HIGH(GPIOA_V_BAT4) | \
                                     PIN_ODR_HIGH(GPIOA_AUX_IO1) | \
                                     PIN_ODR_HIGH(GPIOA_AUX_IO2) | \
                                     PIN_ODR_HIGH(GPIOA_AUX_IO3) | \
                                     PIN_ODR_HIGH(GPIOA_AUX_IO4) | \
                                     PIN_ODR_HIGH(GPIOA_AUX_IO5) | \
                                     PIN_ODR_HIGH(GPIOA_AUX_IO6) | \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) | \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) | \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) | \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) | \
                                     PIN_ODR_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_TP1, 0U) | \
                                     PIN_AFIO_AF(GPIOA_V_BAT1, 0U) | \
                                     PIN_AFIO_AF(GPIOA_V_BAT2, 0U) | \
                                     PIN_AFIO_AF(GPIOA_V_BAT3, 0U) | \
                                     PIN_AFIO_AF(GPIOA_V_BAT4, 0U) | \
                                     PIN_AFIO_AF(GPIOA_AUX_IO1, 0U) | \
                                     PIN_AFIO_AF(GPIOA_AUX_IO2, 0U) | \
                                     PIN_AFIO_AF(GPIOA_AUX_IO3, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_AUX_IO4, 0U) | \
                                     PIN_AFIO_AF(GPIOA_AUX_IO5, 0U) | \
                                     PIN_AFIO_AF(GPIOA_AUX_IO6, 0U) | \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10U) | \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10U) | \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U) | \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U) | \
                                     PIN_AFIO_AF(GPIOA_PIN15, 0U))

/*
 * GPIOB setup:
 */
#define VAL_GPIOB_MODER             (PIN_MODE_OUTPUT(GPIOB_ABS_P_SENS_EN_N) | \
                                     PIN_MODE_OUTPUT(GPIOB_DIF_P_SENS_EN_N) | \
                                     PIN_MODE_INPUT(GPIOB_BOOT1) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SWO) | \
                                     PIN_MODE_INPUT(GPIOB_PIN4) | \
                                     PIN_MODE_INPUT(GPIOB_PIN5) | \
                                     PIN_MODE_ALTERNATE(GPIOB_USART1_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOB_USART1_RX) | \
                                     PIN_MODE_INPUT(GPIOB_GPS_TIMEPULSE) | \
                                     PIN_MODE_ALTERNATE(GPIOB_IMU_INT0) | \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C2_SCL) | \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C2_SDA) | \
                                     PIN_MODE_OUTPUT(GPIOB_SPI2_NSS) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_SCK) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_MISO) | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_MOSI))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_ABS_P_SENS_EN_N) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_DIF_P_SENS_EN_N) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_BOOT1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SWO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USART1_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_USART1_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_GPS_TIMEPULSE) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_IMU_INT0) | \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C2_SCL) | \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C2_SDA) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_NSS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_SCK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_MISO) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_MOSI))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_ABS_P_SENS_EN_N) | \
                                     PIN_OSPEED_HIGH(GPIOB_DIF_P_SENS_EN_N) | \
                                     PIN_OSPEED_HIGH(GPIOB_BOOT1) | \
                                     PIN_OSPEED_HIGH(GPIOB_SWO) | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN4) | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN5) | \
                                     PIN_OSPEED_HIGH(GPIOB_USART1_TX) | \
                                     PIN_OSPEED_HIGH(GPIOB_USART1_RX) | \
                                     PIN_OSPEED_HIGH(GPIOB_GPS_TIMEPULSE) | \
                                     PIN_OSPEED_HIGH(GPIOB_IMU_INT0) | \
                                     PIN_OSPEED_HIGH(GPIOB_I2C2_SCL) | \
                                     PIN_OSPEED_HIGH(GPIOB_I2C2_SDA) | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI2_NSS) | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI2_SCK) | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI2_MISO) | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI2_MOSI))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_ABS_P_SENS_EN_N) | \
                                     PIN_PUPDR_FLOATING(GPIOB_DIF_P_SENS_EN_N) | \
                                     PIN_PUPDR_PULLUP(GPIOB_BOOT1) | \
                                     PIN_PUPDR_FLOATING(GPIOB_SWO) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN4) | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN5) | \
                                     PIN_PUPDR_FLOATING(GPIOB_USART1_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOB_USART1_RX) | \
                                     PIN_PUPDR_PULLDOWN(GPIOB_GPS_TIMEPULSE) | \
                                     PIN_PUPDR_PULLUP(GPIOB_IMU_INT0) | \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C2_SCL) | \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C2_SDA) | \
                                     PIN_PUPDR_PULLUP(GPIOB_SPI2_NSS) | \
                                     PIN_PUPDR_PULLUP(GPIOB_SPI2_SCK) | \
                                     PIN_PUPDR_PULLUP(GPIOB_SPI2_MISO) | \
                                     PIN_PUPDR_PULLUP(GPIOB_SPI2_MOSI))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_ABS_P_SENS_EN_N) | \
                                     PIN_ODR_HIGH(GPIOB_DIF_P_SENS_EN_N) | \
                                     PIN_ODR_HIGH(GPIOB_BOOT1) | \
                                     PIN_ODR_HIGH(GPIOB_SWO) | \
                                     PIN_ODR_HIGH(GPIOB_PIN4) | \
                                     PIN_ODR_HIGH(GPIOB_PIN5) | \
                                     PIN_ODR_HIGH(GPIOB_USART1_TX) | \
                                     PIN_ODR_HIGH(GPIOB_USART1_RX) | \
                                     PIN_ODR_HIGH(GPIOB_GPS_TIMEPULSE) | \
                                     PIN_ODR_HIGH(GPIOB_IMU_INT0) | \
                                     PIN_ODR_HIGH(GPIOB_I2C2_SCL) | \
                                     PIN_ODR_HIGH(GPIOB_I2C2_SDA) | \
                                     PIN_ODR_HIGH(GPIOB_SPI2_NSS) | \
                                     PIN_ODR_HIGH(GPIOB_SPI2_SCK) | \
                                     PIN_ODR_HIGH(GPIOB_SPI2_MISO) | \
                                     PIN_ODR_HIGH(GPIOB_SPI2_MOSI))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_ABS_P_SENS_EN_N, 0U) | \
                                     PIN_AFIO_AF(GPIOB_DIF_P_SENS_EN_N, 0U) | \
                                     PIN_AFIO_AF(GPIOB_BOOT1, 0U) | \
                                     PIN_AFIO_AF(GPIOB_SWO, 0U) | \
                                     PIN_AFIO_AF(GPIOB_PIN4, 0U) | \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0U) | \
                                     PIN_AFIO_AF(GPIOB_USART1_TX, 7U) | \
                                     PIN_AFIO_AF(GPIOB_USART1_RX, 7U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_GPS_TIMEPULSE, 0U) | \
                                     PIN_AFIO_AF(GPIOB_IMU_INT0, 0U) | \
                                     PIN_AFIO_AF(GPIOB_I2C2_SCL, 4U) | \
                                     PIN_AFIO_AF(GPIOB_I2C2_SDA, 4U) | \
                                     PIN_AFIO_AF(GPIOB_SPI2_NSS, 0U) | \
                                     PIN_AFIO_AF(GPIOB_SPI2_SCK, 5U) | \
                                     PIN_AFIO_AF(GPIOB_SPI2_MISO, 5U) | \
                                     PIN_AFIO_AF(GPIOB_SPI2_MOSI, 5U))

/*
 * GPIOC setup:
 */
#define VAL_GPIOC_MODER             (PIN_MODE_OUTPUT(GPIOC_LED1) | \
                                     PIN_MODE_OUTPUT(GPIOC_LED2) | \
                                     PIN_MODE_OUTPUT(GPIOC_LED3) | \
                                     PIN_MODE_INPUT(GPIOC_GPS_EXTINT) | \
                                     PIN_MODE_INPUT(GPIOC_XBEE_RSSI) | \
                                     PIN_MODE_OUTPUT(GPIOC_GPS_EN_N) | \
                                     PIN_MODE_INPUT(GPIOC_UART6_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOC_UART6_RX) | \
                                     PIN_MODE_OUTPUT(GPIOC_GPS_RESET_N) | \
                                     PIN_MODE_OUTPUT(GPIOC_GPS_SWITCH) | \
                                     PIN_MODE_ALTERNATE(GPIOC_USART4_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOC_USART4_RX) | \
                                     PIN_MODE_INPUT(GPIOC_PIN12) | \
                                     PIN_MODE_INPUT(GPIOC_PIN13) | \
                                     PIN_MODE_INPUT(GPIOC_PIN14) | \
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_LED1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_GPS_EXTINT) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_XBEE_RSSI) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_GPS_EN_N) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_UART6_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_UART6_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_GPS_RESET_N) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_GPS_SWITCH) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_USART4_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_USART4_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN12) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_LED1) | \
                                     PIN_OSPEED_HIGH(GPIOC_LED2) | \
                                     PIN_OSPEED_HIGH(GPIOC_LED3) | \
                                     PIN_OSPEED_HIGH(GPIOC_GPS_EXTINT) | \
                                     PIN_OSPEED_HIGH(GPIOC_XBEE_RSSI) | \
                                     PIN_OSPEED_HIGH(GPIOC_GPS_EN_N) | \
                                     PIN_OSPEED_HIGH(GPIOC_UART6_TX) | \
                                     PIN_OSPEED_HIGH(GPIOC_UART6_RX) | \
                                     PIN_OSPEED_HIGH(GPIOC_GPS_RESET_N) | \
                                     PIN_OSPEED_HIGH(GPIOC_GPS_SWITCH) | \
                                     PIN_OSPEED_HIGH(GPIOC_USART4_TX) | \
                                     PIN_OSPEED_HIGH(GPIOC_USART4_RX) | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN12) | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN13) | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN14) | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_LED1) | \
                                     PIN_PUPDR_FLOATING(GPIOC_LED2) | \
                                     PIN_PUPDR_FLOATING(GPIOC_LED3) | \
                                     PIN_PUPDR_PULLUP(GPIOC_GPS_EXTINT) | \
                                     PIN_PUPDR_PULLUP(GPIOC_XBEE_RSSI) | \
                                     PIN_PUPDR_FLOATING(GPIOC_GPS_EN_N) | \
                                     PIN_PUPDR_FLOATING(GPIOC_UART6_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOC_UART6_RX) | \
                                     PIN_PUPDR_FLOATING(GPIOC_GPS_RESET_N) | \
                                     PIN_PUPDR_FLOATING(GPIOC_GPS_SWITCH) | \
                                     PIN_PUPDR_FLOATING(GPIOC_USART4_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOC_USART4_RX) | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN12) | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN13) | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN14) | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_LOW(GPIOC_LED1) | \
                                     PIN_ODR_LOW(GPIOC_LED2) | \
                                     PIN_ODR_LOW(GPIOC_LED3) | \
                                     PIN_ODR_HIGH(GPIOC_GPS_EXTINT) | \
                                     PIN_ODR_HIGH(GPIOC_XBEE_RSSI) | \
                                     PIN_ODR_HIGH(GPIOC_GPS_EN_N) | \
                                     PIN_ODR_HIGH(GPIOC_UART6_TX) | \
                                     PIN_ODR_HIGH(GPIOC_UART6_RX) | \
                                     PIN_ODR_HIGH(GPIOC_GPS_RESET_N) | \
                                     PIN_ODR_HIGH(GPIOC_GPS_SWITCH) | \
                                     PIN_ODR_HIGH(GPIOC_USART4_TX) | \
                                     PIN_ODR_HIGH(GPIOC_USART4_RX) | \
                                     PIN_ODR_HIGH(GPIOC_PIN12) | \
                                     PIN_ODR_HIGH(GPIOC_PIN13) | \
                                     PIN_ODR_HIGH(GPIOC_PIN14) | \
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_LED1, 0U) | \
                                     PIN_AFIO_AF(GPIOC_LED2, 0U) | \
                                     PIN_AFIO_AF(GPIOC_LED3, 0U) | \
                                     PIN_AFIO_AF(GPIOC_GPS_EXTINT, 0U) | \
                                     PIN_AFIO_AF(GPIOC_XBEE_RSSI, 0U) | \
                                     PIN_AFIO_AF(GPIOC_GPS_EN_N, 0U) | \
                                     PIN_AFIO_AF(GPIOC_UART6_TX, 8U) | \
                                     PIN_AFIO_AF(GPIOC_UART6_RX, 8U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_GPS_RESET_N, 0U) | \
                                     PIN_AFIO_AF(GPIOC_GPS_SWITCH, 0U) | \
                                     PIN_AFIO_AF(GPIOC_USART4_TX, 8U) | \
                                     PIN_AFIO_AF(GPIOC_USART4_RX, 8U) | \
                                     PIN_AFIO_AF(GPIOC_PIN12, 0U) | \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0U) | \
                                     PIN_AFIO_AF(GPIOC_PIN14, 0U) | \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0U))

/*
 * GPIOD setup:
 */
#define VAL_GPIOD_MODER             (PIN_MODE_OUTPUT(GPIOD_FLASH_WP) | \
                                     PIN_MODE_OUTPUT(GPIOD_FLASH_HOLD) | \
                                     PIN_MODE_OUTPUT(GPIOD_IMU_EN_N) | \
                                     PIN_MODE_INPUT(GPIOD_PIN3) | \
                                     PIN_MODE_INPUT(GPIOD_PIN4) | \
                                     PIN_MODE_ALTERNATE(GPIOD_UART2_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOD_UART2_RX) | \
                                     PIN_MODE_INPUT(GPIOD_PIN7) | \
                                     PIN_MODE_ALTERNATE(GPIOD_USART3_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOD_USART3_RX) | \
                                     PIN_MODE_INPUT(GPIOD_PIN10) | \
                                     PIN_MODE_ALTERNATE(GPIOD_USART3_CTS) | \
                                     PIN_MODE_ALTERNATE(GPIOD_USART3_RTS) | \
                                     PIN_MODE_OUTPUT(GPIOD_XBEE_RESET_N) | \
                                     PIN_MODE_OUTPUT(GPIOD_BUZZER) | \
                                     PIN_MODE_INPUT(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_FLASH_WP) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_FLASH_HOLD) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_IMU_EN_N) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_UART2_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_UART2_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_CTS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_RTS) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_XBEE_RESET_N) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_BUZZER) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_FLASH_WP) | \
                                     PIN_OSPEED_HIGH(GPIOD_FLASH_HOLD) | \
                                     PIN_OSPEED_HIGH(GPIOD_IMU_EN_N) | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN3) | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN4) | \
                                     PIN_OSPEED_HIGH(GPIOD_UART2_TX) | \
                                     PIN_OSPEED_HIGH(GPIOD_UART2_RX) | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN7) | \
                                     PIN_OSPEED_HIGH(GPIOD_USART3_TX) | \
                                     PIN_OSPEED_HIGH(GPIOD_USART3_RX) | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN10) | \
                                     PIN_OSPEED_HIGH(GPIOD_USART3_CTS) | \
                                     PIN_OSPEED_HIGH(GPIOD_USART3_RTS) | \
                                     PIN_OSPEED_HIGH(GPIOD_XBEE_RESET_N) | \
                                     PIN_OSPEED_HIGH(GPIOD_BUZZER) | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_FLASH_WP) | \
                                     PIN_PUPDR_FLOATING(GPIOD_FLASH_HOLD) | \
                                     PIN_PUPDR_FLOATING(GPIOD_IMU_EN_N) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN4) | \
                                     PIN_PUPDR_FLOATING(GPIOD_UART2_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_UART2_RX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN7) | \
                                     PIN_PUPDR_FLOATING(GPIOD_USART3_TX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_USART3_RX) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN10) | \
                                     PIN_PUPDR_PULLUP(GPIOD_USART3_CTS) | \
                                     PIN_PUPDR_FLOATING(GPIOD_USART3_RTS) | \
                                     PIN_PUPDR_FLOATING(GPIOD_XBEE_RESET_N) | \
                                     PIN_PUPDR_FLOATING(GPIOD_BUZZER) | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_FLASH_WP) | \
                                     PIN_ODR_HIGH(GPIOD_FLASH_HOLD) | \
                                     PIN_ODR_HIGH(GPIOD_IMU_EN_N) | \
                                     PIN_ODR_HIGH(GPIOD_PIN3) | \
                                     PIN_ODR_HIGH(GPIOD_PIN4) | \
                                     PIN_ODR_HIGH(GPIOD_UART2_TX) | \
                                     PIN_ODR_HIGH(GPIOD_UART2_RX) | \
                                     PIN_ODR_HIGH(GPIOD_PIN7) | \
                                     PIN_ODR_HIGH(GPIOD_USART3_TX) | \
                                     PIN_ODR_HIGH(GPIOD_USART3_RX) | \
                                     PIN_ODR_HIGH(GPIOD_PIN10) | \
                                     PIN_ODR_HIGH(GPIOD_USART3_CTS) | \
                                     PIN_ODR_HIGH(GPIOD_USART3_RTS) | \
                                     PIN_ODR_HIGH(GPIOD_XBEE_RESET_N) | \
                                     PIN_ODR_LOW(GPIOD_BUZZER) | \
                                     PIN_ODR_LOW(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_FLASH_WP, 0U) | \
                                     PIN_AFIO_AF(GPIOD_FLASH_HOLD, 0U) | \
                                     PIN_AFIO_AF(GPIOD_IMU_EN_N, 0U) | \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0U) | \
                                     PIN_AFIO_AF(GPIOD_PIN4, 0U) | \
                                     PIN_AFIO_AF(GPIOD_UART2_TX, 7U) | \
                                     PIN_AFIO_AF(GPIOD_UART2_RX, 7U) | \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_USART3_TX, 7U) | \
                                     PIN_AFIO_AF(GPIOD_USART3_RX, 7U) | \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0U) | \
                                     PIN_AFIO_AF(GPIOD_USART3_CTS, 7U) | \
                                     PIN_AFIO_AF(GPIOD_USART3_RTS, 7U) | \
                                     PIN_AFIO_AF(GPIOD_XBEE_RESET_N, 0U) | \
                                     PIN_AFIO_AF(GPIOD_BUZZER, 0U) | \
                                     PIN_AFIO_AF(GPIOD_PIN15, 0U))

/*
 * GPIOE setup:
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PIN0) | \
                                     PIN_MODE_INPUT(GPIOE_PIN1) | \
                                     PIN_MODE_INPUT(GPIOE_PIN2) | \
                                     PIN_MODE_INPUT(GPIOE_PIN3) | \
                                     PIN_MODE_INPUT(GPIOE_PIN4) | \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO_1) | \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO_2) | \
                                     PIN_MODE_INPUT(GPIOE_PIN7) | \
                                     PIN_MODE_INPUT(GPIOE_PIN8) | \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO_3) | \
                                     PIN_MODE_INPUT(GPIOE_PIN10) | \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO_4) | \
                                     PIN_MODE_INPUT(GPIOE_PIN12) | \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO_5) | \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO_6) | \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO_1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO_2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO_3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO_4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO_5) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO_6) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_PIN0) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN1) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN2) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN3) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN4) | \
                                     PIN_OSPEED_HIGH(GPIOE_SERVO_1) | \
                                     PIN_OSPEED_HIGH(GPIOE_SERVO_2) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN7) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN8) | \
                                     PIN_OSPEED_HIGH(GPIOE_SERVO_3) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN10) | \
                                     PIN_OSPEED_HIGH(GPIOE_SERVO_4) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN12) | \
                                     PIN_OSPEED_HIGH(GPIOE_SERVO_5) | \
                                     PIN_OSPEED_HIGH(GPIOE_SERVO_6) | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_PIN0) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN1) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN2) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN3) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN4) | \
                                     PIN_PUPDR_FLOATING(GPIOE_SERVO_1) | \
                                     PIN_PUPDR_FLOATING(GPIOE_SERVO_2) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN7) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN8) | \
                                     PIN_PUPDR_FLOATING(GPIOE_SERVO_3) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN10) | \
                                     PIN_PUPDR_FLOATING(GPIOE_SERVO_4) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN12) | \
                                     PIN_PUPDR_FLOATING(GPIOE_SERVO_5) | \
                                     PIN_PUPDR_FLOATING(GPIOE_SERVO_6) | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0) | \
                                     PIN_ODR_HIGH(GPIOE_PIN1) | \
                                     PIN_ODR_HIGH(GPIOE_PIN2) | \
                                     PIN_ODR_HIGH(GPIOE_PIN3) | \
                                     PIN_ODR_HIGH(GPIOE_PIN4) | \
                                     PIN_ODR_HIGH(GPIOE_SERVO_1) | \
                                     PIN_ODR_HIGH(GPIOE_SERVO_2) | \
                                     PIN_ODR_HIGH(GPIOE_PIN7) | \
                                     PIN_ODR_HIGH(GPIOE_PIN8) | \
                                     PIN_ODR_HIGH(GPIOE_SERVO_3) | \
                                     PIN_ODR_HIGH(GPIOE_PIN10) | \
                                     PIN_ODR_HIGH(GPIOE_SERVO_4) | \
                                     PIN_ODR_HIGH(GPIOE_PIN12) | \
                                     PIN_ODR_HIGH(GPIOE_SERVO_5) | \
                                     PIN_ODR_HIGH(GPIOE_SERVO_6) | \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0U) | \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0U) | \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0U) | \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0U) | \
                                     PIN_AFIO_AF(GPIOE_PIN4, 0U) | \
                                     PIN_AFIO_AF(GPIOE_SERVO_1, 3U) | \
                                     PIN_AFIO_AF(GPIOE_SERVO_2, 3U) | \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0U) | \
                                     PIN_AFIO_AF(GPIOE_SERVO_3, 1U) | \
                                     PIN_AFIO_AF(GPIOE_PIN10, 0U) | \
                                     PIN_AFIO_AF(GPIOE_SERVO_4, 1U) | \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0U) | \
                                     PIN_AFIO_AF(GPIOE_SERVO_5, 1U) | \
                                     PIN_AFIO_AF(GPIOE_SERVO_6, 1U) | \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0U))

/*
 * GPIOF setup:
 */
#define VAL_GPIOF_MODER             0
#define VAL_GPIOF_OTYPER            0
#define VAL_GPIOF_OSPEEDR           0
#define VAL_GPIOF_PUPDR             0
#define VAL_GPIOF_ODR               0
#define VAL_GPIOF_AFRL              0
#define VAL_GPIOF_AFRH              0

/*
 * GPIOG setup:
 */
#define VAL_GPIOG_MODER             0
#define VAL_GPIOG_OTYPER            0
#define VAL_GPIOG_OSPEEDR           0
#define VAL_GPIOG_PUPDR             0
#define VAL_GPIOG_ODR               0
#define VAL_GPIOG_AFRL              0
#define VAL_GPIOG_AFRH              0

/*
 * GPIOH setup:
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) | \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) | \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(GPIOH_OSC_IN) | \
                                     PIN_OSPEED_HIGH(GPIOH_OSC_OUT))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) | \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) | \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0U) | \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0U))
#define VAL_GPIOH_AFRH              0

/*
 * GPIOI setup:
 */
#define VAL_GPIOI_MODER             0
#define VAL_GPIOI_OTYPER            0
#define VAL_GPIOI_OSPEEDR           0
#define VAL_GPIOI_PUPDR             0
#define VAL_GPIOI_ODR               0
#define VAL_GPIOI_AFRL              0
#define VAL_GPIOI_AFRH              0


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
  void reboot_st_bootloader(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */

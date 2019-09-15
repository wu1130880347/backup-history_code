#ifndef __BOARD_PM25__
#define __BOARD_PM25__

#define GPIO_0 0
#define GPIO_1 1
#define GPIO_2 2
#define GPIO_3 3
#define GPIO_4 4
#define GPIO_5 5
#define GPIO_6 6
#define GPIO_7 7
#define GPIO_8 8
#define GPIO_9 9
#define GPIO_10 10
#define GPIO_11 11
#define GPIO_12 12
#define GPIO_13 13
#define GPIO_14 14
#define GPIO_15 15
#define GPIO_16 16

#define LEDR_IO GPIO_0
#define LEDG_IO GPIO_4
#define LEDB_IO GPIO_12
#define I2C_SCL_IO GPIO_14
#define I2C_SDA_IO GPIO_2
#define UART_TX_IO GPIO_1
#define UART_RX_IO GPIO_3
#define EN_OLED_IO GPIO_15
#define BQ_INT_IO GPIO_13
#define PM_RST_IO GPIO_16
#define PM_SET_IO GPIO_5
#define BAT_ADC_IO NULL

#define LEDR_MASK (1ULL << LEDR_IO)
#define LEDG_MASK (1ULL << LEDG_IO)
#define LEDB_MASK (1ULL << LEDB_IO)
#define I2C_SCL_MASK (1ULL << I2C_SCL_IO)
#define I2C_SDA_MASK (1ULL << I2C_SDA_IO)
#define UART_TX_MASK (1ULL << UART_TX_IO)
#define UART_RX_MASK (1ULL << UART_RX_IO)
#define EN_OLED_MASK (1ULL << EN_OLED_IO)
#define BQ_INT_MASK (1ULL << BQ_INT_IO)
#define PM_RST_MASK (1ULL << PM_RST_IO)
#define PM_SET_MASK (1ULL << PM_SET_IO)
#define BAT_ADC_MASK (1ULL << BAT_ADC_IO)

//set the output pin
#define GPIO_OUTPUT_PIN_SEL ((1ULL << LEDG_IO) | (1ULL << LEDB_IO) | (1ULL << EN_OLED_IO) | (1ULL << PM_RST_IO) | (1ULL << PM_SET_IO))
//set the input pin
#define GPIO_INPUT_PIN_SEL ((1ULL << BQ_INT_IO) | (1ULL << LEDR_IO))

//set the pin pull-up Res
#define GPIO_PULL_UP_PIN_SEL 0

//set the pin pull-down Res
#define GPIO_PULL_DOWN_PIN_SEL 0

#endif
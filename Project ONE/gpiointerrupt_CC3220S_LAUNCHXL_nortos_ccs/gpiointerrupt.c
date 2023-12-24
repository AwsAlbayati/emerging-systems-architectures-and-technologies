/*
Aws Albayati
 */

/*
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include <ti/drivers/I2C.h>

#include <ti/drivers/UART.h>

#define DISPLAY(x) UART_write(uart, &output, x);

#include <ti/drivers/Timer.h>
// Driver Handles - Global variables
Timer_Handle timer0;
volatile unsigned char TimerFlag = 0;
int16_t temperature = 25; // Initial temperature (25 degrees Celsius)
int16_t setpoint = 25;    // Initial set-point temperature (25 degrees Celsius)
int heat = 0;             // Heat off initially
uint32_t seconds = 0;     // Count of seconds since board reset



void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    TimerFlag = 1;
}
void initTimer(void)
{
    Timer_Params params;
    // Init the driver
    Timer_init();
    // Configure the driver
    Timer_Params_init(&params);
    params.period = 200000; // 200ms interval;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;
    // Open the driver
    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == NULL)
    {
        /* Failed to initialized timer */
        while (1)
        {
        }
    }
    if (Timer_start(timer0) == Timer_STATUS_ERROR)
    {
        /* Failed to start timer */
        while (1)
        {
        }
    }
}

// UART Global Variables
char output[64];
int bytesToSend;
// Driver Handles - Global variables
UART_Handle uart;
void initUART(void)
{
    UART_Params uartParams;
    // Init the driver
    UART_init();
    // Configure the driver
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;
    // Open the driver
    uart = UART_open(CONFIG_UART_0, &uartParams);
    if (uart == NULL)
    {
        /* UART_open() failed */
        while (1)
            ;
    }
}


// I2C Global Variables
static const struct
{
    uint8_t address;
    uint8_t resultReg;
    char *id;
} sensors[3] = {
    {0x48, 0x0000, "11X"},
    {0x49, 0x0000, "116"},
    {0x41, 0x0001, "006"}};
uint8_t txBuffer[1];
uint8_t rxBuffer[2];
I2C_Transaction i2cTransaction;
// Driver Handles - Global variables
I2C_Handle i2c;

// Make sure you call initUART() before calling this function.
void initI2C(void)
{
    int8_t i, found;
    I2C_Params i2cParams;
    DISPLAY(snprintf(output, 64, "Initializing I2C Driver - "))
    // Init the driver
    I2C_init();
    // Configure the driver
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    // Open the driver
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2c == NULL)
    {
        DISPLAY(snprintf(output, 64, "Failed\n\r"))
        while (1)
            ;
    }
    DISPLAY(snprintf(output, 32, "Passed\n\r"))
    // Boards were shipped with different sensors.
    // Welcome to the world of embedded systems.
    // Try to determine which sensor we have.
    // Scan through the possible sensor addresses
    /* Common I2C transaction setup */
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;
    found = false;
    for (i = 0; i < 3; ++i)
    {
        i2cTransaction.slaveAddress = sensors[i].address;
        txBuffer[0] = sensors[i].resultReg;
        DISPLAY(snprintf(output, 64, "Is this %s? ", sensors[i].id))
        if (I2C_transfer(i2c, &i2cTransaction))
        {
            DISPLAY(snprintf(output, 64, "Found\n\r"))
            found = true;
            break;
        }
        DISPLAY(snprintf(output, 64, "No\n\r"))
    }
    if (found)
    {
        DISPLAY(snprintf(output, 64, "Detected TMP%s I2C address: %x\n\r", sensors[i].id, i2cTransaction.slaveAddress))
    }
    else
    {
        DISPLAY(snprintf(output, 64, "Temperature sensor not found, contact professor\n\r"))
    }
}


int16_t readTemp(void)
{
    int j;
    int16_t temperature = 0;
    i2cTransaction.readCount = 2;
    if (I2C_transfer(i2c, &i2cTransaction))
    {
        /*
         * Extract degrees C from the received data;
         * see TMP sensor datasheet
         */
        temperature = (rxBuffer[0] << 8) | (rxBuffer[1]);
        temperature *= 0.0078125;
        /*
         * If the MSB is set '1', then we have a 2's complement
         * negative value which needs to be sign extended
         */
        if (rxBuffer[0] & 0x80)
        {
            temperature |= 0xF000;
        }
    }
    else
    {
        DISPLAY(snprintf(output, 64, "Error reading temperature sensor (%d)\n\r",i2cTransaction.status))
        DISPLAY(snprintf(output, 64, "Please power cycle your board by unplugging USB and plugging back in.\n\r"))
    }
    return temperature;
}

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    // Decrease the setpoint by 1 degree
    if (setpoint > 0)
    {
        setpoint--;
    }
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn1(uint_least8_t index)
{
    if (setpoint < 99)
    {
        setpoint++;
    }
}

void taskScheduler(void)
{
  uint32_t previousTime = 0;
  uint32_t buttonCheckTime = 0;
  uint32_t temperatureCheckTime = 0;

  while (1)
  {
      uint32_t currentTime = Timer_getCount(timer0);
      uint32_t elapsedTime = currentTime - previousTime;

      // Check buttons every 200ms
      if (elapsedTime >= 200000)
      {
          // Check and update temperature set-point based on buttons
          previousTime = currentTime;

      }

      // Check temperature every 500ms
      if ((currentTime - temperatureCheckTime) >= 500000)
      {
          temperatureCheckTime = currentTime;
          temperature = readTemp(); // Read temperature from the sensor

          // Control the heater based on temperature and set-point
          if (temperature > setpoint)
          {
              heat = 0; // Turn off the heater
              GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON); // Turn on LED
          }
          else
          {
              heat = 1; // Turn on the heater
              GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Turn off LED
          }
      }

      // Update the UART output every second
      if ((currentTime - previousTime) >= 1000000)
      {
          previousTime = currentTime;
          seconds++; // Increment the seconds count

          // Format and send data via UART
          char uartOutput[64];
          snprintf(uartOutput, sizeof(uartOutput), "<%02d,%02d,%d,%04d>\n\r", temperature, setpoint, heat, seconds);
          DISPLAY(uartOutput);
      }
  }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
       GPIO_init();
       initUART();
       initI2C();
       initTimer();

       /* Configure the LED and button pins */
       GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
       GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

       /* Turn on user LED */
       GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

       /* Install Button callback */
       GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);

       /* Enable interrupts */
       GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

       /* Configure BUTTON1 pin */
       GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

       /* Install Button callback */
       GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
       GPIO_enableInt(CONFIG_GPIO_BUTTON_1);

       // Start taskScheduler()
       taskScheduler();

    return (NULL);
}

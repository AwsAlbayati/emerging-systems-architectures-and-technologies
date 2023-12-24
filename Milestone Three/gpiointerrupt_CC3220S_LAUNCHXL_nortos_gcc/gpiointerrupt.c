/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include "ti_drivers_config.h"

#define DOT_DELAY 500000    // 500ms
#define DASH_DELAY 1500000  // 1500ms
#define CHAR_SPACE_DELAY 1500000 // 1500ms (3 * 500ms)
#define WORD_SPACE_DELAY 3500000 // 3500ms (7 * 500ms)

Timer_Handle timer0;

// Morse code definitions, Note we could add a function that converts words to Morse code notation
const char* SOS_MORSE[] = {".", ".", ".", " ", "-", "-", "-", " ", ".", ".", "."};
const char* OK_MORSE[] = {"-", "-", "-"," ",".", "-", "."};

volatile int morseIndex = 0;
int skip = 0;
volatile uint32_t delayCounter = 0;

// Messages
typedef enum {
    SOS,
    OK
} Mode;

Mode current_mode = SOS;

void wordToMorseLight(const char* morseCode)
{
    GPIO_write(CONFIG_GPIO_LED_0, 0);
    GPIO_write(CONFIG_GPIO_LED_1, 0);

    if (skip == 1){
        skip=0;
    }
    else{
        if (morseCode[morseIndex] == "."){
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON); // Turn on red LED for DOT
            delayCounter = DOT_DELAY;
            skip = 1;

        }
        else if (morseCode[morseIndex] == "-"){
            GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
            GPIO_write(CONFIG_GPIO_LED_0, 0);
            delayCounter = DASH_DELAY;
            skip = 1;
        }
        else if (morseCode[morseIndex] == " "){
            GPIO_write(CONFIG_GPIO_LED_0, 0);
            GPIO_write(CONFIG_GPIO_LED_1, 0);
            delayCounter = CHAR_SPACE_DELAY;
            skip=1;
        }

        morseIndex++;

        if (morseIndex >= (sizeof(morseCode) / sizeof(morseCode))){
            GPIO_write(CONFIG_GPIO_LED_0, 0);
            GPIO_write(CONFIG_GPIO_LED_1, 0);
            morseIndex = 0;
            delayCounter = WORD_SPACE_DELAY;
            skip=1;
        }
    }
    Timer_stop(timer0);
    Timer_setPeriod(timer0, Timer_PERIOD_US, delayCounter);
    Timer_start(timer0);
}
void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    if(current_mode == OK){
        wordToMorseLight(OK_MORSE);
    }
    else{
        wordToMorseLight(SOS_MORSE);
    }
}

void initTimer(void)
{
 Timer_Params params;
 Timer_init();
Timer_Params_init(&params);
params.period = 500000;
params.periodUnits = Timer_PERIOD_US;
params.timerMode = Timer_CONTINUOUS_CALLBACK;
params.timerCallback = timerCallback;

 timer0 = Timer_open(CONFIG_TIMER_0, &params);
 if (timer0 == NULL) {
 /* Failed to initialized timer */
 while (1) {}
 }
 if (Timer_start(timer0) == Timer_STATUS_ERROR) {
 /* Failed to start timer */
 while (1) {}
 }
}

void gpioButtonFxn0(uint_least8_t index)
{
    if (current_mode == SOS)
    {
        current_mode = OK;
    }
    else
    {
        current_mode = SOS;
    }

    // Update the timer period with the new value
    Timer_setPeriod(timer0, Timer_PERIOD_US, WORD_SPACE_DELAY);

}

void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    initTimer(); // Initialize and start the timer

    return (NULL);
}

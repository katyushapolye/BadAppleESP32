#ifndef AUDIO_H
#define AUDIO_H

#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdbool.h>
#include "freertos/FreeRTOS.h"

extern gpio_config_t IOCONFIG;

typedef enum
{
    // pin equivalents
    CHANNEL_0 = 2,
    CHANNEL_1 = 18,
    CHANNEL_2 = -2, // undecided yet

} CHANNEL;

extern bool isChannelActive[];

void INIT_AUDIO_CHANNEL(CHANNEL CHANNEL);

// Will make the thread running this busy 100% of the time, I think
void playTone(int frequency, CHANNEL channel, int msDuration);
void stopTone(void);

// use it as a task to not lock the whole program
void playMusic(int notes[], int duration[], int noteCount, CHANNEL channel);

int channelToChannelIndex(CHANNEL channel);

void playBadApple(void);
void playBadAppleBeat(void);

#endif
#include "../audio/audio.h"
#include "../utils/esp_microsleep.h"
#include "../utils/pitches.h"
#include "freertos/task.h"
#include "../graphics/graphics.h"
bool isChannelActive[] = {false, false, false};

void INIT_AUDIO_CHANNEL(CHANNEL channel)
{

    gpio_config_t pinConfig = {};
    pinConfig.intr_type = GPIO_INTR_DISABLE;
    pinConfig.mode = GPIO_MODE_OUTPUT;
    pinConfig.pin_bit_mask = (1ULL << channel);
    pinConfig.pull_down_en = 0;
    pinConfig.pull_up_en = 0;
    gpio_config(&pinConfig);
}

void playTone(int frequency, CHANNEL channel, int msDuration)
{

    if (isChannelActive[channelToChannelIndex(channel)] == true)
    {
        printf("Function refused -  Channel is busy\n");
        return;
    }
    isChannelActive[channelToChannelIndex(channel)] = true;

    if (frequency == 0)
    {
        gpio_set_level(channel, 0);
        vTaskDelay(pdMS_TO_TICKS(msDuration));
        isChannelActive[channelToChannelIndex(channel)] = false;
        return;
    }

    // period calculation

    float period = (1.0f / (float)frequency);
    int perioduSeconds = (period * 10e5) / 2;
    // esp_microsleep_calibrate();

    // setting up a periodic task based on the period with times to write high and low in the
    // io pin

    // printf("period float: %fs\n", period);
    // printf("Half period periodU: %dus\n", perioduSeconds);

    // How many loops do we need to hit the msDuration
    int MAX = (msDuration * 1000) / (perioduSeconds * 2); // converting how many cycles we have
    int c = 0;
    while (c != MAX)
    {
        gpio_set_level(channel, 1);
        esp_microsleep_delay(perioduSeconds);
        gpio_set_level(channel, 0);
        esp_microsleep_delay(perioduSeconds);
        c++;
    }
    gpio_set_level(channel, 0);
    isChannelActive[channelToChannelIndex(channel)] = false;
}

int channelToChannelIndex(CHANNEL channel)
{

    switch (channel)
    {
    case CHANNEL_0:
        return 0;
    case CHANNEL_1:
        return 1;
    case CHANNEL_2:
        return 2;
    default:
        return -1;
        break;
    }
}

void playMusic(int notes[], int duration[], int noteCount, CHANNEL channel)
{
    int c = 0;
    float speed = 1.0f;

    while (c < noteCount)
    {
        xSemaphoreTake(LCD_MUTEX, portMAX_DELAY);

        if (notes[c] == 0)
        {
            drawString(note_names[notes[c]], 0, 0);
        }
        else
        {

            drawString(note_names[notes[c] - 12], 0, 0);
        }
        xSemaphoreGive(LCD_MUTEX);
        if (c < 120)
        {
            playTone(note_frequencies[notes[c]], channel, duration[c] * 1.05);
        }
        else
        {
            playTone(note_frequencies[notes[c]], channel, duration[c] * 0.95);
        }

        xSemaphoreTake(LCD_MUTEX, portMAX_DELAY);
        drawString("    ", 0, 0);
        xSemaphoreGive(LCD_MUTEX);

        c++;
    }
}

void playBadApple(void)
{
    printf("INIT TASK - Handle...\n");
    // int task = *((int *)TASK_HANDLE);

    // printf("Task created - HANDLE %d\n", task);
    playMusic(BadApplenotes, BadAppleduration, BADAPPLESIZE, CHANNEL_0);
    vTaskDelay(99999);

    // vTaskDelete(task);
}

void playBadAppleBeat(void)
{
    printf("INIT TASK - Handle...\n");
    // int task = *((int *)TASK_HANDLE);

    // printf("Task created - HANDLE %d\n", task);
    playMusic(BadAppleBeatnotes, BadAppleBeatduration, BADAPPLEBEATSIZE, CHANNEL_1);

    // vTaskDelete(task);
}
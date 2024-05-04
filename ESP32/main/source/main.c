#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"

// My includes
#include "../utils/utils.h"
#include "../graphics/graphics.h"
#include "../audio/audio.h"
#include "../utils/definitions.h"
#include "../utils/esp_microsleep.h"
#include "../utils/pitches.h"

// #include "badapple.h"
#include "../data/data.h"

#define xBit 32
#define yBit 24

#define M (xBit / 8)
#define N yBit

char LCD_Frame_Scaled[6][64] = {{0}};
char Scale_Buffer[48][8] = {{0}};
int c = 0;
char frameNumber[] = "000";

// NOTES IN THE 7TH OCTAVE

void app_main(void)
{
    // To do
    // actually finish the drawing and buffer clearing
    //   Pad non 8x8 sprites, for example, a normal 4x8 bits sprite, need a 4 bit zero pad in the Y, also
    //   padd before scaling and all
    //   Rotate

    INIT_GRAPHICS();
    INIT_FONT();

    INIT_AUDIO_CHANNEL(CHANNEL_1);
    INIT_AUDIO_CHANNEL(CHANNEL_0);

    TaskHandle_t handle = NULL;

    xTaskCreatePinnedToCore(
        playBadApple,
        "BAD APPLE",
        2048, //
        NULL, // parameter
        1,    // priority
        NULL,
        1); // handle

    Node *root = buildHuffmanTree(huffmanCodes);
    int shift = 7;
    int deCount = 0;

    while (c != 2190)
    {

        char(*frame)[M] = decompressHuffmanFrame(root, data, data_size, &shift, &deCount);
        scaleBitmap(frame, xBit, yBit * 8, 2, Scale_Buffer);
        bitmap2LCDMapGeneric(Scale_Buffer, LCD_Frame_Scaled, xBit * 2, yBit * 2, xBit * 2, yBit * 2);
        xSemaphoreTake(LCD_MUTEX, portMAX_DELAY);
        esp_lcd_panel_draw_bitmap(LCD_PANEL_HANDLE, 32, 16, 32 + xBit * 2, (yBit * 2) + 16, LCD_Frame_Scaled);

        drawString("Frm:", 5, 0);
        itoa(c, frameNumber, 10);
        drawString(frameNumber, 9, 0);
        xSemaphoreGive(LCD_MUTEX);

        free(frame);
        vTaskDelay(pdMS_TO_TICKS(95));
        c++;
    }

    // Node *root = buildHuffmanTree(huffmanCodes);
    //  decompressHuffman(root, data, data_size);

    // esp_restart();
};

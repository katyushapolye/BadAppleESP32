#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "driver/i2c_slave.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_vendor.h"

#include "../utils/utils.h"
#include "../utils/definitions.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define WIDTH 128
#define HEIGHT 64

extern i2c_master_bus_handle_t I2C_BUS_HANDLE;
extern esp_lcd_panel_handle_t LCD_PANEL_HANDLE;

extern int FRONT_BUFFER_ADRESS;
extern int BACK_BUFFER_ADRESS;
extern char LCD_BUFFER[8][128];
extern char VIDEO_BUFFER[1024];

extern char WORK_BUFFER[1024];

extern char FONT[128][8];

extern SemaphoreHandle_t LCD_MUTEX;

// The x,y are given in bits
void bitmap2LCDMapGeneric(void *_bitmap, void *_lcdmap, int bitmapX, int bitmapY, int lcdmapX, int lcdmapY);
void bitmap2LCDMap(char bitmap[64][16], char lcdmap[8][128]);
void printMatrix(void *mat, int n, int m);
void drawLetter(char c, int x, int y);
void drawString(char *string, int x, int y);

// the x,y are given in bits, only positive, >1 factor allowed
void scaleBitmap(void *_bitmap, int x, int y, unsigned int factor, void *_sbitmap);

void INIT_GRAPHICS(void);
void INIT_FONT(void);

void CLEAR_WORK_BUFFER();
void DISPLAY();

#endif
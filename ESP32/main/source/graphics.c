#include "graphics.h"

i2c_master_bus_handle_t I2C_BUS_HANDLE = NULL;
esp_lcd_panel_handle_t LCD_PANEL_HANDLE = NULL;

SemaphoreHandle_t LCD_MUTEX = NULL;

int FRONT_BUFFER_ADRESS = 0x0;
int BACK_BUFFER_ADRESS = 0x0;

char LCD_BUFFER[8][128] = {0};
char VIDEO_BUFFER[1024] = {0};
char WORK_BUFFER[1024] = {0};

char FONT[128][8] = {0};

void printMatrix(void *mat, int n, int m)
{
    char(*_mat)[m] = mat;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            printf(" | 0x%x", ((char)_mat[i][j]));
        }
        printf("\n");
    }
}
// byte,bit,bit,byte
// frames[c], LCD_B, 4, 24, 32, 3);
void bitmap2LCDMapGeneric(void *_bitmap, void *_lcdmap, int bitmapX, int bitmapY, int lcdmapX, int lcdmapY) // bitmap[64][16] and lcd[8][128]
{
    char(*bitmap)[bitmapX / 8] = _bitmap;
    char(*lcdmap)[lcdmapX] = _lcdmap;

    // collums hor
    for (int i = 0; i < (bitmapX / 8); i++)
    { // collums
        for (int j = 0; j < (bitmapY / 8); j++)
        {
            // we need to roll the loop, since the >> negative is undefined
            lcdmap[j][i * 8 + 0] = ((bitmap[7 + j * 8][i] & (0x01 << (7))) >> 0) | ((bitmap[6 + j * 8][i] & (0x01 << (7))) >> 1) | ((bitmap[5 + j * 8][i] & (0x01 << (7))) >> 2) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (7))) >> 3) | ((bitmap[3 + j * 8][i] & (0x01 << (7))) >> 4) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (7))) >> 5) | ((bitmap[1 + j * 8][i] & (0x01 << (7))) >> 6) | (((bitmap[0 + j * 8][i] & (0x01 << (7)))) >> 7);

            lcdmap[j][i * 8 + 1] = ((bitmap[7 + j * 8][i] & (0x01 << (6))) << 1) | ((bitmap[6 + j * 8][i] & (0x01 << (6))) >> 0) | ((bitmap[5 + j * 8][i] & (0x01 << (6))) >> 1) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (6))) >> 2) | ((bitmap[3 + j * 8][i] & (0x01 << (6))) >> 3) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (6))) >> 4) | ((bitmap[1 + j * 8][i] & (0x01 << (6))) >> 5) | (((bitmap[0 + j * 8][i] & (0x01 << (6)))) >> 6);

            lcdmap[j][i * 8 + 2] = ((bitmap[7 + j * 8][i] & (0x01 << (5))) << 2) | ((bitmap[6 + j * 8][i] & (0x01 << (5))) << 1) | ((bitmap[5 + j * 8][i] & (0x01 << (5))) >> 0) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (5))) >> 1) | ((bitmap[3 + j * 8][i] & (0x01 << (5))) >> 2) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (5))) >> 3) | ((bitmap[1 + j * 8][i] & (0x01 << (5))) >> 4) | (((bitmap[0 + j * 8][i] & (0x01 << (5)))) >> 5);

            lcdmap[j][i * 8 + 3] = ((bitmap[7 + j * 8][i] & (0x01 << (4))) << 3) | ((bitmap[6 + j * 8][i] & (0x01 << (4))) << 2) | ((bitmap[5 + j * 8][i] & (0x01 << (4))) << 1) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (4))) >> 0) | ((bitmap[3 + j * 8][i] & (0x01 << (4))) >> 1) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (4))) >> 2) | ((bitmap[1 + j * 8][i] & (0x01 << (4))) >> 3) | (((bitmap[0 + j * 8][i] & (0x01 << (4)))) >> 4);

            lcdmap[j][i * 8 + 4] = ((bitmap[7 + j * 8][i] & (0x01 << (3))) << 4) | ((bitmap[6 + j * 8][i] & (0x01 << (3))) << 3) | ((bitmap[5 + j * 8][i] & (0x01 << (3))) << 2) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (3))) << 1) | ((bitmap[3 + j * 8][i] & (0x01 << (3))) >> 0) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (3))) >> 1) | ((bitmap[1 + j * 8][i] & (0x01 << (3))) >> 2) | (((bitmap[0 + j * 8][i] & (0x01 << (3)))) >> 3);

            lcdmap[j][i * 8 + 5] = ((bitmap[7 + j * 8][i] & (0x01 << (2))) << 5) | ((bitmap[6 + j * 8][i] & (0x01 << (2))) << 4) | ((bitmap[5 + j * 8][i] & (0x01 << (2))) << 3) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (2))) << 2) | ((bitmap[3 + j * 8][i] & (0x01 << (2))) << 1) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (2))) >> 0) | ((bitmap[1 + j * 8][i] & (0x01 << (2))) >> 1) | (((bitmap[0 + j * 8][i] & (0x01 << (2)))) >> 2);

            lcdmap[j][i * 8 + 6] = ((bitmap[7 + j * 8][i] & (0x01 << (1))) << 6) | ((bitmap[6 + j * 8][i] & (0x01 << (1))) << 5) | ((bitmap[5 + j * 8][i] & (0x01 << (1))) << 4) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (1))) << 3) | ((bitmap[3 + j * 8][i] & (0x01 << (1))) << 2) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (1))) << 1) | ((bitmap[1 + j * 8][i] & (0x01 << (1))) >> 0) | (((bitmap[0 + j * 8][i] & (0x01 << (1)))) >> 1);

            lcdmap[j][i * 8 + 7] = ((bitmap[7 + j * 8][i] & (0x01 << (0))) << 7) | ((bitmap[6 + j * 8][i] & (0x01 << (0))) << 6) | ((bitmap[5 + j * 8][i] & (0x01 << (0))) << 5) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (0))) << 4) | ((bitmap[3 + j * 8][i] & (0x01 << (0))) << 3) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (0))) << 2) | ((bitmap[1 + j * 8][i] & (0x01 << (0))) << 1) | (((bitmap[0 + j * 8][i] & (0x01 << (0)))) >> 0);
        }
    }
}

void bitmap2LCDMap(char bitmap[64][16], char lcdmap[8][128]) // bitmap[64][16] and lcd[8][128]
{

    // collums hor
    for (int i = 0; i < 16; i++)
    { // collums
        for (int j = 0; j < (64 / 8); j++)
        {
            // we need to roll the loop, since the >> negative is undefined
            lcdmap[j][i * 8 + 0] = ((bitmap[7 + j * 8][i] & (0x01 << (7))) >> 0) | ((bitmap[6 + j * 8][i] & (0x01 << (7))) >> 1) | ((bitmap[5 + j * 8][i] & (0x01 << (7))) >> 2) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (7))) >> 3) | ((bitmap[3 + j * 8][i] & (0x01 << (7))) >> 4) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (7))) >> 5) | ((bitmap[1 + j * 8][i] & (0x01 << (7))) >> 6) | (((bitmap[0 + j * 8][i] & (0x01 << (7)))) >> 7);

            lcdmap[j][i * 8 + 1] = ((bitmap[7 + j * 8][i] & (0x01 << (6))) << 1) | ((bitmap[6 + j * 8][i] & (0x01 << (6))) >> 0) | ((bitmap[5 + j * 8][i] & (0x01 << (6))) >> 1) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (6))) >> 2) | ((bitmap[3 + j * 8][i] & (0x01 << (6))) >> 3) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (6))) >> 4) | ((bitmap[1 + j * 8][i] & (0x01 << (6))) >> 5) | (((bitmap[0 + j * 8][i] & (0x01 << (6)))) >> 6);

            lcdmap[j][i * 8 + 2] = ((bitmap[7 + j * 8][i] & (0x01 << (5))) << 2) | ((bitmap[6 + j * 8][i] & (0x01 << (5))) << 1) | ((bitmap[5 + j * 8][i] & (0x01 << (5))) >> 0) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (5))) >> 1) | ((bitmap[3 + j * 8][i] & (0x01 << (5))) >> 2) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (5))) >> 3) | ((bitmap[1 + j * 8][i] & (0x01 << (5))) >> 4) | (((bitmap[0 + j * 8][i] & (0x01 << (5)))) >> 5);

            lcdmap[j][i * 8 + 3] = ((bitmap[7 + j * 8][i] & (0x01 << (4))) << 3) | ((bitmap[6 + j * 8][i] & (0x01 << (4))) << 2) | ((bitmap[5 + j * 8][i] & (0x01 << (4))) << 1) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (4))) >> 0) | ((bitmap[3 + j * 8][i] & (0x01 << (4))) >> 1) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (4))) >> 2) | ((bitmap[1 + j * 8][i] & (0x01 << (4))) >> 3) | (((bitmap[0 + j * 8][i] & (0x01 << (4)))) >> 4);

            lcdmap[j][i * 8 + 4] = ((bitmap[7 + j * 8][i] & (0x01 << (3))) << 4) | ((bitmap[6 + j * 8][i] & (0x01 << (3))) << 3) | ((bitmap[5 + j * 8][i] & (0x01 << (3))) << 2) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (3))) << 1) | ((bitmap[3 + j * 8][i] & (0x01 << (3))) >> 0) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (3))) >> 1) | ((bitmap[1 + j * 8][i] & (0x01 << (3))) >> 2) | (((bitmap[0 + j * 8][i] & (0x01 << (3)))) >> 3);

            lcdmap[j][i * 8 + 5] = ((bitmap[7 + j * 8][i] & (0x01 << (2))) << 5) | ((bitmap[6 + j * 8][i] & (0x01 << (2))) << 4) | ((bitmap[5 + j * 8][i] & (0x01 << (2))) << 3) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (2))) << 2) | ((bitmap[3 + j * 8][i] & (0x01 << (2))) << 1) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (2))) >> 0) | ((bitmap[1 + j * 8][i] & (0x01 << (2))) >> 1) | (((bitmap[0 + j * 8][i] & (0x01 << (2)))) >> 2);

            lcdmap[j][i * 8 + 6] = ((bitmap[7 + j * 8][i] & (0x01 << (1))) << 6) | ((bitmap[6 + j * 8][i] & (0x01 << (1))) << 5) | ((bitmap[5 + j * 8][i] & (0x01 << (1))) << 4) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (1))) << 3) | ((bitmap[3 + j * 8][i] & (0x01 << (1))) << 2) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (1))) << 1) | ((bitmap[1 + j * 8][i] & (0x01 << (1))) >> 0) | (((bitmap[0 + j * 8][i] & (0x01 << (1)))) >> 1);

            lcdmap[j][i * 8 + 7] = ((bitmap[7 + j * 8][i] & (0x01 << (0))) << 7) | ((bitmap[6 + j * 8][i] & (0x01 << (0))) << 6) | ((bitmap[5 + j * 8][i] & (0x01 << (0))) << 5) |
                                   ((bitmap[4 + j * 8][i] & (0x01 << (0))) << 4) | ((bitmap[3 + j * 8][i] & (0x01 << (0))) << 3) |
                                   ((bitmap[2 + j * 8][i] & (0x01 << (0))) << 2) | ((bitmap[1 + j * 8][i] & (0x01 << (0))) << 1) | (((bitmap[0 + j * 8][i] & (0x01 << (0)))) >> 0);
        }
    }
}

void INIT_GRAPHICS(void)
{
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = -1,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .flags.enable_internal_pullup = true,
    };

    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = LCD_ADDR,
        .scl_speed_hz = FREQ,
        .control_phase_bytes = 1, // According to SSD1306 datasheet
        .lcd_cmd_bits = 8,        // According to SSD1306 datasheet
        .lcd_param_bits = 8,      // According to SSD1306 datasheet
        .dc_bit_offset = 6,       // According to SSD1306 datasheet

    };
    i2c_new_master_bus(&bus_config, &I2C_BUS_HANDLE);

    printf("I2C BUS INITIALIZED...\n, STARTING LCD INIT...\n");

    esp_lcd_new_panel_io_i2c(I2C_BUS_HANDLE, &io_config, &io_handle);

    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
    };

    LCD_MUTEX = xSemaphoreCreateMutex();
    xSemaphoreGive(LCD_MUTEX);

    esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &LCD_PANEL_HANDLE);
    esp_lcd_panel_reset(LCD_PANEL_HANDLE);
    esp_lcd_panel_init(LCD_PANEL_HANDLE);

    esp_lcd_panel_disp_on_off(LCD_PANEL_HANDLE, true);
    esp_lcd_panel_mirror(LCD_PANEL_HANDLE, true, true);
    esp_lcd_panel_draw_bitmap(LCD_PANEL_HANDLE, 0, 0, WIDTH, HEIGHT, clear);

    printf("LCD INIT COMPLETE, MEMORY CLEARED, PASSING CONTROL TO MAIN...\n");
}

// Display what is in the back buffer by swapping it with the front one
void DISPLAY()
{
    SWAP_BUFFER(&VIDEO_BUFFER, &WORK_BUFFER, 1024);
    bitmap2LCDMapGeneric(VIDEO_BUFFER, LCD_BUFFER, 16, 64, 128, 8);
    esp_lcd_panel_draw_bitmap(LCD_PANEL_HANDLE, 0, 0, WIDTH, HEIGHT, LCD_BUFFER);
}

void CLEAR_WORK_BUFFER()
{
    memset(WORK_BUFFER, 0, 1024);
}

void INIT_FONT(void)
{

    for (int j = 0; j < 128; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            FONT[j][i] = reverse(font[j][i]);
        }
    }
}

// Draw a single letter to position (x,y) in text cordinates (16x8)
void drawLetter(char c, int x, int y)
{
    if (x > 15 || y > 8 || x < 0 || y < 0)
    {
        printf("Invalid char position...\n");
        printf("(x,y) = (%d,%d)\n", x, y);
        esp_restart();
    }

    char Sprite[1][8];
    // put the letter in the backbuffer in the correct place
    bitmap2LCDMapGeneric(FONT[(int)c], Sprite, 8, 8, 8, 8);
    esp_lcd_panel_draw_bitmap(LCD_PANEL_HANDLE, 8 * x, 8 * y, (8 * x) + 8, (8 * y) + 8, Sprite);
}

// prints a null terminated string to position, wraps around
void drawString(char *string, int x, int y)
{
    int k, j = 0;
    k = x;
    j = y;
    while (*string != '\0')
    {
        if (k > 15)
        {
            k = 0;
            j++;
        }
        drawLetter(*string, k, j);
        k++;

        string++;
    }
}

void scaleBitmap(void *_bitmap, int _x, int _y, unsigned int factor, void *_sbitmap)
{
    int x = _x / 8; // 1
    int y = _y / 8;
    char(*bitmap)[x] = _bitmap;

    int sX = x * factor;
    int sY = y * factor;

    char(*sBitmap)[sX] = _sbitmap;

    unsigned short int c0 = 0;
    char c1 = 0;
    char c2 = 0;

    // scale each collum then duplicate each line
    for (int i = 0; i < (y); i++)
    {

        for (int j = 0; j < (x); j++)
        {
            c0 = interleave(bitmap[i][j], bitmap[i][j]);
            c1 = (c0 >> 8) & 0xff;
            c2 = ((c0 & 0xffff));

            sBitmap[(i * 2)][j * 2] = c1;
            sBitmap[(i * 2)][j * 2 + 1] = c2;

            sBitmap[(i * 2) + 1][j * 2] = c1;
            sBitmap[(i * 2) + 1][j * 2 + 1] = c2;
        }
    }

    // copying the each line, not very efficient
}

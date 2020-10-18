#include "ili9806.h"
#include <string.h>
#include <stdio.h>
#include "main.h"

static void ILI9806_Reset(void) {
    HAL_GPIO_WritePin(ILI9806_RES_GPIO_Port, ILI9806_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ILI9806_RES_GPIO_Port, ILI9806_RES_Pin, GPIO_PIN_SET);
}

static void ILI9806_WriteCommand(uint8_t cmd) {
    ILI9806_WRITE_REG = cmd;
}

static void ILI9806_WriteData(uint16_t data) {
    ILI9806_WRITE_DATA = data;
}

static void ILI9806_WriteDataMultiple(uint16_t * datas, uint32_t dataNums) {
    while(dataNums--)
    {
        ILI9806_WRITE_DATA = *datas++;
    }
}

static void ILI9806_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ILI9806_WriteCommand(0x2A); // CASET
    {
        ILI9806_WriteData(x0 >> 8);
        ILI9806_WriteData(x0 & 0x00FF);
        ILI9806_WriteData(x1 >> 8);
        ILI9806_WriteData(x1 & 0x00FF);
    }
    // row address set
    ILI9806_WriteCommand(0x2B); // RASET
    {
        ILI9806_WriteData(y0 >> 8);
        ILI9806_WriteData(y0 & 0x00FF);
        ILI9806_WriteData(y1 >> 8);
        ILI9806_WriteData(y1 & 0x00FF);
    }
    // write to RAM
    ILI9806_WriteCommand(0x2C); // RAMWR
}

void ILI9806_Init(void) 
{
    ILI9806_Reset();
	HAL_Delay(20);
    ILI9806_WriteCommand(0xFF); // EXTC Command Set enable register 
    ILI9806_WriteData(0xFF); 
    ILI9806_WriteData(0x98); 
    ILI9806_WriteData(0x06); 

    ILI9806_WriteCommand(0xBA); // SPI Interface Setting 
    ILI9806_WriteData(0xE0); 

    ILI9806_WriteCommand(0xBC); // GIP 1 
    {
        uint16_t data[21] = {0x03, 0x0F, 0x63, 0x69, 0x01, 0x01, 0x1B, 0x11, 0x70, 0x73, 0xFF,
                            0xFF, 0x08, 0x09, 0x05, 0x00, 0xEE, 0xE2, 0x01, 0x00, 0xC1};
        ILI9806_WriteDataMultiple(data, 21);
    }

    ILI9806_WriteCommand(0xBD); // GIP 2 
    {
        uint16_t data[8] = {0x01, 0x23, 0x45, 0x67, 0x01, 0x23, 0x45, 0x67};
        ILI9806_WriteDataMultiple(data, 8);
    }

    ILI9806_WriteCommand(0xBE); // GIP 3 
    {
        uint16_t data[9] = {0x00, 0x22, 0x27, 0x6A, 0xBC, 0xD8, 0x92, 0x22, 0x22};
        ILI9806_WriteDataMultiple(data, 9);
    }

    ILI9806_WriteCommand(0xC7); // Vcom 
    ILI9806_WriteData(0x1E);
    
    ILI9806_WriteCommand(0xED); // EN_volt_reg 
    ILI9806_WriteData(0x7F); 
    ILI9806_WriteData(0x0F); 
    ILI9806_WriteData(0x00); 

    ILI9806_WriteCommand(0xC0); // Power Control 1
    ILI9806_WriteData(0xE3); 
    ILI9806_WriteData(0x0B); 
    ILI9806_WriteData(0x00);
    
    ILI9806_WriteCommand(0xFC);	//	LVGL Voltage setting
    ILI9806_WriteData(0x08); 

    ILI9806_WriteCommand(0xDF); // Engineering Setting 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x02); 

    ILI9806_WriteCommand(0xF3); // DVDD Voltage Setting 
    ILI9806_WriteData(0x74); 

    ILI9806_WriteCommand(0xB4); // Display Inversion Control 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x00); 

    ILI9806_WriteCommand(0xF7); // 480x854
    ILI9806_WriteData(0x81); 

    ILI9806_WriteCommand(0xB1); // Frame Rate 
    ILI9806_WriteData(0x00); 
    ILI9806_WriteData(0x10); 
    ILI9806_WriteData(0x14); 

    ILI9806_WriteCommand(0xF1); // 	Panel Timing Control 
    ILI9806_WriteData(0x29); 
    ILI9806_WriteData(0x8A); 
    ILI9806_WriteData(0x07); 

    ILI9806_WriteCommand(0xF2); //	Panel Timing Control 
    ILI9806_WriteData(0x40); 
    ILI9806_WriteData(0xD2); 
    ILI9806_WriteData(0x50); 
    ILI9806_WriteData(0x28); 

    ILI9806_WriteCommand(0xC1); // 	Power Control 2 
    ILI9806_WriteData(0x17);
    ILI9806_WriteData(0X85); 
    ILI9806_WriteData(0x85); 
    ILI9806_WriteData(0x20); 

    ILI9806_WriteCommand(0xE0);	//	Positive Gamma Control
    {
        uint16_t data[] = { 0x00, 0x0C, 0x15, 0x0D, 0x0F, 0x0C, 0x07, 0x05,
                            0x07, 0x0B, 0x10, 0x10, 0x0D, 0x17, 0x0F, 0x00};
        ILI9806_WriteDataMultiple(data, 15);                    
    }

    ILI9806_WriteCommand(0xE1);	//	Negative Gamma Control
    {
        uint16_t data[] = { 0x00, 0x0D, 0x15, 0x0E, 0x10, 0x0D, 0x08, 0x06,  
                            0x07, 0x0C, 0x11, 0x11, 0x0E, 0x17, 0x0F, 0x00};
        ILI9806_WriteDataMultiple(data, 15);
    }


    ILI9806_WriteCommand(0x35); //Tearing Effect ON 
    ILI9806_WriteData(0x00); 

    ILI9806_WriteCommand(0x36);	//	Memory Access Control
    ILI9806_WriteData(ILI9806_ROTATION); 

    ILI9806_WriteCommand(0x3A); //	Interface Pixel format
    ILI9806_WriteData(0x55);    //  0x55 -> 16bit | 0x77 -> 24bit

    ILI9806_WriteCommand(0x11); //Exit Sleep 
    HAL_Delay(50);
    ILI9806_WriteCommand(0x29); // Display On 
}


void ILI9806_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= ILI9806_WIDTH) || (y >= ILI9806_HEIGHT))
        return;
    ILI9806_SetAddressWindow(x, y, x+1, y+1);
    ILI9806_WriteData(color);
}

static void ILI9806_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
    uint32_t i, b, j;
    ILI9806_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);
    for (i = 0; i < font.height; i++)
    {
        b = font.data[(ch-32) * font.height + i];
        for (j = 0; j < font.width; j++)
        {
            if ((b << j) & 0x8000)
                ILI9806_DrawPixel(x+j, y+i, color);

            else
                ILI9806_DrawPixel(x+j, y+i, bgcolor);
        }
    }
}

void ILI9806_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    while(*str) {
        if(x + font.width >= ILI9806_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= ILI9806_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ILI9806_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
}


void ILI9806_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= ILI9806_WIDTH) || (y >= ILI9806_HEIGHT)) return;
    if((x + w - 1) >= ILI9806_WIDTH) w = ILI9806_WIDTH - x;
    if((y + h - 1) >= ILI9806_HEIGHT) h = ILI9806_HEIGHT - y;

    ILI9806_SetAddressWindow(x, y, x+w-1, y+h-1);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            ILI9806_WriteData(color);
        }
    }
}


void ILI9806_FillScreen(uint16_t color) {
    ILI9806_FillRectangle(0, 0, ILI9806_WIDTH, ILI9806_HEIGHT, color);
}

void ILI9806_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                     uint16_t color) {
    uint16_t swap;
    uint16_t steep = ABS(y1 - y0) > ABS(x1 - x0);
    if (steep) {
        swap = x0;
        x0 = y0;
        y0 = swap;

        swap = x1;
        x1 = y1;
        y1 = swap;
        //_swap_int16_t(x0, y0);
        //_swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        swap = x0;
        x0 = x1;
        x1 = swap;

        swap = y0;
        y0 = y1;
        y1 = swap;
        //_swap_int16_t(x0, x1);
        //_swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = ABS(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            ILI9806_DrawPixel(y0, x0, color);
        } else {
            ILI9806_DrawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void ILI9806_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9806_DrawPixel(x0, y0 + r, color);
    ILI9806_DrawPixel(x0, y0 - r, color);
    ILI9806_DrawPixel(x0 + r, y0, color);
    ILI9806_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9806_DrawPixel(x0 + x, y0 + y, color);
        ILI9806_DrawPixel(x0 - x, y0 + y, color);
        ILI9806_DrawPixel(x0 + x, y0 - y, color);
        ILI9806_DrawPixel(x0 - x, y0 - y, color);

        ILI9806_DrawPixel(x0 + y, y0 + x, color);
        ILI9806_DrawPixel(x0 - y, y0 + x, color);
        ILI9806_DrawPixel(x0 + y, y0 - x, color);
        ILI9806_DrawPixel(x0 - y, y0 - x, color);
    }
}

void ILI9806_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9806_DrawPixel(x0, y0 + r, color);
    ILI9806_DrawPixel(x0, y0 - r, color);
    ILI9806_DrawPixel(x0 + r, y0, color);
    ILI9806_DrawPixel(x0 - r, y0, color);
    ILI9806_DrawLine(x0 - r, y0, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9806_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        ILI9806_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

        ILI9806_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
        ILI9806_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
    }
}

void ILI9806_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color)
{
    /* Draw lines */
    ILI9806_DrawLine(x1, y1, x2, y2, color);
    ILI9806_DrawLine(x2, y2, x3, y3, color);
    ILI9806_DrawLine(x3, y3, x1, y1, color);
}

void ILI9806_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                uint16_t x3, uint16_t y3, uint16_t color)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1);
    deltay = ABS(y2 - y1);
    x = x1;
    y = y1;

    if (x2 >= x1) {
        xinc1 = 1;
        xinc2 = 1;
    }
    else {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) {
        yinc1 = 1;
        yinc2 = 1;
    }
    else {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay) {
        xinc1 = 0;
        yinc2 = 0;
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;
    }
    else {
        xinc2 = 0;
        yinc1 = 0;
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++) {
        ILI9806_DrawLine(x, y, x3, y3, color);

        num += numadd;
        if (num >= den) {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }
}

void ILI9806_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data) {
    if((x >= ILI9806_WIDTH) || (y >= ILI9806_HEIGHT)) return;
    if((x + w - 1) >= ILI9806_WIDTH) return;
    if((y + h - 1) >= ILI9806_HEIGHT) return;

    ILI9806_SetAddressWindow(x, y, x+w-1, y+h-1);
    ILI9806_WriteDataMultiple((uint16_t*)data, w*h);
}

void ILI9806_InvertColors(bool invert) {
    ILI9806_WriteCommand(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
}

void ILI9806_Test(void)
{
    ILI9806_FillScreen(ILI9806_WHITE);
    HAL_Delay(1000);
    ILI9806_WriteString(10, 10, "Fuxk you shit", Font_16x26, ILI9806_RED, ILI9806_WHITE);
    HAL_Delay(2000);
    ILI9806_FillScreen(ILI9806_RED);
    HAL_Delay(1000);
    ILI9806_FillScreen(ILI9806_BLUE);
    HAL_Delay(1000);
    ILI9806_FillScreen(ILI9806_GREEN);
    HAL_Delay(1000);
    ILI9806_FillScreen(ILI9806_YELLOW);
    HAL_Delay(1000);
    ILI9806_FillScreen(ILI9806_CYAN);
    HAL_Delay(1000);
    ILI9806_FillScreen(ILI9806_MAGENTA);
    HAL_Delay(1000);
    ILI9806_FillScreen(ILI9806_BLACK);
    HAL_Delay(2000);

    ILI9806_FillRectangle(10, 10, 30, 20, ILI9806_WHITE);
    HAL_Delay(1500);
    ILI9806_FillScreen(ILI9806_BLACK);

    ILI9806_DrawLine(10, 10, 30, 30, ILI9806_WHITE);
    HAL_Delay(1500);
    ILI9806_FillScreen(ILI9806_BLACK);

    ILI9806_DrawCircle(100, 100, 50, ILI9806_WHITE);
    HAL_Delay(1500);
    ILI9806_FillScreen(ILI9806_BLACK);

    ILI9806_DrawFilledCircle(100, 100, 50, ILI9806_WHITE);
    HAL_Delay(1500);
    ILI9806_FillScreen(ILI9806_BLACK);

    ILI9806_DrawTriangle(10, 10, 50, 50, 100, 10, ILI9806_WHITE);
    HAL_Delay(1500);
    ILI9806_FillScreen(ILI9806_BLACK);

    ILI9806_DrawFilledTriangle(10, 10, 50, 50, 100, 10, ILI9806_WHITE);
    HAL_Delay(1500);
    ILI9806_FillScreen(ILI9806_WHITE);
    HAL_Delay(1000);
}

void FPS_Test(void)
{
    ILI9806_FillScreen(ILI9806_WHITE);

    uint32_t start = HAL_GetTick();
    uint32_t end = start;
    int fps = 0;
    char message[] = "ABCDEFGHJK";
    do {
        ILI9806_WriteString(10, 10, message, Font_11x18, ILI9806_RED, ILI9806_WHITE);

        char ch = message[0];
        memmove(message, message+1, sizeof(message)-2);
        message[sizeof(message)-2] = ch;

        fps++;
        end = HAL_GetTick();
    } while ((end - start) < 5000);

    HAL_Delay(1000);
    char buff[64];
    fps = (float) fps / ((end - start) / 1000.0);
    snprintf(buff, sizeof(buff), "~%d FPS", fps);

    ILI9806_FillScreen(ILI9806_WHITE);
    ILI9806_WriteString(10, 10, buff, Font_11x18, ILI9806_RED, ILI9806_WHITE);
    HAL_Delay(3000);
}



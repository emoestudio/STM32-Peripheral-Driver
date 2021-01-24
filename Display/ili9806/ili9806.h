#ifndef __ILI9806_H__
#define __ILI9806_H__

#include "fonts.h"
#include "main.h"
#include <stdbool.h>

#define ILI9806_CMD_ADDR ( ( uint32_t ) 0x60000000 )    //  Base data

/* Change this value below if you have multiple devices on FSMC interface
 * and you're not using A16 for ILI9806, e.g:
 * 0x600000000 |  (0x1<<(16+1)) = 0x60020000;
 * And if it's A17, then 0x600000000 | (0x1<<(17+1)) = 0x60040000;
 */
#define ILI9806_DATA_ADDR ( ( uint32_t ) 0x60020000 )   //  LCD Register Select: A16

//  Basic functions to write data via 16-bit parallel interface (FSMC)
#define ILI9806_WRITE_REG ( *(__IO uint16_t *) (ILI9806_CMD_ADDR) )
#define ILI9806_WRITE_DATA ( *(__IO uint16_t *) (ILI9806_DATA_ADDR) )

/********************************************************
 * Pin Maps for LCD and STM32:
 * STM32        |       ILI9806_LCD     |       Comment
 * FSMC_Dx     -->      DBx             |   (16 lines in total)
 * FSMC_NOE    -->      LCD_RD          |   Read
 * FSMC_NWE    -->      LCD_WR          |   Write
 * FSMC_NEx    -->      LCD_CS          |   Chip select
 * FSMC_Ax     -->      LCD_RS          |   Register select (NOT Reset!)
 * User defined reset pin  --> LCD_RST  |   Reset   (See below)
 ********************************************************/

//  Reset pin is still needed for reset purpose
#define ILI9806_RES_Pin       GPIO_PIN_6
#define ILI9806_RES_GPIO_Port GPIOD

//  Orientation params
#define ILI9806_MADCTL_MY  0x80
#define ILI9806_MADCTL_MX  0x40
#define ILI9806_MADCTL_MV  0x20
#define ILI9806_MADCTL_ML  0x10
#define ILI9806_MADCTL_RGB 0x00
#define ILI9806_MADCTL_BGR 0x08
#define ILI9806_MADCTL_MH  0x04
#define ILI9806_MADCTL_SS  0x02
#define ILI9806_MADCTL_GS  0x01

/* Select a rotate direction for display */
#define ILI9806_SET_ROTATION 0

// default orientation
#if ILI9806_SET_ROTATION == 0
    #define ILI9806_WIDTH  854
    #define ILI9806_HEIGHT 480
    #define ILI9806_ROTATION (ILI9806_MADCTL_MX | ILI9806_MADCTL_MV |ILI9806_MADCTL_RGB)


// upside down
#elif ILI9806_SET_ROTATION == 1
    #define ILI9806_WIDTH  854
    #define ILI9806_HEIGHT 480
    #define ILI9806_ROTATION (ILI9806_MADCTL_MY | ILI9806_MADCTL_MV | ILI9806_MADCTL_RGB)


// rotate left
#elif ILI9806_SET_ROTATION == 2
    #define ILI9806_WIDTH  480
    #define ILI9806_HEIGHT 854
    #define ILI9806_ROTATION (ILI9806_MADCTL_RGB)

// rorate right
#elif ILI9806_SET_ROTATION == 3
    #define ILI9806_WIDTH  480
    #define ILI9806_HEIGHT 854
    #define ILI9806_ROTATION (ILI9806_MADCTL_GS |ILI9806_MADCTL_ML | ILI9806_MADCTL_MX | ILI9806_MADCTL_RGB)
#endif

/****************************/

// Color definitions
#define	ILI9806_BLACK   0x0000
#define	ILI9806_BLUE    0x001F
#define	ILI9806_RED     0xF800
#define	ILI9806_GREEN   0x07E0
#define ILI9806_CYAN    0x07FF
#define ILI9806_MAGENTA 0xF81F
#define ILI9806_YELLOW  0xFFE0
#define ILI9806_WHITE   0xFFFF
#define ILI9806_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

#define ABS(x) ((x) > 0 ? (x) : -(x))


/**
 * @brief Set the Display Address Window
 * @param x, y -> Coordinates of the 2 corner of the window
 * @return none
 */
void ILI9806_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * @brief Initialize the ili9806 controller
 * @param none
 * @return none
 */
void ILI9806_Init(void);

/**
 * @brief Draw a pixel on screen
 * @param x, y -> Coordinates to draw
 * @param color -> color to draw
 * @return none
 */
void ILI9806_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Write string to screen
 * @param x, y -> Coordinates to write
 * @param str -> string to write
 * @param font -> font to use
 * @param color -> color of the string
 * @param bgcolor -> Background color of the string
 * @return none
 */
void ILI9806_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

/**
 * @brief Fill a Rect-Area with single color
 * @param x, y -> Coordinates to start
 * @param w, h -> Width & Height of the Rect.
 * @param color -> color of the Rect.
 * @return none
 */
void ILI9806_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief Fill screen with single color
 * @param color -> color to fill with
 * @return none
 */
void ILI9806_FillScreen(uint16_t color);

/**
 * @brief Draw a line with single color
 * @param x1&y1 -> coordinate of the start point
 * @param x2&y2 -> coordinate of the end point
 * @param color -> color of the line to Draw
 * @return none
 */
void ILI9806_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                      uint16_t color);

/**
 * @brief Draw a circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle line
 * @return  none
 */
void ILI9806_DrawCircle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);

/**
 * @brief Draw a Filled circle with single color
 * @param x0&y0 -> coordinate of circle center
 * @param r -> radius of circle
 * @param color -> color of circle
 * @return  none
 */
void ILI9806_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

/**
 * @brief Draw a Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the lines
 * @return  none
 */
void ILI9806_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                            uint16_t x3, uint16_t y3, uint16_t color);

/**
 * @brief Draw a filled Triangle with single color
 * @param  xi&yi -> 3 coordinates of 3 top points.
 * @param color ->color of the triangle
 * @return  none
 */
void ILI9806_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                uint16_t x3, uint16_t y3, uint16_t color);

/**
 * @brief Draw an image on the screen
 * @param x, y -> Coordinate of the image's top-left dot (where to start)
 * @param w, h -> Width & Height of the image
 * @param data -> Must be '(uint16_t *)data' ,the image data array
 * @return none
 */
void ILI9806_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);

/**
 * @brief Invert screen color
 * @param invert -> Invert or not
 * @return none
 */
void ILI9806_InvertColors(bool invert);

/**
 * @brief Simple test function for almost all functions
 * @param none
 * @return none
 */
void ILI9806_Test(void);

/**
 * @brief Simple FPS test function (not so reliable...)
 * @param none
 * @return none
 */
void FPS_Test(void);

#endif // __ILI9806_H__
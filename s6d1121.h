#ifndef S6D1121_h
#define S6D1121_h

#include "stdbool.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <math.h>

#define swap(type, i, j) {type t = i; i = j; j = t;}
#define pgm_read_byte(ptr) ((uint8_t)*(ptr))
#define fontbyte(x) pgm_read_byte(&cfont.font[x])

#define LCD_CTRL_PORT GPIOB
#define LCD_DATA_PORT GPIOA

#define LCD_CS	GPIO_PIN_10
#define LCD_RS  GPIO_PIN_1
#define LCD_WR  GPIO_PIN_0
#define LCD_RST	GPIO_PIN_11

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

#define S6D1121_8		14
#define S6D1121_16		15

typedef struct
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
} _current_font;

uint8_t fcolorr,fcolorg,fcolorb;
uint8_t bcolorr,bcolorg,bcolorb;
uint8_t orient;
int32_t disp_x_size, disp_y_size;
uint8_t display_model, display_transfer_mode, display_serial_mode;
_current_font	cfont;

//Public

		void InitLCD(uint8_t orientation);
		void clrScr();
		void drawPixel(int x, int y);
		void drawLine(int x1, int y1, int x2, int y2);
		void fillScr(uint8_t r, uint8_t g, uint8_t b);
		void drawRect(int x1, int y1, int x2, int y2);
		void drawRoundRect(int x1, int y1, int x2, int y2);
		void fillRect(int x1, int y1, int x2, int y2);
		void fillRoundRect(int x1, int y1, int x2, int y2);
		void drawCircle(int x, int y, int radius);
		void fillCircle(int x, int y, int radius);
		void setColor(uint8_t r, uint8_t g, uint8_t b);
		void setBackColor(uint8_t r, uint8_t g, uint8_t b);
		void print(char *st, int x, int y, int deg);
		void printNumI(long num, int x, int y, int length, char filler);
		void setFont(uint8_t* font);
		void lcdOff();
		void lcdOn();
		void setContrast(char c);
		int  getDisplayXSize();
		int	 getDisplayYSize();

//Private

		void LCD_Writ_Bus(char VH,char VL);
		void LCD_Write_COM(char VL);
		void LCD_Write_DATA(char VH,char VL);
		void LCD_Write_COM_DATA(char com1,int dat1);
		void setPixel(uint8_t r,uint8_t g,uint8_t b);
		void drawHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
		void printChar(uint8_t c, int x, int y);
		void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		void clrXY();
		void rotateChar(uint8_t c, int x, int y, int pos, int deg);
		void _set_direction_registers(uint8_t mode);

#endif

#include "S6D1121.h"

static void pin_mode_output(GPIO_TypeDef *GPIOx, uint32_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void lcd_data_output(void){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL = 0x33333333;
}

void InitLCD(uint8_t orientation) {
	orient = orientation;

	pin_mode_output(LCD_CTRL_PORT, LCD_CS | LCD_RS | LCD_WR | LCD_RST);
	lcd_data_output();

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_RST, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_RST, GPIO_PIN_RESET);
	HAL_Delay(15);
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_RST, GPIO_PIN_SET);
	HAL_Delay(15);
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);

	LCD_Write_COM_DATA(0x11,0x2004);
	LCD_Write_COM_DATA(0x13,0xCC00);
	LCD_Write_COM_DATA(0x15,0x2600);
	LCD_Write_COM_DATA(0x14,0x252A);
	LCD_Write_COM_DATA(0x12,0x0033);
	LCD_Write_COM_DATA(0x13,0xCC04);
	LCD_Write_COM_DATA(0x13,0xCC06);
	LCD_Write_COM_DATA(0x13,0xCC4F);
	LCD_Write_COM_DATA(0x13,0x674F);
	LCD_Write_COM_DATA(0x11,0x2003);
	LCD_Write_COM_DATA(0x30,0x2609);
	LCD_Write_COM_DATA(0x31,0x242C);
	LCD_Write_COM_DATA(0x32,0x1F23);
	LCD_Write_COM_DATA(0x33,0x2425);
	LCD_Write_COM_DATA(0x34,0x2226);
	LCD_Write_COM_DATA(0x35,0x2523);
	LCD_Write_COM_DATA(0x36,0x1C1A);
	LCD_Write_COM_DATA(0x37,0x131D);
	LCD_Write_COM_DATA(0x38,0x0B11);
	LCD_Write_COM_DATA(0x39,0x1210);
	LCD_Write_COM_DATA(0x3A,0x1315);
	LCD_Write_COM_DATA(0x3B,0x3619);
	LCD_Write_COM_DATA(0x3C,0x0D00);
	LCD_Write_COM_DATA(0x3D,0x000D);
	LCD_Write_COM_DATA(0x16,0x0007);
	LCD_Write_COM_DATA(0x02,0x0013);
	LCD_Write_COM_DATA(0x03,0x0003);
	LCD_Write_COM_DATA(0x01,0x0127);
	LCD_Write_COM_DATA(0x08,0x0303);
	LCD_Write_COM_DATA(0x0A,0x000B);
	LCD_Write_COM_DATA(0x0B,0x0003);
	LCD_Write_COM_DATA(0x0C,0x0000);
	LCD_Write_COM_DATA(0x41,0x0000);
	LCD_Write_COM_DATA(0x50,0x0000);
	LCD_Write_COM_DATA(0x60,0x0005);
	LCD_Write_COM_DATA(0x70,0x000B);
	LCD_Write_COM_DATA(0x71,0x0000);
	LCD_Write_COM_DATA(0x78,0x0000);
	LCD_Write_COM_DATA(0x7A,0x0000);
	LCD_Write_COM_DATA(0x79,0x0007);
	LCD_Write_COM_DATA(0x07,0x0051);
	LCD_Write_COM_DATA(0x07,0x0053);
	LCD_Write_COM_DATA(0x79,0x0000);
	LCD_Write_COM(0x22);

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);

	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	cfont.font=0;
}

void LCD_Write_COM_DATA(char com1,int dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA(dat1>>8,dat1);
}

static void lcd_pulse_low(void)
{
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_WR, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_WR, GPIO_PIN_SET);
}

void LCD_Writ_Bus(char VH,char VL)
{
	GPIOA->BSRR = VH; // set true values
	GPIOA->BSRR = (~VH)<<16;
	lcd_pulse_low();
	GPIOA->BSRR = VL; // set true values
	GPIOA->BSRR = (~VL)<<16;
	lcd_pulse_low();
}

void LCD_Write_COM(char VL)
{
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_RS, GPIO_PIN_RESET);
	LCD_Writ_Bus(0x00,VL);
}

void LCD_Write_DATA(char VH,char VL)
{
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_RS, GPIO_PIN_SET);
	LCD_Writ_Bus(VH,VL);
}

void setColor(uint8_t r, uint8_t g, uint8_t b)
{
	fcolorr=r;
	fcolorg=g;
	fcolorb=b;
}

void setBackColor(uint8_t r, uint8_t g, uint8_t b)
{
	bcolorr=r;
	bcolorg=g;
	bcolorb=b;
}

void clrScr()
{
	long i;

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
	clrXY();
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_RS, GPIO_PIN_SET);

	for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
	{
		LCD_Writ_Bus(0,0);
	}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
}

void clrXY()
{
	if (orient==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	else
		setXY(0,0,disp_y_size,disp_x_size);
}

void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {

	if (orient==LANDSCAPE)
	{
		swap(uint16_t, x1, y1);
		swap(uint16_t, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap(uint16_t, y1, y2)
	}

	LCD_Write_COM_DATA(0x46,(x2 << 8) | x1);
	LCD_Write_COM_DATA(0x47,y2);
	LCD_Write_COM_DATA(0x48,y1);
	LCD_Write_COM_DATA(0x20,x1);
	LCD_Write_COM_DATA(0x21,y1);
	LCD_Write_COM(0x22);
}

void fillRect(int x1, int y1, int x2, int y2) {

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	if (orient==PORTRAIT)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			drawHLine(x1, y1+i, x2-x1);
			drawHLine(x1, y2-i, x2-x1);
		}
	}
	else
	{
		for (int i=0; i<((x2-x1)/2)+1; i++)
		{
			drawVLine(x1+i, y1, y2-y1);
			drawVLine(x2-i, y1, y2-y1);
		}
	}
}

void drawHLine(int x, int y, int l)
{
	char ch, cl;

	ch=((fcolorr&248)|fcolorg>>5);
	cl=((fcolorg&28)<<3|fcolorb>>3);

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
	setXY(x, y, x+l, y);
	for (int i=0; i<l+1; i++)
	{
		LCD_Write_DATA(ch, cl);
	}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	clrXY();
}

void drawVLine(int x, int y, int l)
{
	char ch, cl;

	ch=((fcolorr&248)|fcolorg>>5);
	cl=((fcolorg&28)<<3|fcolorb>>3);

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
	setXY(x, y, x, y+l);
	for (int i=0; i<l; i++)
	{
		LCD_Write_DATA(ch, cl);
	}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	clrXY();
}

void print(char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void printChar(uint8_t c, int x, int y)
{
	uint8_t i,ch;
	uint16_t j;
	uint16_t temp;

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);

	if (orient==PORTRAIT)
	{
		setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);

		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
		{
			ch=pgm_read_byte(&cfont.font[temp]);
			for(i=0;i<8;i++)
			{
				if((ch&(1<<(7-i)))!=0)
				{
					setPixel(fcolorr, fcolorg, fcolorb);
				}
				else
				{
					setPixel(bcolorr, bcolorg, bcolorb);
				}
			}
			temp++;
		}
	}
	else
	{
		temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

		for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
		{
			setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
			for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
			{
				ch=pgm_read_byte(&cfont.font[temp+zz]);
				for(i=0;i<8;i++)
				{
					if((ch&(1<<i))!=0)
					{
						setPixel(fcolorr, fcolorg, fcolorb);
					}
					else
					{
						setPixel(bcolorr, bcolorg, bcolorb);
					}
				}
			}
			temp+=(cfont.x_size/8);
		}
	}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	clrXY();
}

void rotateChar(uint8_t c, int x, int y, int pos, int deg)
{
	uint8_t i,j,ch;
	uint16_t temp;
	int newx,newy;
	double radian;
	radian=deg*0.0175;

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);

	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++)
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]);
			for(i=0;i<8;i++)
			{
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);

				if((ch&(1<<(7-i)))!=0)
				{
					setPixel(fcolorr, fcolorg, fcolorb);
				}
				else
				{
					setPixel(bcolorr, bcolorg, bcolorb);
				}
			}
		}
		temp+=(cfont.x_size/8);
	}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	clrXY();
}

void setPixel(uint8_t r,uint8_t g,uint8_t b)
{
	LCD_Write_DATA(((r&248)|g>>5),((g&28)<<3|b>>3));	// rrrrrggggggbbbbb
}

void drawRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void drawLine(int x1, int y1, int x2, int y2) {

	double delta, tx, ty;
	double m, b, dx, dy;
	char ch, cl;

	ch=((fcolorr&248)|fcolorg>>5);
	cl=((fcolorg&28)<<3|fcolorb>>3);

	if (((x2-x1)<0))
	{
		swap(int, x1, x2);
		swap(int, y1, y2);
	}
    if (((y2-y1)<0))
	{
		swap(int, x1, x2);
		swap(int, y1, y2);
	}

	if (y1==y2)
	{
		if (x1>x2)
		{
			swap(int, x1, x2);
		}
		drawHLine(x1, y1, x2-x1);
	}
	else if (x1==x2)
	{
		if (y1>y2)
		{
			swap(int, y1, y2);
		}
		drawVLine(x1, y1, y2-y1);
	}
	else if (abs(x2-x1)>abs(y2-y1))
	{
		HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
		delta=((double)(y2-y1)/(double)(x2-x1));
		ty=(double)(y1);
		if (x1>x2)
		{
			for (int i=x1; i>=x2; i--)
			{
				setXY(i, (int)(ty+0.5), i, (int)(ty+0.5));
				LCD_Write_DATA(ch, cl);
        		ty=ty-delta;
			}
		}
		else
		{
			for (int i=x1; i<=x2; i++)
			{
				setXY(i, (int)(ty+0.5), i, (int)(ty+0.5));
				LCD_Write_DATA(ch, cl);
        		ty=ty+delta;
			}
		}
		HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
		delta=((float)(x2-x1)/(float)(y2-y1));
		tx=(float)(x1);
        if (y1>y2)
        {
			for (int i=y2+1; i>y1; i--)
			{
		 		setXY((int)(tx+0.5), i, (int)(tx+0.5), i);
				LCD_Write_DATA(ch, cl);
        		tx=tx+delta;
			}
        }
        else
        {
			for (int i=y1; i<y2+1; i++)
			{
		 		setXY((int)(tx+0.5), i, (int)(tx+0.5), i);
				LCD_Write_DATA(ch, cl);
        		tx=tx+delta;
			}
        }
        HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	}

	clrXY();
}

void drawPixel(int x, int y)
{
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
	setXY(x, y, x, y);
	setPixel(fcolorr, fcolorg, fcolorb);
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	clrXY();
}

void fillRoundRect(int x1, int y1, int x2, int y2) {

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void fillCircle(int x, int y, int radius)
{
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
	for(int y1=-radius; y1<=radius; y1++)
		for(int x1=-radius; x1<=radius; x1++)
			if(x1*x1+y1*y1 <= radius*radius)
			{
				setXY(x+x1, y+y1, x+x1, y+y1);
				LCD_Write_DATA(((fcolorr&248)|fcolorg>>5),((fcolorg&28)<<3|fcolorb>>3));
			}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	clrXY();
}

void drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
	char ch, cl;

	ch=((fcolorr&248)|fcolorg>>5);
	cl=((fcolorg&28)<<3|fcolorb>>3);

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA(ch,cl);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA(ch,cl);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA(ch,cl);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA(ch,cl);

	while(x1 < y1)
	{
		if(f >= 0)
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA(ch,cl);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA(ch,cl);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA(ch,cl);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA(ch,cl);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA(ch,cl);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA(ch,cl);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA(ch,cl);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA(ch,cl);
	}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
	clrXY();
}

void drawRoundRect(int x1, int y1, int x2, int y2) {

	if (x1>x2)
	{
		swap(int, x1, x2);
	}
	if (y1>y2)
	{
		swap(int, y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void fillScr(uint8_t r, uint8_t g, uint8_t b)
{
	long i;
	char ch, cl;

	ch=((r&248)|g>>5);
	cl=((g&28)<<3|b>>3);

	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_RESET);
	clrXY();
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_RS, GPIO_PIN_SET);

	for (i=0; i<((disp_x_size+1)*(disp_y_size+1)); i++)
	{
		LCD_Writ_Bus(ch,cl);
	}
	HAL_GPIO_WritePin(LCD_CTRL_PORT, LCD_CS, GPIO_PIN_SET);
}

void setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

void printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	bool neg=false;
	int c=0, f=0;

	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}

		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;

		if (neg)
		{
			st[0]=45;
		}

		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y,0);
}



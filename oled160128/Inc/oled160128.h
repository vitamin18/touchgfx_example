/*
 * OLED_160128.h
 *
 *  Created on: Mar 26, 2021
 *      Author: KostyrevVS
 */

#ifndef INC_OLED160128_H_
#define INC_OLED160128_H_

#ifdef __cplusplus
  extern "C" {
#endif

	#include <stdlib.h>
	#include "main.h"
	#include "dma.h"

	#define X_SIZE					160
	#define Y_SIZE					128

	#define ADDR_FRAME_BUFF	 (uint8_t*)0x20030000
	#define ADDR_CMD        *(uint8_t*)0xC0000000
	#define ADDR_DATA       *(uint8_t*)0xC0010000

	#define BLACK 0x0000
	#define BLUE 0x001F
	#define RED 0xF800
	#define GREEN 0x07E0
	#define CYAN 0x07FF
	#define MAGENTA 0xF81F
	#define YELLOW 0xFFE0
	#define WHITE 0xFFFF

  void Oled_160128_Command(uint8_t cmd);
  void Oled_160128_Data(uint8_t data);
  void Oled_160128_Reg(uint32_t reg, uint32_t value);
  void Oled_160128_Read_Pixel(uint16_t *pixel, uint16_t xPos, uint16_t yPos);
  void OLED_Init_160128RGB(void);
  void Oled_160128_Fill_Rect(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, uint16_t color);
  void Oled_160128_Set_Window(uint16_t xPos, uint16_t yPos, uint16_t height, uint16_t width);
  void LCD_IO_WriteMultipleData(uint16_t *pData, uint32_t Size);


#ifdef __cplusplus
}
#endif


#endif /* INC_OLED160128_H_ */

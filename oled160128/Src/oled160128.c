#include "../../oled160128/Inc/oled160128.h"

#include "fmc.h"
#include "cmsis_os.h"
#include <string.h>

extern osSemaphoreId screen_frame_buffer_semHandle;

void Oled_160128_Reset(void)
{
	HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, GPIO_PIN_RESET);
	osDelay(10);
	HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, GPIO_PIN_SET);
	osDelay(10);
}

void Oled_160128_Command(uint8_t cmd)
{
	ADDR_CMD = cmd;
}

void Oled_160128_Data(uint8_t data)
{
	ADDR_DATA = data;
}

void Oled_160128_Read_Pixel(uint16_t *pixel, uint16_t xPos, uint16_t yPos)
{
	uint8_t buf [2] = {0};

	Oled_160128_Reg(0x20, xPos);
	Oled_160128_Reg(0x21, yPos);
	Oled_160128_Command(0x22);

	buf [0] = ADDR_DATA;
	buf [0] = ADDR_DATA;
	buf [1] = ADDR_DATA;

	*pixel = (uint16_t)buf[0]<<8 | buf[1];
}

void Oled_160128_Reg(uint32_t reg, uint32_t value)
{
	Oled_160128_Command(reg);
	Oled_160128_Data(value);
}

// 160*128
void Oled_160128_Set_Window(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height)
{
	//set window
	Oled_160128_Reg(0x17, xPos);
	Oled_160128_Reg(0x18, xPos + width - 1);
	Oled_160128_Reg(0x19, yPos);
	Oled_160128_Reg(0x1A, yPos + height - 1);

	//start position
	Oled_160128_Reg(0x20, xPos);
	Oled_160128_Reg(0x21, yPos);
}

void Oled_160128_Fill_Rect(uint16_t xPos, uint16_t yPos, uint16_t width, uint16_t height, uint16_t color)
{

	Oled_160128_Set_Window(xPos, yPos, width, height);

	Oled_160128_Command(0x22); //Enable write to display RAM

	for(int i = 0; i < X_SIZE*Y_SIZE*2; i+=2){
		ADDR_DATA = color>>8;
		ADDR_DATA = color;
	}

	//	HAL_DMA_Start (&hdma_memtomem_dma2_stream0, buf, &ADDR_DATA, width*height*2);
//	HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)ADDR_FRAME_BUFF, (uint32_t)&ADDR_DATA, width*height);
//	osSemaphoreWait(screen_frame_buffer_semHandle, portMAX_DELAY);
	//HAL_SRAM_Write_16b (&hsram1, &ADDR_DATA, buf, width*height);
}

void OLED_Init_160128RGB(void)
{
	Oled_160128_Reset();

		Oled_160128_Reg(0x04, 0x01); //Set REDUCE_CURRENT

		osDelay(2);

		Oled_160128_Reg(0x04, 0x00); //Set REDUCE_CURRENT

		osDelay(2);

		Oled_160128_Reg(0x05, 0x00); //Set SOFT_RST

		Oled_160128_Reg(0x06, 0x00); //Set DISP_ON_OFF

		Oled_160128_Reg(0x02, 0x01); //Set OSC_CTL

		Oled_160128_Reg(0x03, 0x30); //Set CLOCK_DIV

		Oled_160128_Reg(0x28, 0x7F); //Set DUTY

		Oled_160128_Reg(0x29, 0x00); //Set DSL

		Oled_160128_Reg(0x20, 0x00); //Set MEMORY_ACCESSPOINTER_X

		Oled_160128_Reg(0x21, 0x00); //Set MEMORY_ACCESSPOINTER_Y

		Oled_160128_Reg(0x14, 0x31); //Set RGB_IF

		Oled_160128_Reg(0x15, 0x00); //Set RGB_POL

		Oled_160128_Reg(0x13, 0x00); //Set DISPLAY_MODE_SET

		Oled_160128_Reg(0x16, 0x66); //Set MEMORY_WRITE_MODE 16 bit

		Oled_160128_Reg(0x10, 0x45); //Set DRIVING_CURRENT_R

		Oled_160128_Reg(0x11, 0x34); //Set DRIVING_CURRENT_G

		Oled_160128_Reg(0x12, 0x33); //Set DRIVING_CURRENT_B

		Oled_160128_Reg(0x08, 0x04); //Set PRECHARGE_TIME_R

		Oled_160128_Reg(0x09, 0x05); //Set PRECHARGE_TIME_G

		Oled_160128_Reg(0x0A, 0x05); //Set PRECHARGE_TIME_B

		Oled_160128_Reg(0x0B, 0x9D); //Set PRECHARGE_CURRENT_R

		Oled_160128_Reg(0x0C, 0x8C); //Set PRECHARGE_CURRENT_R

		Oled_160128_Reg(0x0D, 0x57); //Set PRECHARGE_CURRENT_R

		Oled_160128_Reg(0x80, 0x00); //Set IREF

		Oled_160128_Reg(0x17, 0x00); //Set MX1_ADDR

		Oled_160128_Reg(0x18, 0x9F); //Set MX2_ADDR

		Oled_160128_Reg(0x19, 0x00); //Set MY1_ADDR

		Oled_160128_Reg(0x1A, 0x7F); //Set MY2_ADDR

		Oled_160128_Reg(0x06, 0x01); //Set DISP_ON_OFF

		Oled_160128_Fill_Rect(0x00, 0x0, 160, 128, BLUE);

		osDelay(1000);
}

//static void ConvHL(uint16_t *dst, uint16_t *src, int32_t l)
//{
//	while (l > 0) {
//		*dst = (*src>>8) | (*src<<8);
//		src++;
//		dst++;
//		l--;
//	}
//}

void LCD_IO_WriteMultipleData(uint16_t *pData, uint32_t Size)
{
//	HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)buff, (uint32_t)&ADDR_DATA, Size);
//	osSemaphoreWait(screen_frame_buffer_semHandle, portMAX_DELAY);

	for(uint32_t i = 0; i < Size; i++){
		ADDR_DATA = pData[i]>>8;
		ADDR_DATA = pData[i];
	}

}

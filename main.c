#include "main.h"
#include "MAX7219.h"

SPI_HandleTypeDef hspi1;

/*uint8_t character[] = {
	0x00, 0x07, 0x05, 0x05, 0x05, 0x05, 0x05, 0x07, // 0
	0x00, 0x01, 0x03, 0x05, 0x01, 0x01, 0x01, 0x01, // 1
	0x00, 0x07, 0x01, 0x01, 0x07, 0x04, 0x04, 0x07, // 2
	0x00, 0x07, 0x01, 0x01, 0x07, 0x01, 0x01, 0x07,	// 3
	0x00, 0x05, 0x05, 0x05, 0x07, 0x01, 0x01, 0x01,	// 4
	0x00, 0x07, 0x04, 0x04, 0x07, 0x01, 0x01, 0x07,	// 5
	0x00, 0x07, 0x04, 0x04, 0x07, 0x05, 0x05, 0x07, // 6
	0x00, 0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // 7
	0x00, 0x07, 0x05, 0x05, 0x07, 0x05, 0x05, 0x07, // 8
	0x00, 0x07, 0x05, 0x05, 0x07, 0x01, 0x01, 0x07	// 9
};

uint8_t special_character[] = {
	0x00, 0x66, 0x99, 0x81, 0x81, 0x42, 0x24, 0x18 // heart character
};*/

int main(void)
{
    //initMAX7219(NODEC, 15, DIS7);
	initMAX7219(DECALL, 15, DIS7);
    while (1)
    {
       /*displayACharacter(0, special_character);
       // shiftingACharacter(5, character, 1);*/

		displayNumber(235, 0);
	   	HAL_Delay(2000);
		displayNumber(235.6, 0);
		HAL_Delay(2000);
		displayNumber(-235, 0);
	   	HAL_Delay(2000);
		displayNumber(-235.6, 0);
		HAL_Delay(2000);
    }
    
    return 0;
}
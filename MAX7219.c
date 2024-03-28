#include "MAX7219.h"

// Characters and number for LED matrix 8x8. Specification is 1088AS

/*
	Function: Send data to MAX7219
	Parameter:
	- address: Address register.
	- value: Sent data.
	Return: None
*/
void sendData(uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address;
	data[1] = value;
	HAL_GPIO_WritePin(CS_PORT,CS_PIN, 0);
	HAL_SPI_Transmit(&hspi1,data, sizeof(data), 100);
	HAL_GPIO_WritePin(CS_PORT,CS_PIN, 1);
}

/*
	Function: Initialize MAX7219
	Parameter:
	- decode_mode: The decode mode you want to choose, see on MAX7219.h file.
	- intensity: Intensity of LED.
	- scan_limit: the number of digits (for seven segment) or the number of row (for LED Matrix, I'm using 1088AS) you want to display.
		I highly recommend you turn all on.
	Return: None
	Intensity is from 0 - 15
*/
void initMAX7219(uint8_t decode_mode, uint8_t intensity, uint8_t scan_limit)
{
	sendData(SHUDOWN, 0x01);
	// Test LED first
	sendData(DIS_TEST, 0x01);
	HAL_Delay(1000);
	sendData(DIS_TEST, 0x00);
	#ifdef	SEGMENT
		sendData(DECMODE, decode_mode);
		sendData(INTENSITY, intensity);
		sendData(SCAN_LIM, scan_limit);
	#endif
	
	#ifdef LED_MATRIX
		sendData(DECMODE, NODEC);
		sendData(INTENSITY, intensity);
		sendData(SCAN_LIM, DIS7);
	#endif
}

#ifdef SEGMENT
#endif

#ifdef LED_MATRIX
/*
	Function: Display a character
	Parameter:
	- Character: The character you want to display.
	- array_character: Use character or special character array. See on MAX7219.h.
	Return: None
*/
void displayACharacter(uint8_t Character, uint8_t* array_character)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		sendData(i + 1, *(array_character + i + (8 * Character)));
	}
}

/*
	Function: Shift a character to the left
	Parameter:
	- Character: The character you want to display.
	- array_character: Use character or special character array. See on MAX7219.h.
	- shift: the number of column you want to shift
	Return: None
*/
void shiftingACharacter(uint8_t Character, uint8_t* array_character, uint8_t shift)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		sendData(i + 1, (*(array_character + i + (8 * Character))) << shift);
	}
}
#endif
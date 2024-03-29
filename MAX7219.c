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
	HAL_GPIO_WritePin(CS_PORT,CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1,data, sizeof(data), 100);
	HAL_GPIO_WritePin(CS_PORT,CS_PIN, GPIO_PIN_SET);
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
/*
	Function: Display a number on segment LED
	Parameter:
	- number: The number you want to display. It can be the positive or negative integer/float
	Return: None
*/
void displayNumber(float number)
{
	int16_t integer_part = (int16_t)number;
	int16_t decimal_part = (int16_t)((number - (float)integer_part) * 1000); // Maximu of figure in decimal part is three
	uint16_t temp_integer_part = integer_part;
	uint16_t temp_decimal_part = decimal_part;
	uint8_t count = 0;
	uint8_t sign;
	
	if(number < 0)
	{
		integer_part = (uint16_t)(integer_part * (-1.0));
		decimal_part = (uint16_t)(decimal_part * (-1.0));
		sign = 0;
	}
	else	
	{
		sign = 1;
	}
	
	if(decimal_part == 0)
	{
		do
		{
			count++;
			sendData(count, integer_part % 10);
			integer_part /= 10;
		}while(integer_part > 0);
		
		switch (sign)
		{
			case 0:
				count++;
				sendData(count, 0x0A);
				break;
			
			case 1:
				count++;
				sendData(count, 0x0F);
				break;
		}
		
		for(uint8_t i = count + 1; i <= 8; i++)
		{
			sendData(i, 0x0F);
		}
	}
	
	else if(decimal_part != 0)
	{
		// Display the decimal parts first
		do
		{
			count++;
			sendData(count, decimal_part % 10);
			decimal_part /= 10;
		}while(decimal_part > 0);
		
		// Display the integer part with the dot
		count++;
		sendData(count, (integer_part % 10) | 0x80);
		integer_part /= 10;
		
		// Display the remain integer parts
		do
		{
			count++;
			sendData(count, integer_part % 10);
			integer_part /= 10;
		}while(integer_part > 0);
		
		switch (sign)
		{
			case 0:
				count++;
				sendData(count, 0x0A);
				break;
			
			case 1:
				count++;
				sendData(count, 0x0F);
				break;
		}
		
		for(uint8_t i = count + 1; i <= 8; i++)
		{
			sendData(i, 0x0F);
		}
	}
}
#endif

#ifdef LED_MATRIX
/*
	Function: Display a number
	Parameter:
	- number: The number you want to display.
	- array_character: Use character or special character array. See on MAX7219.h.
	Return: None
*/
void displayANumber(uint8_t number, uint8_t* array_character)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		sendData(i + 1, *(array_character + i + (8 * number)));
	}
}

/*
	Function: Shift a number to the left
	Parameter:
	- number: The number you want to display.
	- array_character: Use character or special character array. See on MAX7219.h.
	- shift: the number of column you want to shift
	Return: None
*/
void shiftingANumber(uint8_t number, uint8_t* array_character, uint8_t shift)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		sendData(i + 1, (*(array_character + i + (8 * number))) << shift);
	}
}

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
		sendData(i + 1, *(array_character + i + (8 * (Character - 87))));
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
		sendData(i + 1, (*(array_character + i + (8 * (Character - 87)))) << shift);
	}
}

/*
	Function: Shift a character in a string to the left
	Parameter:
	- Character: The character you want to display.
	- array_character: Use character or special character array. See on MAX7219.h.
	- shift: the number of column you want to shift
	Return: None
*/
void shiftingACharacterInString(uint8_t Character, uint8_t* array_character, uint8_t shift)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		sendData(i + 1, (*(array_character + i + (8 * (Character - 55)))) << shift);
	}
}

/*
	Function: Display a string
	Parameter:
	- str: The string you want to display.
	- array_character: Use character or special character array. See on MAX7219.h.
	Return: None
*/
void displayString(uint8_t* str, uint8_t* array_character)
{
	uint8_t len = 0;
	while(*(str + len) != '\0')
	{
		shiftingACharacterInString(*(str + strlen((char*)str) - 1 - len), array_character, len * 3);
		len++;
	}
}
#endif

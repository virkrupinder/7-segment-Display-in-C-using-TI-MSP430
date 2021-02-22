#include "msp.h"
#include<stdlib.h>
#include<stdio.h>

void wait(int w);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	typedef   signed char   int8_t;
	typedef unsigned char  uint8_t;
	typedef          short  int16_t;
	typedef unsigned short uint16_t;
	typedef          int    int32_t;
	typedef unsigned int   uint32_t;

	#define     __IO    volatile             /*!< Defines 'read / write' permissions */
	#define     __I    volatile             /*!< Defines 'read / write' permissions */

	#define P1                               ((DIO_PORT_Odd_Interruptable_Type*)  (DIO_BASE + 0x0000))
	#define P2                               ((DIO_PORT_Even_Interruptable_Type*) (DIO_BASE + 0x0000))
	#define P3                               ((DIO_PORT_Odd_Interruptable_Type*)  (DIO_BASE + 0x0020))
	#define P4                               ((DIO_PORT_Even_Interruptable_Type*) (DIO_BASE + 0x0020))
	#define P5                               ((DIO_PORT_Odd_Interruptable_Type*)  (DIO_BASE + 0x0040))
	#define P6                               ((DIO_PORT_Even_Interruptable_Type*) (DIO_BASE + 0x0040))
	#define P7                               ((DIO_PORT_Odd_Interruptable_Type*)  (DIO_BASE + 0x0060))
	#define P8                               ((DIO_PORT_Even_Interruptable_Type*) (DIO_BASE + 0x0060))
	#define P9                               ((DIO_PORT_Odd_Interruptable_Type*)  (DIO_BASE + 0x0080))
	#define P10                              ((DIO_PORT_Even_Interruptable_Type*) (DIO_BASE + 0x0080))

	#define PERIPH_BASE                              ((uint32_t)0x40000000)          /*!< Peripherals start address */
	#define DIO_BASE                              (PERIPH_BASE +0x00004C00)          /*!< Base address of module DIO registers */

	typedef struct {
	  __I uint8_t IN;                                                                 /*!< Port Input */
	  uint8_t RESERVED0;
	  __IO uint8_t OUT;                                                               /*!< Port Output */
	  uint8_t RESERVED1;
	  __IO uint8_t DIR;                                                               /*!< Port Direction */
	  uint8_t RESERVED2;
	  __IO uint8_t REN;                                                               /*!< Port Resistor Enable */
	  uint8_t RESERVED3;
	  __IO uint8_t DS;                                                                /*!< Port Drive Strength */
	  uint8_t RESERVED4;
	  __IO uint8_t SEL0;                                                              /*!< Port Select 0 */
	  uint8_t RESERVED5;
	  __IO uint8_t SEL1;                                                              /*!< Port Select 1 */
	  uint8_t RESERVED6;
	  __I  uint16_t IV;                                                               /*!< Port Interrupt Vector Value */
	  uint8_t RESERVED7[6];
	  __IO uint8_t SELC;                                                              /*!< Port Complement Select */
	  uint8_t RESERVED8;
	  __IO uint8_t IES;                                                               /*!< Port Interrupt Edge Select */
	  uint8_t RESERVED9;
	  __IO uint8_t IE;                                                                /*!< Port Interrupt Enable */
	  uint8_t RESERVED10;
	  __IO uint8_t IFG;                                                               /*!< Port Interrupt Flag */
	  uint8_t RESERVED11;
	} DIO_PORT_Odd_Interruptable_Type;

	typedef struct {
	  uint8_t RESERVED0;
	  __I uint8_t IN;                                                                 /*!< Port Input */
	  uint8_t RESERVED1;
	  __IO uint8_t OUT;                                                               /*!< Port Output */
	  uint8_t RESERVED2;
	  __IO uint8_t DIR;                                                               /*!< Port Direction */
	  uint8_t RESERVED3;
	  __IO uint8_t REN;                                                               /*!< Port Resistor Enable */
	  uint8_t RESERVED4;
	  __IO uint8_t DS;                                                                /*!< Port Drive Strength */
	  uint8_t RESERVED5;
	  __IO uint8_t SEL0;                                                              /*!< Port Select 0 */
	  uint8_t RESERVED6;
	  __IO uint8_t SEL1;                                                              /*!< Port Select 1 */
	  uint8_t RESERVED7[9];
	  __IO uint8_t SELC;                                                              /*!< Port Complement Select */
	  uint8_t RESERVED8;
	  __IO uint8_t IES;                                                               /*!< Port Interrupt Edge Select */
	  uint8_t RESERVED9;
	  __IO uint8_t IE;                                                                /*!< Port Interrupt Enable */
	  uint8_t RESERVED10;
	  __IO uint8_t IFG;                                                               /*!< Port Interrupt Flag */
	  __I uint16_t IV;                                                                /*!< Port Interrupt Vector Value */
	} DIO_PORT_Even_Interruptable_Type;

	//Lookup table for each digit, with their corresponding index for the bits, that will be displayed on the 7-segment display
	int digit_array[16];
	digit_array[0]  =  0b11000000;      //0
	digit_array[1]  =  0b11111001;      //1
	digit_array[2]  =  0b10100100;      //2
	digit_array[3]  =  0b10110000;      //3
	digit_array[4]  =  0b10011001;      //4
	digit_array[5]  =  0b10010010;      //5
	digit_array[6]  =  0b10000010;      //6
	digit_array[7]  =  0b11111000;      //7
	digit_array[8]  =  0b10000000;      //8
	digit_array[9]  =  0b10010000;      //9
	digit_array[10] =  0b10001000;      //A
	digit_array[11] =  0b10000011;      //b
	digit_array[12] =  0b11000110;      //C
	digit_array[13] =  0b10100001;      //d
	digit_array[14] =  0b10000110;      //E
	digit_array[15] =  0b10001110;      //F

	while(1){

	    //Output for the Display

	    P8 ->DIR |= BIT2 | BIT3 | BIT4 | BIT5;                                  // P8 pins output for Digit D, C, C, E

	    P4 ->DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;      // P4 pins output a-g and DP


	    while(1)                                        // Infinite loop to display each digit one by one at a time
	    {
	        // Digit D
	                P8 -> OUT =0b00111000;               //Pin P8.2 turns on Digit D
	                P4 -> OUT = digit_array[0];         // Displays number on Digit D
	                wait(0);                            //Call for wait time loop
	       //Digit C
	                 P8 -> OUT = 0b00110100;              //Pin P8.3 turns on Digit C
	                 P4 -> OUT = digit_array[10];         // Displays number on Digit C
	                 wait(0);                             //Call for wait time loop

	       //Digit B
	                 P8 -> OUT = 0b00101100;               //Pin P8.4 turns on Digit B
	                 P4 -> OUT = digit_array[6];           // Displays number on Digit B
	                 wait(0);                             //Call for wait time loop
	       //Digit A
	                 P8 -> OUT = 0b00011100;                //Pin P8.5 turns on Digit A
	                 P4 -> OUT = digit_array[7];           // Displays number on Digit A
	                 wait(0);                              //Call for wait time loop

	        }
	}
}


//Function to add wait time to avoid overlapping of digits on the display
void wait(int w){
    for(w=0; w<1500; w++)
    {
        //a busy wait
    }
}

#include <stdint.h>
#include "tm4c123gh6pm.h"

// Define the LED pins on Port F
#define RED_LED   0x02  // PF1
#define BLUE_LED  0x04  // PF2
#define GREEN_LED 0x08  // PF3

// Function prototypes
void UART0_Init(void);
void GPIOF_Init(void);
char UART0_ReadChar(void);
void UART0_SendChar(char data);
void delayMs(int n);

int main(void) {
    UART0_Init();  // Initialize UART0
    GPIOF_Init();  // Initialize GPIO for LEDs

    char receivedChar;

    while (1) {
        // Read character from the UART terminal (computer)
        receivedChar = UART0_ReadChar();

        // Echo the received character back to the UART terminal
        UART0_SendChar(receivedChar);

        // Control LEDs based on the received character
        switch (receivedChar) {
            case 'R':
                GPIO_PORTF_DATA_R = RED_LED;   // Turn on RED LED (PF1)
                break;
            case 'B':
                GPIO_PORTF_DATA_R = BLUE_LED;  // Turn on BLUE LED (PF2)
                break;
            case 'G':
                GPIO_PORTF_DATA_R = GREEN_LED; // Turn on GREEN LED (PF3)
                break;
            default:
                GPIO_PORTF_DATA_R = 0x00;      // Turn off all LEDs
                break;
        }
    }
}

// Function to initialize UART0 for serial communication (Logic added by User 2)
void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;   // Enable clock to UART0
    SYSCTL_RCGCGPIO_R |= 0x01;   // Enable clock to PORTA

    UART0_CTL_R &= ~0x01;        // Disable UART0
    UART0_IBRD_R = 104;          // Integer portion of baud rate divisor (for 9600 baud, assuming 16 MHz clock)
    UART0_FBRD_R = 11;           // Fractional portion of baud rate divisor
    UART0_LCRH_R = 0x70;         // 8-bit, no parity, 1 stop bit
    UART0_CTL_R = 0x301;         // Enable UART0, TXE, RXE

    GPIO_PORTA_AFSEL_R |= 0x03;  // Enable alternate function on PA0 and PA1 (UART)
    GPIO_PORTA_PCTL_R |= 0x01 | (0x01 << 4); // Configure PA0 and PA1 for UART
    GPIO_PORTA_DEN_R |= 0x03;    // Enable digital functionality on PA0 and PA1
}

// Function to initialize GPIO Port F for the LEDs
void GPIOF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;   // Enable clock to Port F
    GPIO_PORTF_DIR_R |= 0x0E;    // Set PF1, PF2, PF3 as outputs (LEDs)
    GPIO_PORTF_DEN_R |= 0x0E;    // Enable digital function on PF1, PF2, PF3
}

// Function to read a character from UART0
char UART0_ReadChar(void) {
    while ((UART0_FR_R & 0x10) != 0); // Wait until the receive FIFO is not empty
    return (char)(UART0_DR_R & 0xFF); // Return the received character
}

// Function to send a character via UART0
void UART0_SendChar(char data) {
    while ((UART0_FR_R & 0x20) != 0); // Wait until the transmit FIFO is not full
    UART0_DR_R = data;                // Send the character
}

// Simple delay function (not necessary for UART, but useful for other timing)
void delayMs(int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 3180; j++) {
            // Do nothing
        }
    }
}

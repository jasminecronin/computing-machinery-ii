// This program drives a system of 3 LEDs and 2 push buttons.
// The system starts in state 1, where the lights light up in order 1 2 3 1 2 3...
// with about 0.5s in between. State 2 has the lights light up in order
// 3 2 1 3 2 1... with about 0.25s in between. Switch A will put the system in
// state 2 and switch B will put the system in state 1.

#include "uart.h"
#include "gpio.h"
#include "sysreg.h"
#include "irq.h"

// Function declarations
void init_17_out();
void set_17();
void clr_17();

void init_27_out();
void set_27();
void clr_27();

void init_22_out();
void set_22();
void clr_22();

void init_23_falling_edge();
void init_24_rising_edge();

unsigned int state;

void main()
{
    int delay; // Delay counter for busy loops

    uart_init();

    state = 1;
    
    // Configure the input and output pins
    init_17_out();
    init_27_out();
    init_22_out();
    init_23_falling_edge();
    init_24_rising_edge();

    // Enable IRQ exceptions
    enableIRQ();
    
    // Loop forever
    while (1) {

        // Turn on LED 1
        if (state == 1) {
            set_17();
            clr_27();
            clr_22();
            //uart_puts("LED 1 ON\n");
        
            delay = 0x002FFFFF;
    	    while (delay--) {
      		    asm volatile("nop");
    	    }
        }
        // Turn on LED 3
        else {
            clr_17();
            clr_27();
            set_22();
            //uart_puts("LED 3 ON\n");
        
            delay = 0x000FFFFF;
    	    while (delay--) {
      		    asm volatile("nop");
    	    }
        }

        // Turn on LED 2
        if (state == 1) {
            clr_17();
            set_27();
            clr_22();
            //uart_puts("LED 2 ON\n");

            delay = 0x002FFFFF;
    	    while (delay--) {
      	    	asm volatile("nop");
    	    }
        }
        // Turn on LED 2
        else {
            clr_17();
            set_27();
            clr_22();
            //uart_puts("LED 2 ON\n");
        
            delay = 0x000FFFFF;
    	    while (delay--) {
      		    asm volatile("nop");
    	    }
        }

        // Turn on LED 3
        if (state == 1) {
            clr_17();
            clr_27();
            set_22();
            //uart_puts("LED 3 ON\n");

            delay = 0x002FFFFF;
            while (delay--) {
                asm volatile("nop");
            }
        }
        // Turn on LED 1
        else {
            set_17();
            clr_27();
            clr_22();
            //uart_puts("LED 1 ON\n");
        
            delay = 0x000FFFFF;
    	    while (delay--) {
      		    asm volatile("nop");
    	    }
        }        
    }
}

// Initialize pin 17 as an output pin
void init_17_out()
{
    register unsigned int count;

    *GPFSEL1 &= ~(0x7 << 21); // Clear bits for pin 17
    
    *GPFSEL1 |= (0x1 << 21); // Set pin 17 for output

    *GPPUD = 0x0;

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = (0x1 << 23);

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = 0;
}

// Set pin 17 to 1
void set_17()
{
	  *GPSET0 = (0x1 << 17);
}

// Clear pin 17 to 0
void clr_17()
{
      *GPCLR0 = (0x1 << 17);
}

// Initialize pin 27 as an output pin
void init_27_out()
{
    register unsigned int count;

    *GPFSEL2 &= ~(0x7 << 21); // Clear bits for pin 27
    
    *GPFSEL2 |= (0x1 << 21); // Set pin 27 for output

    *GPPUD = 0x0;

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = (0x1 << 23);

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = 0;
}

// Set pin 27 to 1
void set_27()
{
	  *GPSET0 = (0x1 << 27);
}

// Clear pin 27 to 0
void clr_27()
{
      *GPCLR0 = (0x1 << 27);
}

// Initialize pin 22 to output
void init_22_out()
{
    register unsigned int count;

    *GPFSEL2 &= ~(0x7 << 6); // Clear bits for pin 22
    
    *GPFSEL2 |= (0x1 << 6); // Set pin 22 for output

    *GPPUD = 0x0;

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = (0x1 << 23);

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = 0;
}

// Set pin 22 to 1
void set_22()
{
	  *GPSET0 = (0x1 << 22);
}

// Clear pin 22 to 0
void clr_22()
{
      *GPCLR0 = (0x1 << 22);
}

// Initialize pin 23 as an input pin that triggers a falling edge interrupt
void init_23_falling_edge() 
{
    register unsigned int count;

    *GPFSEL2 &= ~(0x7 << 9); // Set pin to input

    *GPPUD = 0x0;

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = (0x1 << 23);

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = 0;
    *GPFEN0 = (0x1 << 23); // Falling edge detection
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20); // Enable interrupts
}

// Initialize pin 24 as an input pin that triggers a rising edge interrupt
void init_24_rising_edge()
{
    register unsigned int count;

    *GPFSEL2 &= ~(0x7 << 12); // Set pin to input
    *GPPUD = 0x0;

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = (0x1 << 24);

    count = 150; // Wait 150 cycles
    while (count--) {
      asm volatile("nop");
    }

    *GPPUDCLK0 = 0;
    *GPREN0 = (0x1 << 24); // Rising edge detection
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20); // Enable interrupts
}
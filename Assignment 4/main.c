#include "gpio.h"
#include "systimer.h"
#include "framebuffer.h"

void init_LATCH();
void set_LATCH();
void clear_LATCH();
void init_CLOCK();
void set_CLOCK();
void clear_CLOCK();
void init_DATA();
unsigned int get_DATA();

//TODO fix these defines
#define START = 3;
#define UP = 4;
#define DOWN = 5;
#define LEFT = 6;
#define RIGHT = 7;

#define PATH = 0;
#define WALL = 1;
#define STRT = 2;
#define END = 3;

int maze[12][16] = {
	{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
	{WALL, PATH, WALL, PATH, WALL, PATH, PATH, PATH, PATH, PATH, WALL, PATH, PATH, PATH, PATH, WALL},
	{STRT, PATH, PATH, PATH, WALL, PATH, WALL, PATH, WALL, PATH, PATH, PATH, WALL, WALL, PATH, WALL},
	{WALL, PATH, WALL, WALL, WALL, PATH, WALL, PATH, WALL, WALL, WALL, WALL, WALL, WALL, PATH, WALL},
	{WALL, PATH, PATH, WALL, PATH, PATH, WALL, PATH, PATH, PATH, PATH, PATH, PATH, WALL, PATH, WALL},
	{WALL, WALL, PATH, PATH, PATH, WALL, WALL, WALL, WALL, WALL, PATH, WALL, WALL, WALL, PATH, WALL},
	{WALL, PATH, PATH, WALL, PATH, WALL, PATH, PATH, PATH, WALL, PATH, WALL, PATH, PATH, PATH, WALL},
	{WALL, PATH, WALL, WALL, PATH, WALL, PATH, WALL, WALL, WALL, PATH, WALL, PATH, WALL, WALL, WALL},
	{WALL, PATH, PATH, WALL, PATH, WALL, PATH, WALL, PATH, WALL, PATH, WALL, PATH, WALL, PATH, END},
	{WALL, WALL, PATH, WALL, PATH, PATH, PATH, WALL, PATH, WALL, PATH, WALL, WALL, WALL, PATH, WALL},
	{WALL, PATH, PATH, WALL, PATH, WALL, PATH, PATH, PATH, WALL, PATH, PATH, PATH, PATH, PATH, WALL},
	{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}
};

bool play = false;
int position[2] = { -1, -1 }; // Player's position

void main() {

	init_LATCH(); // Set GPIO9 to LATCH output
	init_CLOCK(); // Set GPIO11 to CLOCK output
	init_DATA(); // Set GPIO10 to DATA input

	clear_LATCH();
	set_CLOCK(); 

	initializeFrameBuffer();
	displayFrameBuffer(maze, position);

	while (1) {
		// Read data from the SNES controller
		unsigned int data = get_SNES();

		if (data == START) { // pin 3
			position = { 3, 0 }; // Reset the player position
			play = true; // Play mode on

			displayFrameBuffer(maze, position);
			continue;
		}

		if (play) {
			if (data == UP) { //TODO fix this check
				// check that we can move up
				// move the player
			}
			else if (data == DOWN) { //TODO fix this check
				// check that we can move down
				// move the player
			}
			else if (data == LEFT) { //TODO fix this check
				// check that we can move left
				// move the player
			}
			else if (data == RIGHT) { // TODO fix this check
				// check that we can move right
				// move the player
			}

			displayFrameBuffer(maze, position);
		}
			
		if (maze[position[0]][position[1]] == END) {
			play = false;
		}
		

		// Delay 1/60th of a second
		microsecond_delay(16667);
	}
}

// Initializes GPIO pin 9 to be the LATCH output
void init_LATCH()
{
	register unsigned int count;

	*GPFSEL0 &= ~(0x7 << 27); // Clear bits for pin 9
	*GPFSEL0 |= (0x1 << 27); // Set pin 9 for output

	*GPPUD = 0x0; // Disable pull up/pull down

	count = 150; // Wait 150 cycles
	while (count--) {
		asm volatile("nop");
	}

	*GPPUDCLK0 = (0x1 << 9); 

	count = 150; // Wait 150 cycles
	while (count--) {
		asm volatile("nop");
	}

	*GPPUDCLK0 = 0;
}

// Sets LATCH output to a high level
void set_LATCH()
{
	*GPSET0 = (0x1 << 9);
}

// Clears LATCH output to a low level
void clear_LATCH()
{
	*GPCLR0 = (0x1 << 9);
}

// Initializes GPIO pin 11 to be the CLOCK output
void init_CLOCK()
{
	register unsigned int count;

	*GPFSEL1 &= ~(0x7 << 3); // Clear bits for pin 11
	*GPFSEL1 |= (0x1 << 3); // Set pin 11 for output

	*GPPUD = 0x0; // Disable pull up/pull down

	count = 150; // Wait 150 cycles
	while (count--) {
		asm volatile("nop");
	}

	*GPPUDCLK0 = (0x1 << 11);

	count = 150; // Wait 150 cycles
	while (count--) {
		asm volatile("nop");
	}

	*GPPUDCLK0 = 0;
}

// Set CLOCK output to a high level
void set_CLOCK()
{
	*GPSET0 = (0x1 << 11);
}

// Clears CLOCK output to a low level
void clear_CLOCK()
{
	*GPCLR0 = (0x1 << 11);
}

// Initialized GPIO pin 10 to be DATA input
void init_DATA()
{
	register unsigned int count;

	*GPFSEL1 &= ~(0x7 << 0); // Clear bits for pin 10

	*GPPUD = 0x0; // Disable pull up/pull down

	count = 150; // Wait 150 cycles
	while (count--) {
		asm volatile("nop");
	}

	*GPPUDCLK0 = (0x1 << 10);

	count = 150; // Wait 150 cycles
	while (count--) {
		asm volatile("nop");
	}

	*GPPUDCLK0 = 0;
}

// Get the current value of the DATA pin
unsigned int get_DATA()
{
	return ((*GPLEV0 >> 10) & 0x1);
}

// Get the current button configuration of the SNES controller
unsigned short get_SNES()
{
	unsigned short data = 0;
	unsigned int value;

	set_LATCH();
	microsecond_delay(12);
	clear_LATCH();

	// Output 16 clock pulses, and read 16 bits of serial data
	for (int i = 0; i < 16; i++) {
		microsecond_delay(6);
		clear_CLOCK();

		value = get_DATA();
		if (value == 0) { // 0 means button is pressed
			data |= (0x1 << i);
		}

		microsecond_delay(6);
		set_CLOCK();
	}

	return data;
}


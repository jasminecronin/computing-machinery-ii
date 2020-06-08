// Handler program to handle interrupts on pins 23 and 24
// Sets the system to state 1 when Switch B is pressed, and sets
// the system to state 2 when Switch A is pressed.

#include "uart.h"
#include "gpio.h"
#include "irq.h"
#include "sysreg.h"

extern unsigned int state; // State variable

void IRQ_handler()
{
  if (*IRQ_PENDING_2 == (0x1 << 20)) { // If there is any exception
    if (*GPEDS0 == (0x1 << 23)) { // If the exception is from pin 23
      *GPEDS0 = (0x1 << 23); // Clear the exception
      state = 2; // Switch to state 2
    }
    else if (*GPEDS0 == (0x1 << 24)) { // If the exception is from pin 24
      *GPEDS0 = (0x1 << 24); // Clear the exception
      state = 1; // Switch to state 1
    }
  }

  return;

}
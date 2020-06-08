	// Definition for the enableIRQ function
	// Function copied over from sysreg.s provided on D2L

			.text						// Put code in text section
			.balign 4					// Ensure instruction alignment

			.global enableIRQ			// Make function visible to linker
	enableIRQ:	
			msr DAIFClr, 0b0010			// Enable IRQ exceptions
			ret							// Return
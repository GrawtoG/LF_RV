#include "dshot_common.h"
#include <cstdio>  // For sprintf
#include <cstring> // For strcpy

#if DBG
// Ensure this also has C linkage if it's called from C components
extern "C" void pioToPioStr(PIO pio, char str[32]) {
	if (pio == pio0) {
		strcpy(str, "pio0");
	} else if (pio == pio1) {
		strcpy(str, "pio1");
#if NUM_PIOS > 2
	} else if (pio == pio2) {
		strcpy(str, "pio2");
#endif
	} else {
		sprintf(str, "%p (invalid PIO)", pio);
	}
}
#endif

// This tells the compiler: "I know gpio_init might already exist in the SDK"
extern "C" {
	__attribute__((weak)) void _gpio_init(uint gpio);

	__attribute__((weak)) void gpio_init(uint gpio) {
		// If the SDK has _gpio_init, call it.
		// Note: This is a common pattern in the Pico SDK to avoid
		// multiple definition errors.
		if (_gpio_init) {
			_gpio_init(gpio);
		}
	}
}
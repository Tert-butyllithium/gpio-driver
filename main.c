#include "gpio.h"
#include "types.h"
#include <stdarg.h>

enum gpio_cmd {
	GPIO_INPUT,
	GPIO_SET,
	GPIO_CLEAR,
	GPIO_TOGGLE,
};
 
u64 SUNXI_PIO_BASE_MAPPED; 
u64 SUNXI_R_PIO_BASE_MAPPED;

/**
 * @deprecated	Please use driver model instead
 * Request a GPIO. This should be called before any of the other functions
 * are used on this GPIO.
 *
 * Note: With driver model, the label is allocated so there is no need for
 * the caller to preserve it.
 *
 * @param gpio	GPIO number
 * @param label	User label for this GPIO
 * @return 0 if ok, -1 on error
 */
extern int gpio_request(unsigned gpio, const char *label);

/**
 * @deprecated	Please use driver model instead
 * Stop using the GPIO.  This function should not alter pin configuration.
 *
 * @param gpio	GPIO number
 * @return 0 if ok, -1 on error
 */
extern int gpio_free(unsigned gpio);

/**
 * @deprecated	Please use driver model instead
 * Make a GPIO an input.
 *
 * @param gpio	GPIO number
 * @return 0 if ok, -1 on error
 */
extern int gpio_direction_input(unsigned gpio);

/**
 * @deprecated	Please use driver model instead
 * Make a GPIO an output, and set its value.
 *
 * @param gpio	GPIO number
 * @param value	GPIO value (0 for low or 1 for high)
 * @return 0 if ok, -1 on error
 */
extern int gpio_direction_output(unsigned gpio, int value);

/**
 * @deprecated	Please use driver model instead
 * Get a GPIO's value. This will work whether the GPIO is an input
 * or an output.
 *
 * @param gpio	GPIO number
 * @return 0 if low, 1 if high, -1 on error
 */
extern int gpio_get_value(unsigned gpio);

/**
 * @deprecated	Please use driver model instead
 * Set an output GPIO's value. The GPIO must already be an output or
 * this function may have no effect.
 *
 * @param gpio	GPIO number
 * @param value	GPIO value (0 for low or 1 for high)
 * @return 0 if ok, -1 on error
 */
extern int gpio_set_value(unsigned gpio, int value);
#define unlikely(x)	__builtin_expect(!!(x), 0)
#define IS_ERR_VALUE(x) unlikely((x) >= (unsigned long)-4095)

extern int fast_sprintf(char *dst_buf, char *format, ...);

void (* printd_ptr)(const char* s, ...);
void print_console(char* fmt, ...){
	va_list args;
	char tmp[100];
	va_start(args, fmt);
	fast_sprintf(tmp, fmt, args);
	va_end(args);

	(*printd_ptr)("%s", tmp);
	
}

int do_gpio(enum gpio_cmd cmd, int gpio){
    int value;
    if ( cmd == GPIO_INPUT) {
		gpio_direction_input(gpio);
		value = gpio_get_value(gpio);
	} else {
		switch (cmd) {
		case GPIO_SET:
			value = 1;
			break;
		case GPIO_CLEAR:
			value = 0;
			break;
		case GPIO_TOGGLE:
			value = gpio_get_value(gpio);
			if (!IS_ERR_VALUE(value))
				value = !value;
			break;
		default:
			break;
		}
		gpio_direction_output(gpio, value);
	}
	print_console("gpio: (gpio %i) value is ", gpio);
    if (IS_ERR_VALUE(value))
		print_console("unknown (ret=%d)\n", value);
	else
		print_console("%d\n", value);
}

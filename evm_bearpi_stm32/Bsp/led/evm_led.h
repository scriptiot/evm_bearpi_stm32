#ifndef __EVM_LED_H__
#define __EVM_LED_H__
#include <stdint.h>

char led_init(uint8_t id);
char led_intensity(uint8_t id,uint8_t value);
char led_on(uint8_t id);
char led_off(uint8_t id);
char led_toggle(uint8_t id);


#endif


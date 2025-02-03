#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#define BUTTON_PIN 5
#define DEBOUNCING_TIME_US 200000

#define LED_PIN_1 11
#define LED_PIN_2 12
#define LED_PIN_3 13
#define INTERVAL_MS 3000

static volatile uint8_t counter = 0;
static volatile bool is_alarm_enabled = false;

static void handle_button_press(uint gpio, uint32_t events);
static bool update_semaphore(struct repeating_timer *_);

int64_t callback(alarm_id_t _id, void *_) {
	gpio_put(LED_PIN_1, counter <= 0);
	gpio_put(LED_PIN_2, counter <= 1);
	gpio_put(LED_PIN_3, counter <= 2);

	is_alarm_enabled = (counter < 3);

	counter++;
	return is_alarm_enabled ? (INTERVAL_MS * 1000) : 0;
}

int main() {
    stdio_init_all();

	gpio_init(LED_PIN_1);
	gpio_set_dir(LED_PIN_1, GPIO_OUT);

	gpio_init(LED_PIN_2);
	gpio_set_dir(LED_PIN_2, GPIO_OUT);

	gpio_init(LED_PIN_3);
	gpio_set_dir(LED_PIN_3, GPIO_OUT);

	gpio_put(LED_PIN_1, 0);
	gpio_put(LED_PIN_2, 0);
	gpio_put(LED_PIN_3, 0);

	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_up(BUTTON_PIN);
	gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &handle_button_press);

	while (true)
		sleep_ms(10000);
}

static void handle_button_press(uint gpio, uint32_t events) {
	static volatile uint32_t last_time = 0;

	if (gpio != BUTTON_PIN) return;
    uint32_t current_time = to_us_since_boot(get_absolute_time());

	if (current_time - last_time > DEBOUNCING_TIME_US) {
		if (!gpio_get(BUTTON_PIN) && !is_alarm_enabled) {
			counter = 0;
			is_alarm_enabled = true;
			add_alarm_in_ms(1, callback, NULL, false);
		}
		last_time = current_time;
	}
}
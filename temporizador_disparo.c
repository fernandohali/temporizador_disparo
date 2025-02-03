#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"

#define BUTTON_PIN 5
#define DEBOUNCING_TIME_US 200000

#define LED_PIN_1 11
#define LED_PIN_2 12
#define LED_PIN_3 13
#define INTERVAL_MS 3000

static volatile uint8_t led_counter = 0;
static volatile bool alarm_active = false;

static void button_press_handler(uint gpio, uint32_t events);
int64_t led_control_callback(alarm_id_t _id, void *_);

int main()
{
	stdio_init_all();

	// Configuração dos LEDs como saída
	gpio_init(LED_PIN_1);
	gpio_set_dir(LED_PIN_1, GPIO_OUT);

	gpio_init(LED_PIN_2);
	gpio_set_dir(LED_PIN_2, GPIO_OUT);

	gpio_init(LED_PIN_3);
	gpio_set_dir(LED_PIN_3, GPIO_OUT);

	gpio_put(LED_PIN_1, 0);
	gpio_put(LED_PIN_2, 0);
	gpio_put(LED_PIN_3, 0);

	// Configuração do botão como entrada com pull-up ativado
	gpio_init(BUTTON_PIN);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_up(BUTTON_PIN);
	gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_press_handler);

	// Loop principal
	while (true)
		sleep_ms(10000);
}

// Função para lidar com o pressionamento do botão
static void button_press_handler(uint gpio, uint32_t events)
{
	static volatile uint32_t last_time = 0;

	if (gpio != BUTTON_PIN)
		return;
	uint32_t current_time = to_us_since_boot(get_absolute_time());

	// Verifica se o botão foi pressionado após o tempo de debounce
	if (current_time - last_time > DEBOUNCING_TIME_US)
	{
		if (!gpio_get(BUTTON_PIN) && !alarm_active)
		{
			led_counter = 0;
			alarm_active = true;
			add_alarm_in_ms(1, led_control_callback, NULL, false);
		}
		last_time = current_time;
	}
}

// Callback para controlar os LEDs em intervalos definidos
int64_t led_control_callback(alarm_id_t _id, void *_)
{
	gpio_put(LED_PIN_1, led_counter <= 0);
	gpio_put(LED_PIN_2, led_counter <= 1);
	gpio_put(LED_PIN_3, led_counter <= 2);

	alarm_active = (led_counter < 3);

	led_counter++;
	return alarm_active ? (INTERVAL_MS * 1000) : 0;
}

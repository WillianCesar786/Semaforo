#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Definição dos pinos do LED RGB
#define RED_PIN    11
#define YELLOW_PIN 12
#define GREEN_PIN  13

// Estados do semáforo
typedef enum {
    RED,
    YELLOW,
    GREEN
} TrafficLightState;

volatile TrafficLightState currentState = RED; // Estado inicial

// Função de callback do temporizador
bool timer_callback(repeating_timer_t *rt) {
    // Desliga todos os LEDs
    gpio_put(RED_PIN, 0);
    gpio_put(YELLOW_PIN, 0);
    gpio_put(GREEN_PIN, 0);

    // Atualiza o estado do semáforo
    switch(currentState) {
        case RED:
            currentState = YELLOW;
            gpio_put(YELLOW_PIN, 1);
            break;
            
        case YELLOW:
            currentState = GREEN;
            gpio_put(GREEN_PIN, 1);
            break;
            
        case GREEN:
            currentState = RED;
            gpio_put(RED_PIN, 1);
            break;
    }
    return true;
}

int main() {
    stdio_init_all();

    // Configuração dos GPIOs
    gpio_init(RED_PIN);
    gpio_init(YELLOW_PIN);
    gpio_init(GREEN_PIN);
    
    gpio_set_dir(RED_PIN, GPIO_OUT);
    gpio_set_dir(YELLOW_PIN, GPIO_OUT);
    gpio_set_dir(GREEN_PIN, GPIO_OUT);

    // Inicia com LED vermelho ligado
    gpio_put(RED_PIN, 1);

    // Configura o temporizador repetitivo
    repeating_timer_t timer;
    add_repeating_timer_ms(3000, timer_callback, NULL, &timer);

    // Loop principal
    while(1) {
        printf("Estado do semáforo: %d\n", currentState);
        sleep_ms(1000);
    }

    return 0;
}
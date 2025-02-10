#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define PWM_FREQ 50          // Frequência do PWM: 50Hz (Período de 20ms)
#define CLOCK_DIV 50         // Divisor de clock
#define WRAP_VALUE 49999     // WRAP para 50Hz (125000000 / (50 * 50) - 1)

// Configuração do PWM para a GPIO do servo
void setup_pwm(uint slice_num) {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM); 
    pwm_set_clkdiv(slice_num, CLOCK_DIV);        
    pwm_set_wrap(slice_num, WRAP_VALUE);         
    pwm_set_enabled(slice_num, true);            
}

// Define o ângulo do servo com base no tempo de pulso em microssegundos
void set_servo_angle(uint slice_num, uint32_t pulse_width_us) {
    uint32_t level = (pulse_width_us * WRAP_VALUE) / 20000; // Conversão para duty cycle
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);       // Define o duty cycle do canal A
    sleep_ms(20); // Espera para garantir que o servo tenha tempo para reagir
}

int main() {
    stdio_init_all();

    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    setup_pwm(slice_num);

    // 1) Defina o ciclo ativo para 2400µs (180°)
    printf("Posição de 180° (2400µs)\n");
    set_servo_angle(slice_num, 2400);
    sleep_ms(5000);

    // 2) Defina o ciclo ativo para 1470µs (90°)
    printf("Posição de 90° (1470µs)\n");
    set_servo_angle(slice_num, 1470);
    sleep_ms(5000);

    // 3) Defina o ciclo ativo para 500µs (0°)
    printf("Posição de 0° (500µs)\n");
    set_servo_angle(slice_num, 500);
    sleep_ms(5000);

    // 4) Movimentação suave entre 0° e 180°
    printf("Iniciando movimentação suave entre 0° e 180°\n");
    while (1) {
        // Movimento de 0° a 180° (500µs a 2400µs)
        for (uint32_t pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_angle(slice_num, pulse);
            sleep_ms(10); // Atraso de 10ms para movimentação suave
        }
        sleep_ms(1000); // Aguarda 1 segundo na posição de 180°

        // Movimento de 180° a 0° (2400µs a 500µs)
        for (uint32_t pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_angle(slice_num, pulse);
            sleep_ms(10); // Atraso de 10ms para movimentação suave
        }
        sleep_ms(1000); // Aguarda 1 segundo na posição de 0°
    }

    return 0;
}





#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"

#define SERVO_PIN 22
#define LED 12
#define PWM_FREQ 50 

void posicao(uint slice_num, uint channel, float pulse_us) {
    uint16_t level = (pulse_us / 20000.0) * 24999;
    if (level > 24999) {
        level = 24999;
    }
    pwm_set_chan_level(slice_num, channel, level);
}

int main() {
    stdio_init_all();

    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    // slice e canal PWM
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    
    // Configuração do PWM 
    pwm_set_clkdiv(slice_num, 64.0f);   
    pwm_set_wrap(slice_num, 24999);     
    pwm_set_enabled(slice_num, true);
    

    posicao(slice_num, channel, 2400); // 180 graus
    printf("180 graus\n");
    sleep_ms(5000);
    posicao(slice_num, channel, 1470); // 90 graus
    printf("90 graus\n");
    sleep_ms(5000);
    posicao(slice_num, channel, 500);  // 0 graus
    printf("0 graus\n");
    sleep_ms(5000);

    while (true) {

        // De 0 para 180 graus
        for (float pulse = 500; pulse <= 2400; pulse += 5) {
            posicao(slice_num, channel, pulse);
            printf("posicao = %.0f us\n", pulse);
            gpio_put(LED, !gpio_get(LED));
            sleep_ms(10);
        }
        gpio_put(LED, 0);
        sleep_ms(1000);
        
        // De 180 para 0 graus
        for (float pulse = 2400; pulse >= 500; pulse -= 5) {
            posicao(slice_num, channel, pulse);
            printf("posicao = %.0f us\n", pulse);
            gpio_put(LED, !gpio_get(LED));
            sleep_ms(10);
        }
        gpio_put(LED, 0);
        sleep_ms(1000);
    }
}

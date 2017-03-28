#include "STM32Sleep.h"
#include <libmaple/pwr.h>
#include <libmaple/scb.h>

static void noop() {};


void sleepAndWakeUp(SleepMode mode, RTClock *rt, uint8_t seconds) {
  rt->createAlarm(&noop, rt->getTime() + seconds);
  goToSleep(mode);
}

void goToSleep(SleepMode mode) {
    // Clear PDDS and LPDS bits
    PWR_BASE->CR &= ~PWR_CR_PDDS;
    PWR_BASE->CR &= ~PWR_CR_LPDS;

    // Clear previous wakeup register
    PWR_BASE->CR |= PWR_CR_CWUF;

    if (mode == STANDBY) {
      PWR_BASE->CR |= PWR_CR_PDDS;
    }

    PWR_BASE->CR |= PWR_CR_LPDS;

    SCB_BASE->SCR |= SCB_SCR_SLEEPDEEP;

    // Now go into stop mode, wake up on interrupt
    asm("    wfi");  
}

void disableAllPeripheralClocks() {
    // Disable all peripheral clocks
    rcc_clk_disable(RCC_ADC1);
    rcc_clk_disable(RCC_ADC2);
    rcc_clk_disable(RCC_ADC3);
    rcc_clk_disable(RCC_AFIO);
    rcc_clk_disable(RCC_BKP);
    rcc_clk_disable(RCC_CRC);
    rcc_clk_disable(RCC_DAC);
    rcc_clk_disable(RCC_DMA1);
    rcc_clk_disable(RCC_DMA2);
    rcc_clk_disable(RCC_FLITF);
    rcc_clk_disable(RCC_FSMC);
    rcc_clk_disable(RCC_GPIOA);
    rcc_clk_disable(RCC_GPIOB);
    rcc_clk_disable(RCC_GPIOC);
    rcc_clk_disable(RCC_GPIOD);
    rcc_clk_disable(RCC_GPIOE);
    rcc_clk_disable(RCC_GPIOF);
    rcc_clk_disable(RCC_GPIOG);
    rcc_clk_disable(RCC_I2C1);
    rcc_clk_disable(RCC_I2C2);
    rcc_clk_disable(RCC_PWR);
    rcc_clk_disable(RCC_SDIO);
    rcc_clk_disable(RCC_SPI1);
    rcc_clk_disable(RCC_SPI2);
    rcc_clk_disable(RCC_SPI3);
    rcc_clk_disable(RCC_SRAM);
    rcc_clk_disable(RCC_TIMER1);
    rcc_clk_disable(RCC_TIMER2);
    rcc_clk_disable(RCC_TIMER3);
    rcc_clk_disable(RCC_TIMER4);
    rcc_clk_disable(RCC_TIMER5);
    rcc_clk_disable(RCC_TIMER6);
    rcc_clk_disable(RCC_TIMER7);
    rcc_clk_disable(RCC_TIMER8);
    rcc_clk_disable(RCC_TIMER9);
    rcc_clk_disable(RCC_TIMER10);
    rcc_clk_disable(RCC_TIMER11);
    rcc_clk_disable(RCC_TIMER12);
    rcc_clk_disable(RCC_TIMER13);
    rcc_clk_disable(RCC_TIMER14);
    rcc_clk_disable(RCC_USART1);
    rcc_clk_disable(RCC_USART2);
    rcc_clk_disable(RCC_USART3);
    rcc_clk_disable(RCC_UART4);
    rcc_clk_disable(RCC_UART5);
    rcc_clk_disable(RCC_USB);
}

void setGPIOModeToAllPins(gpio_pin_mode mode) {
  for(uint8_t i = 0; i < 16; i++) {
    gpio_set_mode(GPIOA, i, mode);
    gpio_set_mode(GPIOB, i, mode);
    gpio_set_mode(GPIOC, i, mode);
  }
}

void switchToPLLwithHSE(rcc_pll_multiplier pllMultiplier) {
  rcc_turn_on_clk(RCC_CLK_HSI);
  while (!rcc_is_clk_ready(RCC_CLK_HSI))
      ;
  rcc_switch_sysclk(RCC_CLKSRC_HSI);
  rcc_turn_off_clk(RCC_CLK_HSE);
  rcc_turn_off_clk(RCC_CLK_PLL);
  rcc_clk_init(RCC_CLKSRC_PLL, RCC_PLLSRC_HSE, RCC_PLLMUL_2);
}

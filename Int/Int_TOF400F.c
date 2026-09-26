#include "Int_TOF400F.h"
#include "usart.h"
#include <stdio.h>

/* ================================================================
 *  TOF400F — VL53L1X, UART3 115200-8N1, 7-byte auto-output
 *
 *  Frame: 01 03 02 DistH DistL B5 B6
 * ================================================================ */

/* ---- Ring buffer ---- */
#define RING_SIZE 256
static volatile uint8_t  ring[RING_SIZE];
static volatile uint8_t  wr;
static volatile uint16_t total;
static          uint8_t  rd;
static uint16_t distance_mm;
static uint8_t  distance_valid;

/* ---- Called from raw USART3 ISR (stm32f1xx_it.c) ---- */
void Int_TOF400F_IRQ_Byte(uint8_t b)
{
    uint8_t next = (wr + 1) % RING_SIZE;
    if (next == rd) rd = (rd + 1) % RING_SIZE;
    ring[wr] = b;
    wr = next;
    total++;
}

void Int_TOF400F_Init(void)
{
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    distance_mm = 0; distance_valid = 0;
    wr = 0; rd = 0; total = 0;

    /* Enable RX */
    SET_BIT(USART3->CR1, USART_CR1_RXNEIE);
    USART3->CR1 |= USART_CR1_RE;

}

void Int_TOF400F_PrintDistance(void)
{
    static uint32_t last_tick = 0;
    static uint8_t  state = 0, fbuf[7], fidx = 0;
    uint32_t now = HAL_GetTick();

    /* Drain ring into parser */
    for (;;) {
        uint8_t w, avail, r;
        __disable_irq(); w = wr; r = rd; __enable_irq();
        if (w >= r) avail = w - r;
        else        avail = RING_SIZE - r + w;
        if (avail == 0) break;

        uint8_t b = ring[r];
        rd = (r + 1) % RING_SIZE;

        if (state == 0) {
            if (b == 0x01) { fbuf[0] = b; fidx = 1; state = 1; }
        } else {
            fbuf[fidx++] = b;
            if (fidx >= 7) {
                state = 0;
                /* fbuf: [01] [B1] [B2] [DistH] [DistL] [B5] [B6] */
                if (fbuf[3] != 0xFF) {
                    distance_mm = ((uint16_t)fbuf[3] << 8) | fbuf[4];
                    distance_valid = 1;
                }
            }
        }
    }

    /* Frame timeout — discard partial frame after 10ms */
    {
        static uint32_t ts = 0;
        static uint8_t  ps = 0;
        if (state == 1) {
            if (ps == 0) ts = now;
            else if (now - ts > 10) state = 0;
        }
        ps = state;
    }

    /* Print every 200ms — distance only */
    if (now - last_tick >= 200) {
        last_tick = now;
        if (distance_valid) {
            printf("%d\r\n", distance_mm);
            distance_valid = 0;
        }
    }
}

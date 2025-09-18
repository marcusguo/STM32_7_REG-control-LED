#include "stm32f10x.h"

void delay_ms(volatile unsigned int ms)
{
    for (; ms > 0; ms--) {
        for (volatile unsigned int i = 0; i < 7200; i++) {
            __asm__("nop");
        }
    }
}

int main(void)
{
//	*(unsigned int*)0x40021018 |= (1<<3);
//	*(unsigned int*)0x40010C00 |= (1<<0);
//	*(unsigned int*)0x40010C0C &= ~(1<<0); 
	
	int flag = 0;  
	
	if (flag ==1){
		
		while(1){
			// 1. 開啟 GPIOB 時鐘
			*(unsigned int*)0x40021018 |= (1<<3);

			// 2. 設定 PB0、PB1、PB5 為輸出模式（一般推挽，max 2MHz）
			// CRL 控制 PB0~PB7（每個 pin 占 4 bit）
			*(unsigned int*)0x40010C00 &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)));  // 清除 PB0, PB1
			*(unsigned int*)0x40010C00 |=  ((0x2 << (0 * 4)) | (0x2 << (1 * 4)));  // 設為輸出模式2MHz，推挽

			// CRH 控制 PB8~PB15，所以 PB5 也在 CRL（5 < 8）
			*(unsigned int*)0x40010C00 &= ~(0xF << (5 * 4));  // 清除 PB5
			*(unsigned int*)0x40010C00 |=  (0x2 << (5 * 4));  // 設為輸出模式2MHz，推挽

			// 3. 控制輸出（低電位點亮）
			*(unsigned int*)0x40010C0C |=  (1 << 0);   // PB0 = 1 (關綠燈)
			*(unsigned int*)0x40010C0C |=  (1 << 1);   // PB1 = 1 (關藍燈)
			*(unsigned int*)0x40010C0C &= ~(1 << 5);   // PB5 = 0 (亮紅燈)
			delay_ms(100);
			*(unsigned int*)0x40010C0C |=  (1 << 0);   // PB0 = 1 (關綠燈)
			*(unsigned int*)0x40010C0C &= ~(1 << 1);   // PB1 = 0 (亮藍燈)
			*(unsigned int*)0x40010C0C |=  (1 << 5);   // PB5 = 1 (關紅燈)
			delay_ms(100);
			*(unsigned int*)0x40010C0C &= ~(1 << 0);   // PB0 = 0 (亮綠燈)
			*(unsigned int*)0x40010C0C |=  (1 << 1);   // PB1 = 1 (關藍燈)
			*(unsigned int*)0x40010C0C |=  (1 << 5);   // PB5 = 1 (關紅燈)
			delay_ms(100);
			*(unsigned int*)0x40010C0C &= ~(1 << 0);   // PB0 = 0 (亮綠燈)
			*(unsigned int*)0x40010C0C &= ~(1 << 1);   // PB1 = 0 (亮藍燈)
			*(unsigned int*)0x40010C0C &= ~(1 << 5);   // PB5 = 0 (亮紅燈)
			delay_ms(100);
			
		}	//end-while
	}	//end-if
}

void SystemInit(void)
{
	//compiler cheating
}

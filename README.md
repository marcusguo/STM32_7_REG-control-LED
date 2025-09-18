# STM32_7_REG-control-LED
By using REG setting to control GPIO LED

# 1. include "startup_stm32f10x_hd.s"
While we create a new project without using STM32 Standard Peripheral Library, we should include startup file "startup_stm32f10x_hd.s".

D:\BaiduNetdiskDownload\野火【STM32F103开发板-指南者】资料\3-STM32官方资料_参考资料\标准库固件包\STM32F103官方固件库与手册（标准库）\STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm\startup_stm32f10x_hd.s
<img height="200" alt="image" src="https://github.com/user-attachments/assets/b9aa8f10-0ff4-406b-b098-cdb3688e548a" />

# 2. include "stm32f10x.h" and write void SystemInit(void) at main()
There is need write SystemInit to cheat startup sequence. And add "stm32f10x.h" with empty file
<img height="180" alt="image" src="https://github.com/user-attachments/assets/3510a843-889b-46d7-af55-8288fe0cb724" />

# 3. Turn On LED 
LED positive t0 3.3V, negative to GND and cascade a current-limiting resistor.

<img width="300" alt="image" src="https://github.com/user-attachments/assets/9b94a068-de41-4c81-b11d-f1001728102f" />

# 4.GPIO sinking current (pulling low) is usually used to turn on an LED:
GPIO sink (pulling low) capability is usually stronger than source (pulling high).In many MCUs like STM32, GPIO sink current > source current.For example, some pins on STM32F103:

***Source (pull high): max 6–8 mA***

***Sink (pull low): max 20–25 mA***

➤ LEDs usually need 5–15 mA. Letting GPIO source current may cause voltage drop or instability.

# 5. Turn on GPIOB clock
```c
// 1. 開啟 GPIOB 時鐘
*(unsigned int*)0x40021018 |= (1<<3);
```
PB means GPIOB, when we want to anything about GPIO, turn on the clock first.We config RCC AHB to turn on GPIOB's clock.
<img width="300" alt="image" src="https://github.com/user-attachments/assets/4453ebe7-d354-47f4-a2f5-542e19d79049" />
<img height="200" alt="image" src="https://github.com/user-attachments/assets/510953cb-6cb2-49c9-9ea8-fa2b19c76756" />


<img height="300" alt="image" src="https://github.com/user-attachments/assets/512092ba-bc8c-4b27-af5e-be51616e8ff6" />

# 6. Config. GPIO
Clear bit first, then set GPIO settings.

```c
// 2. 設定 PB0、PB1、PB5 為輸出模式（一般推挽，max 2MHz）
// CRL 控制 PB0~PB7（每個 pin 占 4 bit）
*(unsigned int*)0x40010C00 &= ~((0xF << (0 * 4)) | (0xF << (1 * 4)));  // 清除 PB0, PB1
*(unsigned int*)0x40010C00 |=  ((0x2 << (0 * 4)) | (0x2 << (1 * 4)));  // 設為輸出模式2MHz，推挽

// CRH 控制 PB8~PB15，所以 PB5 也在 CRL（5 < 8）
*(unsigned int*)0x40010C00 &= ~(0xF << (5 * 4));  // 清除 PB5
*(unsigned int*)0x40010C00 |=  (0x2 << (5 * 4));  // 設為輸出模式2MHz，推挽
```
<img width="400" alt="image" src="https://github.com/user-attachments/assets/588d80f0-9608-4872-8d59-1ea27c253838" />

<img  height="250" alt="image" src="https://github.com/user-attachments/assets/cb699e4a-319e-4342-83d0-314b118d2dcf" />


# 7. GPIO pull low
If you want to turn RBG color LED, pull low GPIO which you color you wanna turn on and pull others LED to high

```c
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
```
<img height="300" alt="image" src="https://github.com/user-attachments/assets/2de82072-f08f-4222-a478-11077480ac24" />





















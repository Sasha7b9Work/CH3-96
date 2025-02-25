// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Settings.h"
#include <stm32f4xx_hal.h>
#include <cstdlib>


#define PIN_BL_E  GPIO_PIN_0
#define PORT_BL_E GPIOB
#define BL_E      PORT_BL_E, PIN_BL_E

#define PIN_D_C  GPIO_PIN_1
#define PORT_D_C GPIOB
#define D_C      PORT_D_C, PIN_D_C

#define PIN_WR  GPIO_PIN_2
#define PORT_WR GPIOB
#define WR      PORT_WR, PIN_WR

#define PIN_RD  GPIO_PIN_10
#define PORT_RD GPIOB
#define RD      PORT_RD, PIN_RD

#define PIN_CS  GPIO_PIN_13
#define PORT_CS GPIOC
#define CS      PORT_CS, PIN_CS

#define PIN_DIP_ON  GPIO_PIN_15
#define PORT_DIP_ON GPIOC
#define DIP_ON      PORT_DIP_ON, PIN_DIP_ON

#define PIN_RESET   GPIO_PIN_14
#define PORT_RESET  GPIOC
#define RESET       PORT_RESET, PIN_RESET

#define PIN_D0  GPIO_PIN_0
#define PORT_D0 GPIOA
#define D0      PORT_D0, PIN_D0

#define PIN_D1  GPIO_PIN_1
#define PORT_D1 GPIOA
#define D1      PORT_D1, PIN_D1

#define PIN_D2  GPIO_PIN_2
#define PORT_D2 GPIOA
#define D2      PORT_D2, PIN_D2

#define PIN_D3  GPIO_PIN_3
#define PORT_D3 GPIOA
#define D3      PORT_D3, PIN_D3

#define PIN_D4  GPIO_PIN_4
#define PORT_D4 GPIOA
#define D4      PORT_D4, PIN_D4

#define PIN_D5  GPIO_PIN_5
#define PORT_D5 GPIOA
#define D5      PORT_D5, PIN_D5

#define PIN_D6  GPIO_PIN_6
#define PORT_D6 GPIOA
#define D6      PORT_D6, PIN_D6

#define PIN_D7  GPIO_PIN_7
#define PORT_D7 GPIOA
#define D7      PORT_D7, PIN_D7


#define PIN_D8  GPIO_PIN_0
#define PORT_D8 GPIOC
#define D8      PORT_D8, PIN_D8

#define PIN_D9  GPIO_PIN_1
#define PORT_D9 GPIOC
#define D9      PORT_D9, PIN_D9

#define PIN_D10  GPIO_PIN_2
#define PORT_D10 GPIOC
#define D10      PORT_D10, PIN_D10

#define PIN_D11  GPIO_PIN_3
#define PORT_D11 GPIOC
#define D11      PORT_D11, PIN_D11

#define PIN_D12  GPIO_PIN_4
#define PORT_D12 GPIOC
#define D12      PORT_D12, PIN_D12

#define PIN_D13  GPIO_PIN_5
#define PORT_D13 GPIOC
#define D13      PORT_D13, PIN_D13

#define PIN_D14  GPIO_PIN_6
#define PORT_D14 GPIOC
#define D14      PORT_D14, PIN_D14

#define PIN_D15  GPIO_PIN_7
#define PORT_D15 GPIOC
#define D15      PORT_D15, PIN_D15


struct StructPIO
{
    StructPIO(GPIO_TypeDef *g, uint16 p) : gpio(g), pin(p) { }

    void Set() const
    {
        HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
    }

    void Reset() const
    {
        HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
    }

    void Write(int state) const
    {
        HAL_GPIO_WritePin(gpio, pin, (GPIO_PinState)state);
    }

    int Read() const
    {
        return HAL_GPIO_ReadPin(gpio, pin);
    }

private:
    GPIO_TypeDef *gpio;
    uint16 pin;
};


StructPIO pinBL_E(BL_E);
StructPIO pinD_C(D_C);
StructPIO pinWR(WR);
StructPIO pinRD(RD);
StructPIO pinCS(CS);
StructPIO pinDIP_ON(DIP_ON);
StructPIO pinRESET(RESET);

StructPIO pinD0(D0);
StructPIO pinD1(D1);
StructPIO pinD2(D2);
StructPIO pinD3(D3);
StructPIO pinD4(D4);
StructPIO pinD5(D5);
StructPIO pinD6(D6);
StructPIO pinD7(D7);

StructPIO pinD8(D8);
StructPIO pinD9(D9);
StructPIO pinD10(D10);
StructPIO pinD11(D11);
StructPIO pinD12(D12);
StructPIO pinD13(D13);
StructPIO pinD14(D14);
StructPIO pinD15(D15);


struct DataBus
{
public:
    static void Init();
    // ���������� ������ �� ����
    static void Set(uint16 data);
    // ������ ������ � ����
    static uint16 Read();

    static void InitWrite();

public:

    static void InitRead();

    static bool forWrite;
};


bool DataBus::forWrite = true;


void HAL_FSMC::Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    DataBus::Init();

    GPIO_InitTypeDef is =
    {
        0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };


    is.Pin = PIN_CS | PIN_DIP_ON | PIN_RESET;
    HAL_GPIO_Init(GPIOC, &is);

    is.Pin = PIN_BL_E  | PIN_D_C | PIN_WR | PIN_RD;
    HAL_GPIO_Init(GPIOB, &is);

    pinRESET.Set();
    pinCS.Set();
    pinRD.Set();
    pinWR.Set();
    pinDIP_ON.Set();
    pinBL_E.Set();
}


void HAL_FSMC::Reset()
{
    pinRESET.Set();
    HAL_Delay(500);
    pinRESET.Reset();
    HAL_Delay(500);
    pinRESET.Set();
    HAL_Delay(500);
}


void HAL_FSMC::WriteCommand(uint16 command)
{
    pinCS.Reset();
    pinD_C.Reset();
    pinWR.Reset();

    DataBus::Set(command);

    pinWR.Set();
    pinD_C.Set();
    pinCS.Set();
}


void HAL_FSMC::WriteCommand(uint16 command, uint data)
{
    WriteCommand(command);
    WriteData(data);
}


void HAL_FSMC::WriteCommand(uint16 command, uint data1, uint data2, uint data3)
{
    WriteCommand(command);
    WriteData(data1);
    WriteData(data2);
    WriteData(data3);
}


void HAL_FSMC::WriteData(uint data)
{
    //pinCS.Reset();
    PORT_CS->BSRR = PIN_CS << 16;

    //pinD_C.Set();
    PORT_D_C->BSRR = PIN_D_C;

    //pinWR.Reset();
    PORT_WR->BSRR = PIN_WR << 16;

    DataBus::Set((uint16)data);

    //pinWR.Set();
    PORT_WR->BSRR = PIN_WR;

    //pinCS.Set();
    PORT_CS->BSRR = PIN_CS;
}


#define SEND_PIXEL(x)                                           \



static void WindowSet(int s_x, int e_x, int s_y, int e_y)
{
    HAL_FSMC::WriteCommand(0x2a);               //SET page address

    HAL_FSMC::WriteData((uint)((s_x) >> 8));    //SET start page address=0
    HAL_FSMC::WriteData((uint)s_x);
    HAL_FSMC::WriteData((uint)(e_x) >> 8);      //SET end page address
    HAL_FSMC::WriteData((uint)e_x);

    HAL_FSMC::WriteCommand(0x2b);               //SET column address

    HAL_FSMC::WriteData((uint)(s_y) >> 8);      //SET start column address=0
    HAL_FSMC::WriteData((uint)s_y);
    HAL_FSMC::WriteData((uint)(e_y) >> 8);      //SET end column address
    HAL_FSMC::WriteData((uint)e_y);
}


void HAL_FSMC::SendBuffer(uint8 *buffer, int x, int y, int width, int height, int k)
{
    int top = x;
    int right = x + width - 1;

    int left = y;
    int bottom = y + height - 1;

    WindowSet(top, right, left, bottom);

    HAL_FSMC::WriteCommand(0x2c);   // Write memory start

    DataBus::InitWrite();

    PORT_CS->BSRR = PIN_CS << 16;

    PORT_D_C->BSRR = PIN_D_C;

    uint *colors = ColorScheme::Current().colors;

    for(int i = 0; i < width * height / 2 / k; i++)
    {
        uint color1 = colors[*buffer++];
        uint color2 = colors[*buffer++];

        PORT_WR->BSRR = PIN_WR << 16;
        GPIOC->ODR = (GPIOC->ODR & 0xff00) + (color1 & 0xFF);       // r1
        color1 >>= 8;
        GPIOA->ODR = (GPIOA->ODR & 0xff00) + (color1 & 0xFF);       // g1
        PORT_WR->BSRR = PIN_WR;

        __asm { nop }                                                       // \warning NOP �������� ��� ��������

        PORT_WR->BSRR = PIN_WR << 16;
        GPIOA->ODR = (GPIOA->ODR & 0xff00) + (color2 & 0xFF);       // r2
        GPIOC->ODR = (GPIOC->ODR & 0xff00) + (color1 >> 8);         // b1
        PORT_WR->BSRR = PIN_WR;

        color2 >>= 8;                                                       // ����� �������� ��� ���� - ������ ��� ����� �������� ������

        PORT_WR->BSRR = PIN_WR << 16;
        GPIOC->ODR = (GPIOC->ODR & 0xff00) + (color2 & 0xFF);       // g2
        GPIOA->ODR = (GPIOA->ODR & 0xff00) + (color2 >> 8);         // b2
        PORT_WR->BSRR = PIN_WR;
    }

    PORT_CS->BSRR = PIN_CS;
}


uint16 HAL_FSMC::ReadData()
{
    pinCS.Reset();
    pinD_C.Set();
    pinRD.Reset();

    uint16 result = DataBus::Read();

    pinRD.Set();
    pinCS.Set();

    return result;
}


uint16 HAL_FSMC::GetData(uint16 address)
{
    WriteCommand(address);
    return ReadData();
}


void DataBus::Init()
{
    InitWrite();
}


void DataBus::Set(uint16 data)
{
    if(!forWrite)
    {
        InitWrite();
    }

    GPIOA->ODR = (GPIOA->ODR & 0xff00) + (uint8)data;
    GPIOC->ODR = (GPIOC->ODR & 0xff00) + (uint8)(data >> 8);
}


uint16 DataBus::Read()
{
    if(forWrite)
    {
        InitRead();
    }

    static const StructPIO pins[16] =
    {
        pinD0, pinD1, pinD2, pinD3, pinD4, pinD5, pinD6, pinD7,
        pinD8, pinD9, pinD10, pinD11, pinD12, pinD13, pinD14, pinD15
    };

    uint16 result = 0;

    for(int i = 0; i < 16; i++)
    {
        uint16 bit = (uint16)((pins[i].Read() & 0x01) << i);

        result |= bit;
    }

    return result;
}



void DataBus::InitWrite()
{
    GPIO_InitTypeDef is =
    {
        0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };

    is.Pin = PIN_D0 | PIN_D1 | PIN_D2 | PIN_D3 | PIN_D4 | PIN_D5 | PIN_D6 | PIN_D7;
    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = PIN_D8 | PIN_D9 | PIN_D10 | PIN_D11 | PIN_D12 | PIN_D13 | PIN_D14 | PIN_D15;
    HAL_GPIO_Init(GPIOC, &is);

    forWrite = true;
};


void DataBus::InitRead()
{
    GPIO_InitTypeDef is =
    {
        0,
        GPIO_MODE_INPUT,
        GPIO_PULLDOWN
    };

    is.Pin = PIN_D0 | PIN_D1 | PIN_D2 | PIN_D3 | PIN_D4 | PIN_D5 | PIN_D6 | PIN_D7;
    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = PIN_D8 | PIN_D9 | PIN_D10 | PIN_D11 | PIN_D12 | PIN_D13 | PIN_D14 | PIN_D15;
    HAL_GPIO_Init(GPIOC, &is);

    forWrite = false;
};





StructPIO pinD16(WR);
StructPIO pinD17(RD);
StructPIO pinD18(CS);
StructPIO pinD19(RESET);
StructPIO pinD20(D_C);
StructPIO pinD21(DIP_ON);
StructPIO pinD22(BL_E);

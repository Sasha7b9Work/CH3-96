#include "defines.h"
#include "stm32f4xx_hal.h"
#include "Display.h"
#include "Hardware/HAL.h"
#include "Display/Primitives.h"
#include <cstring>
#include <cstdlib>


#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF

#define CS_OPEN         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS_CLOSE        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)

#define SET_DC_COMMAND  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define SET_DC_DATA     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define SET_RES_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define SET_RES_HI      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

// http://microsin.net/adminstuff/hardware/ssd1322-oled-controller.html

#define COM_ENABLE_GRAY_SCALE_TABLE     0x00    /// Enable Gray Scale Table
#define COM_COL_ADDRESS                 0x15    /// Set Column Address
#define COM_WRITE_RAM  	                0x5C    /// Write RAM Command
#define COM_ROW_ADDRESS                 0x75    /// Set Row Address
#define COM_REMAP_AND_DUAL              0xA0    /// Set Re-map and Dual COM Line mode
#define COM_DISPLAY_START_LINE          0xA1    /// Set Display Start Line
#define COM_DISPLAY_OFFSET              0xA2    /// Set Display Offset
#define COM_DISPLAY_MODE_GS0            0xA4    /// Все пикселы погашены
#define COM_DISPLAY_MODE_GS15           0xA5    /// Дисплей переводится в режим, в котором все пиксели имеют максимальную яркость GS15
#define COM_DISPLAY_MODE_NORMAL         0xA6    /// Нормальная работа (состояние после сброса).
#define COM_DISPLAY_MODE_INVERSE        0xA7    /// Инверсия дисплея
#define COM_EXIT_PARTIAL_DISPLAY        0xA9    /// Exit Partial Display
#define COM_FUNC_SELECT_VDD             0xAB    /// Function Selection
                                                /// A[0] = 0b, выбрать внешнее питание для VDD (внутренний регулятор запрещен).
                                                /// A[0] = 1b, разрешить работу внутреннего регулятора для VDD(состояние по умолчанию после сброса).
#define COM_SLEEP_MODE_ON               0xAE    /// Set Sleep mode ON
#define COM_SLEEP_MODE_OFF              0xAF    /// Set Sleep mode OFF
#define COM_PHASE_LENGTH                0xB1    /// Set Phase Length
#define COM_FRONT_CLOCK_DIV             0xB3    /// Set Front Clock Divider / Oscillator Frequency
#define COM_DISPLAY_ENHANCEMENT_A       0xB4    /// Display Enhancement A
                                                /// A[1:0] = 00b: разрешить внешнее VSL
                                                /// A[1:0] = 10b : внутреннее VSL(после сброса)
                                                /// B[7:3] = 11111b : улучшает качество GS дисплея
                                                /// B[7:3] = 10110b : обычный режим(после сброса)
#define COM_GPIO                        0xB5    /// Set GPIO
#define COM_SECOND_PRECHARGE_T          0xB6    /// Set Second Precharge Period
                                                /// A[3:0] устанавливает длительность второго периода предзаряда. 
#define COM_GRAY_SCALE_TABLE            0xB8    /// Set Gray Scale Table
#define COM_DEFAULT_GRAY_SCALE_TABLE    0xB9    /// Select Default Linear Gray Scale table
#define COM_PRECHARGE_VOLTAGE           0xBB    /// Set Pre-charge voltage
#define COM_VCOMH                       0xBE    /// Set VCOMH
#define COM_CONTRAST                    0xC1    /// Set Contrast Current
#define COM_MASTER_CONTRAST             0xC7    /// Master Contrast Current Control
#define COM_MUX_RATIO                   0xCA    /// Set MUX Ratio
#define COM_DISPLAY_ENHANCEMENT_B       0xD1    /// Display Enhancement B
                                                /// A[5:4] = 00b: зарезервированное состояние
                                                /// A[5:4] = 10b : нормальное состояние(после сброса).
#define COM_LOCK                        0xFD    /// Set Command Lock
                                                /// A[2] определяет защиту интерфейса от доступа со стороны микроконтроллера(значение после сброса 12h).


using namespace Display::Primitives;

static void Delay(uint ms);
static void SendData(uint8 data);
static void SendData(uint8 *data, uint16 num);
static void SendCommand(uint8 command);
static void SendCommand(uint8 command, uint8 data);
static void SendCommand(uint8 command, uint8 data0, uint8 data1);
static void SendCommand(uint8 command, uint8 *data, uint16 num);

/// В этом буфере будем рисовать. Ширина равна 256 / 2 потому, что в байте хранятся 2 пикселя с 1 из 16-ти возможных градация каждая.
#define WIDTH_BUFFER    (256)
#define HEIGHT_BUFFER   (64)
static uint8 front[HEIGHT_BUFFER][WIDTH_BUFFER];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    CS_CLOSE;
    
    Delay(500);
    SET_RES_HI;
    Delay(500);
    SET_RES_LOW;
    Delay(500);
    SET_RES_HI;
    Delay(500);

    SendCommand(COM_LOCK, 0x12);                        // CommandLock
    SendCommand(COM_SLEEP_MODE_ON);                     // Sleep in
    SendCommand(COM_FRONT_CLOCK_DIV, 0x91);             // Set Display Clock Divide Ratio/Oscillator Frequency
    SendCommand(COM_MUX_RATIO, 0x3F);                   // Set Multipex Ratio
    SendCommand(COM_DISPLAY_OFFSET, 0x00);              // Set display offset
    SendCommand(COM_DISPLAY_START_LINE, 0x00);          // Set display start line
    SendCommand(COM_REMAP_AND_DUAL, 0x14, 0x11);        // Set re-map & dual COM line mode
    SendCommand(COM_GPIO, 0x00);                        // Set GPIO
    SendCommand(COM_FUNC_SELECT_VDD, 0x01);             // Function selection
    SendCommand(COM_DISPLAY_ENHANCEMENT_A, 0xA2, 0xFD); // Enable external VSL
    SendCommand(COM_CONTRAST, 0xFF);                    // Set contrast current
    SendCommand(COM_MASTER_CONTRAST, 0x0F);             // Master contrast current control

    uint8 data[16] =
    {
        0x00, 0x01, 0x03, 0x06, 0x0A, 0x10, 0x1A, 0x28,
        0x37, 0x47, 0x58, 0x6A, 0x7F, 0x96, 0xB4
    };

    SendCommand(COM_GRAY_SCALE_TABLE, data, 15);
    SendCommand(COM_ENABLE_GRAY_SCALE_TABLE);           // Enable gray scale table
    SendCommand(COM_PHASE_LENGTH, 0xE8);                // Set phase length
    SendCommand(COM_DISPLAY_ENHANCEMENT_B, 0x82, 0x20); // Enhance driving scheme capability
    SendCommand(COM_PRECHARGE_VOLTAGE, 0x1F);           // Set pre-charge voltage
    SendCommand(COM_SECOND_PRECHARGE_T, 0x08);          // Set second pre-charge period
    SendCommand(COM_VCOMH, 0x07);                       // Set VCOMH deselect level
    SendCommand(COM_DISPLAY_MODE_NORMAL);               // Set display mode
    SendCommand(COM_SLEEP_MODE_OFF);                    // Sleep out
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetPixel(int x, int y)
{
    return &front[y][x];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::BeginScene(Color color)
{
    std::memset(front, color.value, WIDTH_BUFFER * HEIGHT_BUFFER);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::EndScene()
{
    for (int row = 0; row < HEIGHT_BUFFER; row++)
    {
        SendCommand(COM_COL_ADDRESS, 28, 127);      // Устанавливаем адреса начального и конечного столбца. Первое число задает начальный адрес, второе - конечный адрес.
        SendCommand(COM_ROW_ADDRESS, (uint8)(HEIGHT_BUFFER - row - 1), (uint8)(HEIGHT_BUFFER - row - 1));     // Устанавливает адреса начальной и конечной строки. Первое число задает начальный адрес, второе - конечный адрес.
//        SendCommand(COM_ROW_ADDRESS, row, row);
        SendCommand(COM_WRITE_RAM);                 // Разрешаем микроконтроллеру записать данные в RAM.

        static uint8 line[128];

        for (int i = 0; i < WIDTH_BUFFER; i += 2)
        {
            line[127 - i / 2] = (uint8)(front[row][i] | (front[row][i + 1] << 4));
            //line[i / 2] = (uint8)(front[row][i + 1] | (front[row][i] << 4));
        }

        SendData(line, 128);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Delay(uint ms)
{
    uint timestamp = HAL_GetTick();
    while (HAL_GetTick() - timestamp < ms)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command)
{
    CS_OPEN;
    SET_DC_COMMAND;
    HAL::SPI1_::Send(&command, 1);
    CS_CLOSE;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendData(uint8 data)
{
    CS_OPEN;
    SET_DC_DATA;
    HAL::SPI1_::Send(&data, 1);
    CS_CLOSE;
}

////-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendData(uint8 *data, uint16 num)
{
    CS_OPEN;
    SET_DC_DATA;
    HAL::SPI1_::Send(data, num);
    CS_CLOSE;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 data)
{
    SendCommand(command);
    SendData(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 data0, uint8 data1)
{
    SendCommand(command);
    SendData(data0);
    SendData(data1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SendCommand(uint8 command, uint8 *data, uint16 num)
{
    SendCommand(command);
    SendData(data, num);
}

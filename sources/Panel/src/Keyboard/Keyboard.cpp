// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include <stm32f4xx_hal.h>


#define TIME_UPDATE_KEYBOARD 2   // ����� ����� �������� ����������
#define NUM_RL 4
#define NUM_SL 4

#define PIN_SL0  GPIO_PIN_9
#define PORT_SL0 GPIOB

#define PIN_SL1  GPIO_PIN_8
#define PORT_SL1 GPIOB

#define PIN_SL2  GPIO_PIN_7
#define PORT_SL2 GPIOB

#define PIN_SL3  GPIO_PIN_6
#define PORT_SL3 GPIOB

#define PIN_RL0  GPIO_PIN_5
#define PORT_RL0 GPIOB

#define PIN_RL1  GPIO_PIN_4
#define PORT_RL1 GPIOB

#define PIN_RL2  GPIO_PIN_2
#define PORT_RL2 GPIOD

#define PIN_RL3  GPIO_PIN_15    // ���� ���������� ������ ������ �����
#define PORT_RL3 GPIOA

#define PIN_ENC1  GPIO_PIN_11
#define PORT_ENC1 GPIOC

#define PIN_ENC2  GPIO_PIN_10
#define PORT_ENC2 GPIOC


static TIM_HandleTypeDef handleTIM4;


static Control::E controls[NUM_SL][NUM_RL] =
{
    {Control::Service,      Control::Test,  Control::Auto,          Control::GovButton},
    {Control::Channels,     Control::Mode,  Control::Indication,    Control::None},
    {Control::Enter,        Control::Left,  Control::None,          Control::None},
    {Control::Right,        Control::None,  Control::None,          Control::None},
};

/// ������� ��������� - ����� ��� ������� ������� ����������
#define MAX_ACTIONS 100
static Control actions[MAX_ACTIONS];
/// ���������� ��� ��������� ���������
static int numActions = 0;

/// ������������� � true �������� ��������, ��� ��������� ����-���� ���������� � � ��������� ������ �� ���������� (����� �� ���� ������ ������������ ���� Long
static bool alreadyLong[NUM_RL][NUM_SL];
/// ��� ����������� ������� ������ ���� ������������ ����� �������
static uint timePress[NUM_SL][NUM_RL];

/// ����� ���������� ����������� �����
volatile uint prevRepeat = 0;
volatile uint prevPause = 0;



/// ���������� ��������� ����
static void Set_SL(int, int);
/// ���������� ��������� ���� ����� � ���� ���������
static void Set_All_SL(int);
/// ���������� ��������� ���� rl
static int Read_RL(int rl);
/// ���������������� ����
static void InitPins();
/// ���������������� ������ ��� �������������� ������ ����������
static void InitTimer();
/// �������, ������������ ���������� �� ���������� �������
static void Update();
/// �������� �������� � �����
static void AddAction(Control control, Control::Action::E action);
/// ��������� �����
static void DetectRegulator();

#define BUTTON_IS_PRESS(state) ((state) == 0)



static void Update()
{
    uint time = HAL_GetTick();

    Set_All_SL(1);

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            Control control = controls[sl][rl];

            if (control.value != Control::None)
            {
                if (timePress[sl][rl])                      // ���� ������� ��������� � ������� ���������
                {
                    if (time - timePress[sl][rl] > 100)     // ���� ������ ����� 100 �� � ������� �������
                    {
                        if (!BUTTON_IS_PRESS(state))        // ���� ������ ������ ��������� � ������� ���������
                        {
                            timePress[sl][rl] = 0;
                            if (!alreadyLong[sl][rl])
                            {
                                AddAction(controls[sl][rl], Control::Action::Release);
                            }
                            alreadyLong[sl][rl] = false;
                            prevRepeat = 0;
                        }
                        else if (time - timePress[sl][rl] > 500 && !alreadyLong[sl][rl])
                        {
                            AddAction(controls[sl][rl], Control::Action::Long);
                            alreadyLong[sl][rl] = true;
                        }
                    }
                }
                else if (BUTTON_IS_PRESS(state) && !alreadyLong[sl][rl])
                {
                    timePress[sl][rl] = time;
                    AddAction(controls[sl][rl], Control::Action::Press);
                    prevRepeat = 0;
                }
            }
        }

        Set_All_SL(1);
    }

    DetectRegulator();
    Set_All_SL(1);
}

static void DetectRegulator()
{
    // ����������� �������
    static bool prevStatesIsOne = false;

    bool stateLeft = (HAL_GPIO_ReadPin(PORT_ENC1, PIN_ENC1) == GPIO_PIN_SET);
    bool stateRight = (HAL_GPIO_ReadPin(PORT_ENC2, PIN_ENC2) == GPIO_PIN_SET);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
        prevStatesIsOne = false;

        AddAction(Control::GovLeft, Control::Action::Press);
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
        prevStatesIsOne = false;

        AddAction(Control::GovRight, Control::Action::Press);
    }
}

bool Keyboard::IsPressed()
{
    bool keyboardFail = false;

    Set_All_SL(1);

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            Control control = controls[sl][rl];

            if (control.value != Control::None)
            {
                if (BUTTON_IS_PRESS(state))
                {
                    keyboardFail = true;
                }
            }
        }

        Set_All_SL(1);
    }
    return keyboardFail;
}

void Keyboard::Init()
{
    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            timePress[sl][rl] = 0;
        }
    }

    InitPins();

    InitTimer();
}


void Set_All_SL(int st)
{
    for (int i = 0; i < 4; i++)
    {
        Set_SL(i, st);
    }
}


void Set_SL(int bus, int st)
{
    static const GPIO_TypeDef *ports[4] = { PORT_SL0, PORT_SL1, PORT_SL2, PORT_SL3 };
    static const uint16 pins[4] = { PIN_SL0,  PIN_SL1,  PIN_SL2,  PIN_SL3 };
    static const GPIO_PinState state[2] = { GPIO_PIN_RESET, GPIO_PIN_SET };

    HAL_GPIO_WritePin((GPIO_TypeDef *)ports[bus], pins[bus], state[st]); //-V2567
}


int Read_RL(int rl)
{
    static const GPIO_TypeDef *ports[4] = { PORT_RL0, PORT_RL1, PORT_RL2, PORT_RL3 };
    static const uint16 pins[4] = { PIN_RL0,  PIN_RL1,  PIN_RL2, PIN_RL3 };

    return HAL_GPIO_ReadPin((GPIO_TypeDef *)ports[rl], pins[rl]); //-V2567
}


static void InitPins()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef is =
    {
        PIN_SL0 | PIN_SL1 | PIN_SL2 | PIN_SL3 | PIN_RL0 | PIN_RL1,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = PIN_RL0 | PIN_RL1;
    is.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = PIN_RL2;
    HAL_GPIO_Init(GPIOD, &is);

    is.Pin = PIN_RL3;
    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = PIN_ENC1 | PIN_ENC2;
    HAL_GPIO_Init(GPIOC, &is);
}


static void InitTimer()
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    // �������������� ������, �� ����������� �������� ����� ���������� ����������
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE_KEYBOARD * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&handleTIM4) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }

    if (HAL_TIM_Base_Start_IT(&handleTIM4) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }

    Set_All_SL(1);
}


static void AddAction(Control control, Control::Action::E action)
{
    if (action != Control::Action::Press)
    {
        return;
    }

    control.action = action;
    actions[numActions++] = control;

}


void Keyboard::AppendControl(const Control &control)
{
    AddAction(control, control.action.value);
}


bool Keyboard::Empty()
{
    return numActions == 0;
}



Control Keyboard::NextControl()
{
    if (Empty())
    {
        return Control();
    }

    Control result = actions[0];

    for (int i = 1; i < numActions; i++)
    {
        actions[i - 1] = actions[i];
    }

    --numActions;
    return result;
}


String Control::Name() const
{
    static pchar names[] =
    {
        "����� ������",
        "�����",
        "���������",
        "3",
        "<-",
        "->",
        "6",
        "7",
        "8",
        "9",
        "A",
        "B",
        "C",
        "D",
        "F5",
        "ESC",
        "Left",
        "Right",
        "����� �����",
        "����� ������",
        "Channel",
        "����",
        "����",
        "NULL"
    };

    return String(names[value]);
}


#ifdef __cplusplus
extern "C" {
#endif

    void TIM4_IRQHandler();

    void TIM4_IRQHandler()
    {
        HAL_TIM_IRQHandler(&handleTIM4);
    }

#ifdef __cplusplus
}
#endif


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //-V2009 //-V2558
{
    if (htim == &handleTIM4)
    {
        Update();
    }
}


void Keyboard::Lock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncEmptyUpdate);
}


void Keyboard::Unlock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncUpdate);
}

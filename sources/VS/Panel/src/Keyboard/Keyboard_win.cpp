// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/Application.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"


// ������� ��������� - ����� ��� ������� ������� ����������
#define MAX_ACTIONS 100
static Control actions[MAX_ACTIONS];
// ���������� ��� ��������� ���������
static int numActions = 0;

static bool needStartTimerLong = false;

static bool needStopTimerLong = false;
// ����� ��� ������� ������
static Control::E pressedKey = Control::None;


bool Keyboard::Init()
{
    return true;
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


void Frame::OnDown(wxCommandEvent &event)
{
    Control::E key = (Control::E)event.GetId();

    //std::cout << "down " << Control(key).Name() << std::endl;
    event.Skip();

    AddAction(key, Control::Action::Press);

    needStartTimerLong = true;

    pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
    Control::E key = (Control::E)event.GetId();

    //std::cout << "up   " << Control(key).Name() << std::endl;
    event.Skip();

    AddAction(key, Control::Action::Release);

    needStopTimerLong = true;

    pressedKey = Control::None;
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
    static const char *names[Control::E::Count] =
    {
        /*  1 */   "����� ������",
        /*  2 */   "�����",
        /*  3 */   "���������",
        /*  4 */   "<-",
        /*  5 */   "->",
        /*  6 */   "������",
        /*  7 */   "ENTER",
        /*  8 */   "������",
        /*  9 */   "����� ����",
        /* 10 */   "����� �����",
        /* 11 */   "����",
        /* 12 */   "����",
        /* 13 */   "NULL"
    };

    return String(names[value]);
}


void Keyboard::Lock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncEmptyUpdate);
}


void Keyboard::Unlock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncUpdate);
}

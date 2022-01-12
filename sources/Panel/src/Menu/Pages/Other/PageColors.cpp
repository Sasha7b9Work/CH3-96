// 2022/01/12 11:54:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Other/PageColors.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageService.h"
#include "Display/Primitives.h"


using namespace Primitives;


static void Draw_Colors()
{
    Rectangle(470, 225).FillRounded(5, 5, 1, Color::GREEN_20, Color::WHITE);
}


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageService::self);
}


DEF_BUTTON(bExit,
    "�����", "Exit",
    "������� �� �������� ������", "Go to the SERVICE page",
    OnPress_Exit
);


static void OnChanged_Color()
{

}


DEF_CHOICE_2(cColor,
    "����� �������������� �����", "Choosing a custom color",
    "��� 1",  "��� 2",
    "Back 1", "Back 2",
    OnChanged_Color
);


static void OnPress_Tune()
{

}


DEF_BUTTON(bTune,
    "���������", "Tune",
    "��������� ����", "Tune color",
    OnPress_Tune
);


static Item *items[7] =
{
    &bExit,
    &cColor,
    &bTune,
    nullptr
};


static Page page(items, nullptr, Draw_Colors, true);


Page *PageColors::self = &page;
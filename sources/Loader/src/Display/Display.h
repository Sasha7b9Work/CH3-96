// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"



#define DISPLAY_ADD_STRING(x) Display::AddStringToIndicating(x)



class Display
{
public:
    static void Init();
    static void Update();
    static bool IsRun();
    static void AddStringToIndicating(pString string);
    // \todo ��������. ����� ����������
    static void SaveRow(int) {};
};

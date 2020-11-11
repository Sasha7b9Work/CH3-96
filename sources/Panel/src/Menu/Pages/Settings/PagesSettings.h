#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Settings.h"


// ������� �������������
struct InputImpedance : public Enumeration
{
    enum E
    {
        _1MOmh,         // 1 ���
        _50Omh          // 50 ��
    };

    explicit InputImpedance(E v) : Enumeration((uint8)v) {};

    bool Is_1MOhm() const { return (value == _1MOmh); }
    bool Is_50Ohm() const { return (value == _50Omh); }

    static InputImpedance &Current();
    static void LoadToFPGA();
};


// �������� ����������
struct Divider : public Enumeration
{
    enum E
    {
        _1,           // 1:1
        _10           // 1:10
    };

    explicit Divider(E v) : Enumeration((uint8)v) {};
    static Divider &Current();
    bool Is1() const { return (value == _1); }
    int ToAbs() const { return Is1() ? 1 : 10; }

    static void LoadToFPGA();
};


struct PageSettingsA
{
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ��������� �� ������������� ���� �������������
};


struct PageSettingsB
{
    static Page *self;
    static InputCouple couple;
    static InputImpedance impedance;
    static ModeFilter modeFilter;
    static ModeFront modeFront;
    static Divider divider;
    static TypeSynch typeSynch;
    static void FixPress();
    static Switch *switchTypeSynch;     // ��������� �� ������������� ���� �������������
};


struct PageSettingsC
{
    static Page *self;
};

struct PageSettingsD
{
    static Page *self;
};

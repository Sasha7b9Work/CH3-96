#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PageModesA.h"
#include <cstring>

using namespace Display::Primitives;
using namespace Display;

extern Switch sSync;
extern Item *items[7];

InputCouple     PageSettingsA::couple(InputCouple::AC);
InputImpedance  PageSettingsA::impedance(InputImpedance::_1MOmh);
ModeFilter      PageSettingsA::modeFilter(ModeFilter::Off);
ModeFront       PageSettingsA::modeFront(ModeFront::Front);
Divider         PageSettingsA::divider(Divider::_1);
TypeSynch       PageSettingsA::typeSynch(TypeSynch::Manual);


static void OnPress_Couple()
{
    InputCouple::Load();
}

// ����� ���������(���������) ����� �������� ������
DEF_SWITCH_UGO_2(sCouple,
    "����", "����",
    "�������� ����", "�������� ����",
    "\x81", "\x80",
    PageSettingsA::couple, OnPress_Couple
);


static void OnPress_Impedance()
{
    FreqMeter::LoadImpedance();
}

// ��������� �������� ������������� �������� ������
DEF_SWITCH_UGO_2(sImpedance,
    "R��", "������� ������������� ������",
    "1 ���", "50 ��",
    "1���", "50��",
    PageSettingsA::impedance, OnPress_Impedance
);


static void OnPress_Filter()
{
    ModeFilter::Load();
}

// ���������(����������) ���
DEF_SWITCH_UGO_2(sLowpassFilter,
    "���", "���������/���������� ������� ��",
    "����.", "���.",
    "���", "",
    PageSettingsA::modeFilter, OnPress_Filter
);


static void OnPress_Front()
{
    FreqMeter::LoadModeFront();
}

// ����� ������ ������������� �������� ������
DEF_SWITCH_UGO_2(sFront,
    "\x82/\x83", "����� ���� �������������",
    "�����", "����",
    "\x82", "\x83",
    PageSettingsA::modeFront, OnPress_Front
);


static void OnPress_Divider()
{
    FreqMeter::LoadDivider();
}

// ���������(����������) �������� �������� ����������
DEF_SWITCH_UGO_2(sDivider,
    "��������", "���/���� �������� ��������",
    "1:1", "1:10",
    "1:1", "1:10",
    PageSettingsA::divider, OnPress_Divider
);


static void OnPress_TimeLabels()
{
    uint8 period = PageModesA::periodTimeLabels.value;

    PageModesB::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

// ����� ������� ����� �������
DEF_SWITCH_6(sPeriodTimeLabels,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesA::periodTimeLabels,  OnPress_TimeLabels
);


static void OnPress_TimeMeasure()
{
    uint8 time = PageModesA::timeMeasure.value;

    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;
    PageModesD::timeMeasure.value = time;

    static const uint8 periods[TimeMeasure::Count] =
    {
        NumberPeriods::_1,
        NumberPeriods::_10,
        NumberPeriods::_100,
        NumberPeriods::_1K,
        NumberPeriods::_10K,
        NumberPeriods::_100K,
        NumberPeriods::_1000K
    };

    uint8 period = periods[time];

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

    FreqMeter::LoadTimeMeasure();
}

// ����� ������� ���������
DEF_SWITCH_7(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesA::timeMeasure, OnPress_TimeMeasure
);


static void OnPress_Sync()
{
    if (PageSettingsA::typeSynch.IsHoldoff())
    {
        items[0] = &sSync;
        items[1] = &sTimeMeasure;
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;

        PageSettingsA::self->selectedItem = 0;
    }
    else if(PageSettingsA::typeSynch.IsManual())
    {
        items[0] = &sCouple;
        items[1] = &sImpedance;
        items[2] = &sLowpassFilter;
        items[3] = &sFront;
        items[4] = &sDivider;
        items[5] = &sSync;
        items[6] = nullptr;
        
        PageSettingsA::self->selectedItem = 5;
    }

    TYPE_SYNCH_A = (TypeSynch::E)PageSettingsA::typeSynch.value;
    FreqMeter::LoadTypeSynch();
}

// ����� ������ ������������� ���, ���
DEF_SWITCH_UGO_2(sSync,
    "�����", "����� ������ ������������",
    "����", "Holdoff",
    "����", "Holdoff",
    PageSettingsA::typeSynch, OnPress_Sync
);

static Item *items[7] =
{
    &sCouple,
    &sImpedance,
    &sLowpassFilter,
    &sFront,
    &sDivider,
    &sSync,
    nullptr
};

static Page pageChannelA(items);

Page *PageSettingsA::self = &pageChannelA;

Switch *PageSettingsA::switchTypeSynch = &sSync;

void PageSettingsA::FixPress()
{
    OnPress_Sync();
}


InputImpedance &InputImpedance::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageSettingsA::impedance : PageSettingsB::impedance;
}


ModeFront &ModeFront::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageSettingsA::modeFront : PageSettingsB::modeFront;
}


Divider &Divider::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageSettingsA::divider : PageSettingsB::divider;
}
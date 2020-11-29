#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;

static bool relationOn = false;
static bool startStop = false;


void Channel::RelationOn()
{
    relationOn = true;
}

void Channel::RelationOff()
{
    relationOn = false;
}

bool Channel::RelationCheck()
{
    return relationOn == true;
}


void Channel::PressSetupA()
{
    switch (Channel::A.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        items[1] = &sModeFrequency;
        break;

    case TypeMeasure::Period:
        items[1] = &sModePeriod;
        break;

    case TypeMeasure::Duration:
        items[1] = &sModeDuration;
        break;

    case TypeMeasure::CountPulse:
        items[1] = &sModeCountPulse;
        break;
    }
}


static void OnChanged_TypeMeasure()
{
    Channel::Current().OnChanged_TypeMeasure();
}


DEF_SWITCH_4(sTypeMeasure,
    "���������", "����� ���������",
    "�������", "������", "����.", "��. ���.",
    Channel::A.typeMeasure, OnChanged_TypeMeasure
);


static void OnPress_Statistics()
{
    Menu::SetOpenedPage(PageStatistics::self);
}

DEF_BUTTON(bStatistics, "����", "��������� ���� ����������", OnPress_Statistics);


static void OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (Channel::A.modeFrequency.IsFrequency())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
        Channel::RelationOff();
    }
    else if (Channel::A.modeFrequency.IsRatioAB())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = nullptr;
        Channel::RelationOn();
    }
    else if (Channel::A.modeFrequency.IsRatioAC())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = nullptr;
        Channel::RelationOn();
    }
    else if (Channel::A.modeFrequency.IsT_1())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
        Channel::RelationOff();
    }
    else if (Channel::A.modeFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[3] = nullptr;
        Channel::RelationOff();
    }
    else if (Channel::A.modeFrequency.IsComparator())
    {
        items[2] = &bStatistics;
        items[3] = nullptr;
        Channel::RelationOff();
    }

    ModeFrequency::LoadToFPGA();
}

// ����� ������ ��������� �������, ��������� ������, "���������"
DEF_SWITCH_6(sModeFrequency,
    "�����", "��������� �������",
    "�������",
    "f=1/T",
    "f(A)/f(B)",
    "f(A)/f(C)",
    "��������",
    "����������",
    Channel::A.modeFrequency, OnChanged_ModeFrequency
);


Switch *switchModeFrequencyA = &sModeFrequency;


static void OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (Channel::A.modePeriod.IsPeriod())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (Channel::A.modePeriod.IsF_1())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }

    Channel::RelationOff();
    
    ModePeriod::LoadToFPGA();
}

// ����� ������ ��������� �������
DEF_SWITCH_2(sModePeriod,
    "�����", "��������� �������",
    "������", "T=1/f",
    Channel::A.modePeriod, OnChanged_ModePeriod
);


Switch *switchModePeriodA = &sModePeriod;


static void OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    switch (Channel::A.modeDuration.value)
    {
    case ModeDuration::Ndt_1ns:
        items[2] = nullptr;
        break;

    case ModeDuration::Ndt:
    case ModeDuration::StartStop:
    case ModeDuration::FillFactor:
    case ModeDuration::Phase:
        items[2] = Channel::switchTimeLabels; 
        items[3] = nullptr;
        break;
    }

    Channel::RelationOff();

    ModeDuration::LoadToFPGA();
}

// ����� ������ ��������� ������������ ���������, ����������, ������������ ����������, �������� ���
DEF_SWITCH_5(sModeDuration,
    "�����", "��������� ������������",
    "ndt", "ndt/1��", "������-�����", "�����. ���.", "����",
    Channel::A.modeDuration, OnChanged_ModeDuration
);


Switch *switchModeDurationA = &sModeDuration;


static void OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (Channel::A.modeMeasureCountPulse.value)
    {
    case ModeCountPulse::AtB:
        break;

    case ModeCountPulse::ATB:
        items[2] = Channel::switchNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeCountPulse::StartStop:
        break;
    }

    Channel::RelationOff();

    ModeCountPulse::LoadToFPGA();
}

// ����� ������ ����� ���������
DEF_SWITCH_3(sModeCountPulse,
    "�����", "���� ����� ���������",
    "�(tB)", "�(TB)", "�����/����",
    Channel::A.modeMeasureCountPulse, OnChanged_ModeCountPulse
);


Switch *switchModeCountPulseA = &sModeCountPulse;


bool Channel::StartStop()
{
    return startStop;
}

void Channel::ToggleStartStop()
{
    if (!startStop)
    {
        startStop = true;
    }
    else
    {
        startStop = false;
    }
}


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    Channel::switchTimeMeasue,
    nullptr
};


static void OnChanged_ModeTest()
{
    bool test = FreqMeter::modeTest.IsEnabled();

    switch(Channel::A.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (Channel::A.modeFrequency.IsFrequency())
        {
            items[3] = test ? Channel::switchTimeLabels : nullptr;
        }
        else if (Channel::A.modeFrequency.IsTachometer())
        {
            items[2] = test ? Channel::switchTimeLabels : nullptr;
        }
        break;

    case TypeMeasure::Period:
        items[3] = test ? Channel::switchTimeLabels : nullptr;
        break;
    }

    Channel::A.pageModes->VerifySelectedItem();
}

static void OnEvent(EventType::E event)
{
    if (event == EventType::ModeTestChanged)
    {
        OnChanged_ModeTest();
    }
}


Page pageModesA(items, OnEvent);

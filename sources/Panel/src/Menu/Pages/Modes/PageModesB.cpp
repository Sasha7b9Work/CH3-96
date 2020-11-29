#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/FreqMeter.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;       
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;


void Channel::PressSetupB()
{
    switch (Channel::B.typeMeasure.value)
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
    Channel::B.typeMeasure, OnChanged_TypeMeasure
)


static void OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    Channel::RelationOff();

    if (Channel::B.modeFrequency.IsFrequency())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }
    else if (Channel::B.modeFrequency.IsRatioBA())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = nullptr;
        Channel::RelationOn();
    }
    else if (Channel::B.modeFrequency.IsRatioBC())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = nullptr;
        Channel::RelationOn();
    }
    else if (Channel::B.modeFrequency.IsT_1())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if(Channel::B.modeFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[3] = nullptr;
    }

    ModeFrequency::LoadToFPGA();
}

// ����� ������ ��������� �������, ��������� ������, "���������"
DEF_SWITCH_5(sModeFrequency,
    "�����", "��������� �������",
    "�������", "f=1/T", "f(B)/f(A)", "f(B)/f(C)", "��������",
    Channel::B.modeFrequency, OnChanged_ModeFrequency
);


Switch *switchModeFrequencyB = &sModeFrequency;


static void OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (Channel::B.modePeriod.IsPeriod())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (Channel::B.modePeriod.IsF_1())
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
    Channel::B.modePeriod, OnChanged_ModePeriod
);


Switch *switchModePeriodB = &sModePeriod;


static void OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    switch (Channel::B.modeDuration.value)
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
    "ndt", "ndt/1��", "�����B-����A", "�����. ���.", "����",
    Channel::B.modeDuration, OnChanged_ModeDuration
);


Switch *switchModeDurationB = &sModeDuration;


static void OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (Channel::B.modeMeasureCountPulse.value)
    {
    case ModeCountPulse::BtA:
        break;

    case ModeCountPulse::BTA:
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
    "B(tA)", "B(TA)", "�����/����",
    Channel::B.modeMeasureCountPulse, OnChanged_ModeCountPulse
);


Switch *switchModeCountPulseB = &sModeCountPulse;


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

    switch (Channel::B.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (Channel::B.modeFrequency.IsFrequency())
        {
            items[3] = test ? Channel::switchTimeLabels : nullptr;
        }
        else if (Channel::B.modeFrequency.IsTachometer())
        {
            items[2] = test ? Channel::switchTimeLabels : nullptr;
        }
        break;
    case TypeMeasure::Period:
        items[3] = test ? Channel::switchTimeLabels : nullptr;
        break;
    }

    Channel::B.pageModes->VerifySelectedItem();
}


static void OnEvent(EventType::E event)
{
    switch(event)
    {
    case EventType::ModeTestChanged:
        OnChanged_ModeTest();
        break;

    case EventType::Count:
        break;
    }
}


Page pageModesB(items, OnEvent);

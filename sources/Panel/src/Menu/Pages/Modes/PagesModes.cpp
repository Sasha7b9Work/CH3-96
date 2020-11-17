#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


bool CurrentPageModes::ConsistTimeMeasure()
{
    if (TypeMeasure::Current().IsFrequency())
    {
        if (ModeMeasureFrequency::Current().IsFrequency() || ModeMeasureFrequency::Current().IsRatioAC() || ModeMeasureFrequency::Current().IsRatioBC())
        {
            return true;
        }
    }

    if (TypeMeasure::Current().IsPeriod() && ModeMeasurePeriod::Current().IsF_1())
    {
        return true;
    }

    return false;
}


// ����� ������� ���������
#ifdef USE_1000s

DEF_SWITCH_7(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#else

DEF_SWITCH_6(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure, TimeMeasure::LoadToFPGA
);

#endif


Switch *PageModes::switchTimeMeasue = &sTimeMeasure;


// ����� ����� ����������� �������� �������� �������
#ifdef USE_1000K

DEF_SWITCH_7(sNumberPeriods,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModes::numberPeriods, NumberPeriods::LoadToFPGA
);

#else

DEF_SWITCH_6(sNumberPeriods,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K",
    PageModes::numberPeriods, NumberPeriods::LoadToFPGA
);

#endif


Switch *PageModes::switchNumberPeriods = &sNumberPeriods;


// ����� ������� ����� �������
DEF_SWITCH_6(sTimeLabels,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::timeLabels, PeriodTimeLabels::LoadToFPGA
);


Switch *PageModes::switchTimeLabels = &sTimeLabels;


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable: 4062)
#endif


bool PageModes::IsActiveTimeLabels(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeMeasureFrequency::E mode = (ModeMeasureFrequency::E)m;

        switch (mode)
        {
        case ModeMeasureFrequency::T_1:
        case ModeMeasureFrequency::RatioCA:
        case ModeMeasureFrequency::RatioCB:     return true;

        case ModeMeasureFrequency::Frequency:   return CURRENT_CHANNEL_IS_A_OR_B ? FreqMeter::modeTest.IsEnabled() : false;

        case ModeMeasureFrequency::Tachometer:  return FreqMeter::modeTest.IsEnabled();
        }
    }
    else if (type->IsPeriod())
    {
        ModeMeasurePeriod::E mode = (ModeMeasurePeriod::E)m;

        switch (mode)
        {
        case ModeMeasurePeriod::Period:      return true;

        case ModeMeasurePeriod::F_1:         return FreqMeter::modeTest.IsEnabled();
        }
    }
    else if (type->IsDuration())
    {
        ModeMeasureDuration::E mode = (ModeMeasureDuration::E)m;

        switch (mode)
        {
        case ModeMeasureDuration::Ndt:
        case ModeMeasureDuration::Ndt2:
        case ModeMeasureDuration::FillFactor:
        case ModeMeasureDuration::Phase:        return true;
        }
    }

    return false;
}


bool PageModes::IsActiveTimeMeasure(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeMeasureFrequency::E mode = (ModeMeasureFrequency::E)m;

        switch (mode)
        {
        case ModeMeasureFrequency::Frequency:
        case ModeMeasureFrequency::RatioAC:
        case ModeMeasureFrequency::RatioBC:     return true;
        }
    }
    else if (type->IsPeriod())
    {
        ModeMeasurePeriod::E mode = (ModeMeasurePeriod::E)m;

        switch (mode)
        {
        case ModeMeasurePeriod::F_1:            return true;
        }
    }

    return false;
}


bool PageModes::IsActiveNumberPeriods(TypeMeasure *type, int m)
{
    if (type->IsFrequency())
    {
        ModeMeasureFrequency::E mode = (ModeMeasureFrequency::E)m;

        switch (mode)
        {
        case ModeMeasureFrequency::T_1:
        case ModeMeasureFrequency::RatioAB:
        case ModeMeasureFrequency::RatioBA:
        case ModeMeasureFrequency::RatioCA:
        case ModeMeasureFrequency::RatioCB:     return true;
        }
    }
    else if (type->IsPeriod())
    {
        ModeMeasurePeriod::E mode = (ModeMeasurePeriod::E)m;

        switch (mode)
        {
        case ModeMeasurePeriod::Period:         return true;
        }
    }
    else if (type->IsCountPulse())
    {
        ModeMeasureCountPulse::E mode = (ModeMeasureCountPulse::E)m;

        switch (mode)
        {
        case ModeMeasureCountPulse::ATB:
        case ModeMeasureCountPulse::BTA:
        case ModeMeasureCountPulse::CTA:
        case ModeMeasureCountPulse::CTB:        return true;
        }
    }

    return false;
}


#ifdef WIN32
#pragma warning(pop)
#endif


static void DrawValue(Enumeration &param, int x, int y)
{
    int width = 60;
    Primitives::Rectangle(width, 30).FillRounded(x, y, 2, Color::GREEN_20, Color::WHITE);
    Text(param.ToString()).Write(x + 2, y + 7, width, Color::WHITE);
}


void PageModes::DrawParameters(int x, int y)
{
    Page &page = *Current();

    TypeMeasure *type = page.GetTypeMeasure();

    int mode = page.GetModeMeasure();

    int dX = 70;

    if (IsActiveTimeMeasure(type, mode))
    {
        DrawValue(PageModes::timeMeasure, x, y);
    }

    if (IsActiveNumberPeriods(type, mode))
    {
        DrawValue(PageModes::numberPeriods, x + dX, y);
    }

    if (IsActiveTimeLabels(type, mode))
    {
        DrawValue(PageModes::timeLabels, x + 2 * dX, y);
    }
}

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/Pages/Modes/Modes.h"


class Page;
class Switch;


#define CURRENT_CHANNEL             Channel::Current()
#define CURRENT_CHANNEL_IS_A        (CURRENT_CHANNEL->IsA())
#define CURRENT_CHANNEL_IS_B        (CURRENT_CHANNEL->IsB())
#define CURRENT_CHANNEL_IS_A_OR_B   (CURRENT_CHANNEL_IS_A || CURRENT_CHANNEL_IS_B)
#define CURRENT_CHANNEL_IS_C        (CURRENT_CHANNEL->IsC())
#define CURRENT_CHANNEL_IS_D        (CURRENT_CHANNEL->IsD())

#define NUMBER_CHANNEL(ch)          (ch->Number())
#define NUMBER_CURRENT_CHANNEL      (NUMBER_CHANNEL(CURRENT_CHANNEL))


struct SettingsChannel
{
    SettingsChannel();

    InputCouple    couple;
    InputImpedance impedance;
    ModeFilter     modeFilter;
    ModeSynch      modeSynch;
    Divider        divider;
    TypeSynch      typeSynch;
};


struct ModesChannel
{
    ModesChannel(Switch *switchModeFrequency, Switch *switchModeCountPulse, Switch *switchModePeriod,
        Switch *switchModeDuration,
        const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse);

    TypeMeasure    typeMeasure;
    ModeFrequency  modeFrequency;    // ����� ��������� �������
    ModePeriod     modePeriod;       // ����� ��������� �������
    ModeDuration   modeDuration;     // ����� ��������� ������������
    ModeCountPulse modeCountPulse;   // ����� ����� ���������

    Switch *switchModeFrequency;
    Switch *switchModeCountPulse;
    Switch *switchModePeriod;
    Switch *switchModeDuration;

    static TimeMeasure       timeMeasure;       // ����� �����
    static NumberPeriods     numberPeriods;     // ����� �������� ���������
    static PeriodTimeLabels  timeLabels;        // ������ ����� �������
    static TimeComparator    timeComparator;    // ����� ��������� �����������

    static Switch           *switchTimeMeasue;
    static Switch           *switchNumberPeriods;
    static Switch           *switchTimeLabels;
    static Switch           *switchTimeComparator;

    // ���������� true, ���� ������� �������� ������� �������� ����� ���������
    bool ConsistTimeMeasure();
};


struct Channel
{
    Channel(int number, Page *pSettings, PageModes *pModes, Switch *switchModeFrequency, Switch *switchModeCountPulse,
        Switch *switchModePeriod, Switch *switchModeDuration,
        const bool *enabledMeasures, const bool *enabledModeFrequency, const bool *enabledModeCountPulse);

    static Channel *A;
    static Channel *B;
    static Channel *C;
    static Channel *D;

    static const int Count = 4;

    static void Create();

    static Channel *Current() { return current; };

    static void SetCurrent(Channel *channel) { current = channel; }
    static void SetCurrentNext();

    // ��������� ��������� �������� ������ � FPGA
    static void LoadCurrentToFPGA(bool openSettings, bool openMode);

    void DrawMode(int x, int y);

    void DrawSettings(int x, int y);

    bool IsA() const { return this == A; }
    bool IsB() const { return this == B; }
    bool IsC() const { return this == C; }
    bool IsD() const { return this == D; }

    int Number() const { return number; }

    // ��������� ����� � ����������
    void LoadToFPGA();

    // ���������� ��� ���������� ���� ���������
    void OnChanged_TypeMeasure();

    void PressSetup();

    Page      *pageSettings;
    PageModes *pageModes;

    SettingsChannel set;
    ModesChannel    mod;

    // ���������� true, ���� �� ������ ������ �������� ��������� type � ������ mode
    bool ExistMeasure(TypeMeasure::E type, int mode);

private:

    static Channel *current;                // ������� �����

    int number;

    String GetSettings();
};

#include "defines.h"
#include "FreqMeter.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/MenuItemsDef.h"

using namespace Display::Primitives;
using namespace Display;
extern Item *items[7];
static bool onceLaunch = false;

DisplayTime         PageIndication::displayTime(DisplayTime::_100ms);
RefGenerator        PageIndication::refGenerator(RefGenerator::Internal);
LaunchSource        PageIndication::launchSource(LaunchSource::Internal);
Calibration         PageIndication::calibration(Calibration::Unpressed);
MemoryMode          PageIndication::memoryMode(MemoryMode::On);



static void OnPress_DisplayTime()
{
    FreqMeter::LoadDisplayTime();
}

// ����� ������� ����������� ���������� ���������
DEF_SWITCH_3(sDisplayTime,
    "�-���", "����� ���������",
    "0.1s.", "1s.", "10s.",
    PageIndication::displayTime, OnPress_DisplayTime);


static void OnPress_RefGenerator()
{
    FreqMeter::LoadRefGenerator();
}

// ����� �������� ����������
DEF_SWITCH_2(sRefGenerator,
    "��", "����� �������� ����������",
    "����������", "�������", 
    PageIndication::refGenerator, OnPress_RefGenerator);


static void OnPress_LaunchSource()
{
    FreqMeter::LoadLaunchSource();
}

// ����� ��������� �������
DEF_SWITCH_3(sLaunchSource,
    "���", "����� ��������� �������",
    "����������", "�������", "�����������",
    PageIndication::launchSource, OnPress_LaunchSource);

static void OnPress_Calibration()
{
    FreqMeter::LoadCalibration();
    if(PageIndication::calibration == Calibration::Pressed)
    {
        FPGA::ReadCalibNumber();
    }
}

// ���� � ����� ����������
DEF_SWITCH_2(sCalibration,
    "�����", "",
    "�� ������", "������", 
    PageIndication::calibration, OnPress_Calibration);


static void OnPress_MemoryMode()
{
    FreqMeter::LoadMemoryMode();
}

bool PageIndication::OnceLaunch()
{
    return onceLaunch;
}

void PageIndication::OnceLaunchSwitchTrue()
{
        onceLaunch = true;
}

void PageIndication::OnceLaunchSwitchFalse()
{
        onceLaunch = false;
}

// ���������(����������) ������ ������
DEF_SWITCH_2(sMemoryMode,
    "������", "����� ������",
    "����", "���", 
    PageIndication::memoryMode, OnPress_MemoryMode);

static Item *items[7] =
{
    &sDisplayTime,
    &sRefGenerator,
    &sLaunchSource,
    &sCalibration,
    &sMemoryMode,
    nullptr,
    nullptr
};

static Page pageIndication(items);

Page *PageIndication::self = &pageIndication;

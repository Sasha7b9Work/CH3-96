#include "defines.h"
#include "PageChannelA.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;


/// ����� ���������(���������) ����� �������� ������
static Button bInputChoice("~");

/// ��������� �������� ������������� �������� ������
static Button bImpedance("R��");

/// ���������(����������) ���
static Button bLowpassFilter("���");

/// ����� ������ ������������� �������� ������
static Button bFront(" ");

/// ���������(����������) �������� �������� ����������
static Button bVoltageDivider("1/1 1/10");

/// ����� ������ ������������� ���, ���
static Button bSync("�����");

static Item *items[] =
{
    &bInputChoice,
    &bImpedance,
    &bLowpassFilter,
    &bFront,
    &bVoltageDivider,
    &bSync,
    nullptr
};

static Page pageChannelA(items);

Page *PageChannelA::self = &pageChannelA;

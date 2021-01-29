#pragma once
#include "Utils/String.h"


struct ValueFPGA
{
    ValueFPGA(uint counter1, uint counter2 = 0, uint counter3 = 0, uint counter4 = 0);
    virtual ~ValueFPGA() {};

    String value;

    String mainUnits;

    // ���������� ������� ��� ������ ��������� ��� ������� �������. order == 3 - ����, order == -3 - �����
    char *GetSuffixUnit(int order) const;

    int NumDigitsInNumber(uint number) const;
};

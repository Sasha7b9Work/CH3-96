#pragma once
#include "Utils/String.h"


struct ValueSTRICT
{
    explicit ValueSTRICT(double v);

    void FromDouble(double v);

    double ToDouble() const;
    uint64 ToUINT64() const { return value; }

    void Div(uint div);
    void Mul(uint mul);

    void SetSign(int sign);

    // ���������� ����
    int Sign() const;

    uint64 Abs() const;

private:

    uint64 value;       // �������� ��������� � �������� ��������� "����". ������������� � "1" ������� ��� ��������, ��� ����� �������������
};


struct ValueComparator //-V690
{
    explicit ValueComparator(int v);

    void FromUNITS(int units, uint mUnits, uint uUnits, uint nUnits, uint pUnits, int sign);
    void FromINT(int v);

    void Div(uint div);
    void Mul(uint mul);

    void Add(ValueComparator &value);
    void Sub(const ValueComparator &value);

    int Sign() const;
    void SetSign(int sign);

    // ��������� ������ ��������
    String ToString() const;

    double ToDouble() const;

    uint64 Abs() const;

    int Integer() const;

    uint64 FractPico() const;

private:

    uint64 value;       // �������� ��������� � �������� ��������� "����". ������������� � "1" ������� ��� ��������, ��� ����� �������������
};


ValueComparator operator/(const ValueComparator &first, uint second);
ValueComparator operator-(const ValueComparator &first, const ValueComparator &second);
ValueComparator operator-(const ValueComparator &first, int second);
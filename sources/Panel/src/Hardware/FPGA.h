#pragma once
#include "defines.h"
#include "Utils/Stack.h"


struct FPGA
{
friend struct MathFPGA;

    static void Init();
    static void Update();
    static void WriteCommand(const char command[4], const char argument[6]);
    static void WriteData();
    static void IncreaseN();
    static void DecreaseN();
    static void SwitchAuto();
    static bool AutoMode();
    static int CalibNumber();
    static void ReadCalibNumber();

    // ���������� true � ������ ������������
    static bool IsOverloaded();

    struct Comparator
    {
        class Stack : public ::Stack<double>
        {
        public:
            Stack(int size) : ::Stack<double>(size) {};
            bool AppendValue(double value);
            double GetFromEnd(int fromEnd);
        private:
        };

        static Stack values;
    };

private:

    static void ReadInterpolator();
    static void ReadAutoMode();

    static uint fpgaIdent;
};

#pragma once


/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/


class String;

typedef pchar (*FuncSCPI)(pchar);


// ���������, �������������� ���� ������.
struct StructSCPI
{
    pchar key;            // �������� ����� ���� (�������)

    const StructSCPI *strct;    // ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             // ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   // ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
    bool IsLeaf() const { return func != nullptr; };    // ��������� �������� "������" ������, ����� ��������� ������� func
};


#define SCPI_NODE(key, strct)   {key, strct,   nullptr}
#define SCPI_LEAF(key, func) {key, nullptr, func}
#define SCPI_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG_REQUEST(t) return t; } return nullptr;
#define SCPI_EPILOG(t)         return t; }

#define SCPI_RUN_IF_END_REQUEST(func) if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG_REQUEST(end) }
#define SCPI_RUN_IF_END(func)         if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG(end) }

#define SCPI_REQUEST(func)                          \
    pchar end = SCPI::BeginWith(buffer, "?");       \
    SCPI_RUN_IF_END_REQUEST(func)

#define SCPI_PROCESS_ARRAY(names, func)             \
    for(int i = 0; i < names[i][0] != 0; i++)       \
    {                                               \
        end = SCPI::BeginWith(buffer, names[i]);    \
        SCPI_RUN_IF_END(func)                       \
    }                                               \
    return nullptr;

#define SCPI_EXIT_ERROR()   LOG_WRITE("������ ����� SCPI %s:%d", __FILE__, __LINE__); return false;

#define SCPI_APPEND_STRING(string) SCPI::AppendNewData(string.c_str(), static_cast<int>(std::strlen(string.c_str()))); SCPI::Update()

#define SCPI_IF_BEGIN_WITH_THEN(begin, func) end = SCPI::BeginWith(buffer, begin); if (end) { SCPI_PROLOG(end); func; SCPI_EPILOG(end); }


// ������� ��������� ��� ��������� �������� ���������
struct SetSCPI
{
    virtual ~SetSCPI() {}

    void Set(int i) const;

    virtual void SetParameter(int, int) const {};         // �������������� ������ �������� ����� ��� ����, ����� �������� �� ������� ��������
};


namespace SCPI
{
    // ������-��������� ������ �������
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(pchar buffer, int length);

    void Update();
    
    // ���������� true, ���� ��������� ��������� �� ����������� ������������������
    bool IsLineEnding(pchar *bufer);
    
    // ������� ����� � � ����� �������� 0x0D, ���� ���
    void SendAnswer(pchar message);
    void SendAnswer(const String &message);

    // ������� ������ ��� ���� - ��� ������������ �������
    void SendData(pchar message);
    void SendData(const String &message);

    // ������� ��������� � SCPI - � ������� ���������� �������� � ������ ���������
    void SendMeasure(const String &message);
    
    // ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� ��
    // ��������� �������� ������������������ word.
    // ����� ���������� nullptr.
    const char *BeginWith(pchar buffer, pchar word);
    
    // ������� ��������� �� ��������� ��������, ���� ������� �������
    void SendBadSymbols();

    void ProcessHint(String *message, pchar const *names); //-V2504

    namespace Answer
    {
        void CurrentChannelHasNotParameter();
    }

    // ����� ������� ��������� ������� �������� �������
    pchar ProcessSimpleParameter(pchar buffer, const pchar choice[], uint8 value, const SetSCPI &set);

    // ����� ������� ��� �������� ������ �� ��������� ����� �������
    void AnswerInput(const pchar choice[], uint8 value);
};


namespace SCPI
{
    extern const StructSCPI head[];         //-V2504
    extern const StructSCPI input[]; //-V2504
}

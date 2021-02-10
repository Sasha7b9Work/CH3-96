#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Process/DataSender.h"


static pchar FuncMode(pchar);
static pchar FuncValue(pchar);
static pchar FuncArray(pchar);


const StructSCPI SCPI::data[] =
{
    SCPI_LEAF(":MODE", FuncMode),
    SCPI_LEAF(":VALUE?", FuncValue),
    SCPI_LEAF(":ARRAY",  FuncArray),
    SCPI_EMPTY()
};


static pchar FuncMode(pchar buffer)
{
    static const pchar mode[] =
    {
        " HAND",
        " AUTO",
        ""
    };

    pchar end = nullptr;

    SCPI_PROCESS_ARRAY(mode, SCPI::DataSender::SetMode((SCPI::DataSender::Mode::E)i));
}


static pchar FuncValue(pchar buffer)
{
    SCPI_PROLOG(buffer);

    SCPI::DataSender::SendArray(1);

    SCPI_EPILOG(buffer);

    return nullptr;
}


static pchar FuncArray(pchar buffer)
{
    return nullptr;
}

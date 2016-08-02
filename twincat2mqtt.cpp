#include <iostream>

#include "AdsLib.h"
#include "MqClient.h"
#include "TwincatVar.h"

using namespace std;

static const AmsNetId remoteNetId { 192, 168, 0, 231, 1, 1 };
static const char remoteIpV4[] = "192.168.0.232";

//returns a port
long plc_connect()
{

    // add local route to your EtherCAT Master
    if (AdsAddRoute(remoteNetId, remoteIpV4))
    {
        std::cout << "Adding ADS route failed, did you specified valid addresses?\n";
        return 0;
    }

    // open a new ADS port
    const long port = AdsPortOpenEx();
    if (!port)
    {
        std::cout << "Open ADS port failed\n";
        return 0;
    }
    return port;
}

bool plc_disconnect(long port)
{
    const long closeStatus = AdsPortCloseEx(port);
    if (closeStatus)
    {
        std::cout << "Close ADS port failed with: " << std::dec << closeStatus << '\n';
        return false;
    }
    return true;
}


int main()
{

    const AmsAddr remote { remoteNetId, AMSPORT_R0_PLC_TC3 };

    const long port=plc_connect();

    TwincatVar<bool>  miavar("MAIN.Bool1",remote,port);

    miavar.getValue();

    plc_disconnect(port);

    //publish to mqtt broker (example)
    MqClient mq("test from c++","127.0.0.1");

    std::string message=miavar.getValue()?"TRUE":"FALSE";
    mq.strpublish("plcvars/Bool1",message);

    mq.disconnect();

}

#include <iostream>

#include "AdsLib.h"
#include "MqClient.h"
#include "TwincatVar.h"

using namespace std;


//returns a port if everything goes well
//returns 0 (zero) if error
long plc_connect(const AmsNetId& remote_id, const char* ipV4)
{

    // add local route to your EtherCAT Master
    if (AdsAddRoute(remote_id, ipV4))
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
    const AmsNetId remoteNetId { 192, 168, 0, 231, 1, 1 };
    const char remoteIpV4[] = "192.168.0.232";

    const char* mqBrokerAddress="127.0.0.1";

    const AmsAddr remote { remoteNetId, AMSPORT_R0_PLC_TC3 };

    const long port=plc_connect(remoteNetId,remoteIpV4);

    TwincatVar<bool>  miavar("MAIN.Bool1",remote,port);

    miavar.getValue();


    //publish to mqtt broker (example)
    MqClient mq("test from c++",mqBrokerAddress);

    std::string message;
    //main loop : until the user press ENTER
    do
    {
        message=miavar.getValue()?"TRUE":"FALSE";
        mq.strpublish("plcvars/Bool1",message);
    }
    while (cin.get()!='\n');


    mq.disconnect();
    plc_disconnect(port);


}

#include "stdlib.h"
#include "ns3/core-module.h"
#include "ns3//point-to-point-module.h"
#include "ns3-application.h"

using namespace ns3;

void MCLHandler(Ptr<es::ApplicationWrapper> app, const std::string& feedId){
    app->getApp()->createMCL(feedId);
};

void SubHandler(Ptr<es::ApplicationWrapper> app, const std::string& feedId){
    app->getApp()->subscribe(feedId);
};

void hanlder1(Ptr<es::ApplicationWrapper> app){
    std::vector<uint8_t> payload;
    payload.push_back((uint8_t) 'c');

    payload.push_back((uint8_t) 'A');

    
    app->getApp()->sendPkt(payload);
};

int main(int argc, char *argv[])
{
    NS_LOG_COMPONENT_DEFINE("EntropySwitching");

    CommandLine cmd;
    cmd.Parse(argc, argv);
    
    Ptr<Node> n0 = Create<Node>();
    Ptr<Node> n1 = Create<Node>();
    Ptr<Node> n2 = Create<Node>();
    Ptr<Node> n3 = Create<Node>();
    Ptr<Node> n4 = Create<Node>();
    Ptr<Node> n5 = Create<Node>();
    Ptr<Node> n6 = Create<Node>();
    Ptr<Node> n7 = Create<Node>();
    Ptr<Node> n8 = Create<Node>();
    NodeContainer all;
    all.Add(n0);
    all.Add(n1);
    all.Add(n2);
    all.Add(n3);
    all.Add(n4);
    all.Add(n5);
    all.Add(n6);
    all.Add(n7);
    all.Add(n8);
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    /*
              n7 -- n8       n5
              |              |
        n0 -- n1 -- -- -- -- n4
              |              |
              n2 -- n3       n6
    */

    NetDeviceContainer devices;
    devices.Add(pointToPoint.Install(n1, n0));
    devices.Add(pointToPoint.Install(n1, n2));
    devices.Add(pointToPoint.Install(n1, n4));
    devices.Add(pointToPoint.Install(n1, n7));
    devices.Add(pointToPoint.Install(n2, n3));
    devices.Add(pointToPoint.Install(n4, n5));
    devices.Add(pointToPoint.Install(n4, n6));
    devices.Add(pointToPoint.Install(n7, n8));

    for (size_t i = 0; i < devices.GetN(); i++)
    {
        std::cout << "Address: " << devices.Get(i)->GetAddress() << std::endl;
    }
    

    Ptr<es::ApplicationWrapper> ap[all.GetN()];

    for (uint32_t i = 0; i < all.GetN(); ++i)
    {
        ap[i] = Create<es::ApplicationWrapper>();
        all.Get(i)->AddApplication(ap[i]);
    }

   
    
    Simulator::Schedule(Seconds(2), &MCLHandler, ap[0], "SomeNews");
    Simulator::Schedule(Seconds(2), &MCLHandler, ap[1], "FakeNews");
    for (uint32_t i = 1; i < all.GetN(); ++i)
    {
        if (i >= 2) {
            Simulator::Schedule(Seconds(3), &SubHandler, ap[i], "FakeNews");
        }

        Simulator::Schedule(Seconds(3), &SubHandler, ap[i], "SomeNews");
        
    }
    Simulator::Run ();
    Simulator::Destroy();
    return 0;
}

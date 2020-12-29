


#include "PCAP.h"
#include "cbor.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/v4ping-helper.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/bridge-helper.h"
#include "ns3/ipv4-routing-protocol.h"
#include "ns3/netanim-module.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("HidinTest");

int main (int argc, char *argv[]) {
	testingStuff();
	std::cout << "Hello World" << std::endl;
	PCAP pcap("entropy-data/hans.pcap", 0);
	pcap.open('a', 0);
	pcap.write("/data/hidin");
	pcap.close();
}

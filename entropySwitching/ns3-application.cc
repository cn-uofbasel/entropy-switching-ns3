#include "util.h"
#include "ns3-application.h"

using namespace ns3;

namespace es
{
    ApplicationWrapper::ApplicationWrapper() : ns3::Application()
    {
    }

    ApplicationWrapper::~ApplicationWrapper()
    {
    }

    void ApplicationWrapper::recvPkt(Ptr<NetDevice> dev, Ptr<const Packet> pkt, uint16_t proto, const Address &from, const Address &to, NetDevice::PacketType pt)
    {

        int size = pkt->GetSize();
        uint8_t buf[size];
        memset(buf, 0, size);
        pkt->CopyData(buf, size);
        std::vector<uint8_t> vecBuf(buf, buf + (sizeof(buf) / (sizeof(buf[0]))));
        if (!this->verbose)
        {
            printf("[ns3App](id=%i) recvPkt(%i bytes): ", GetNode()->GetId(), size);
            printBytes(vecBuf);
        }

        app->recvPkt(vecBuf);
    }

    void ApplicationWrapper::sendPkt(std::vector<uint8_t> content)
    {

        if (!this->verbose)
        {
            printf("[ns3App](id=%i) sendPkt(%li bytes): ", GetNode()->GetId(), content.size());
            printBytes(content);
        }
        uint8_t *packedContent = content.data();

        for (uint32_t i = 0; i < GetNode()->GetNDevices(); ++i)
        {
            Ptr<Packet> p = Create<Packet>((uint8_t *)packedContent, content.size());
            auto addr = ns3::Mac48Address("FF:FF:FF:FF:FF:FF");
            GetNode()->GetDevice(i)->Send(p, addr, 0x800);
        }
    }

    void ApplicationWrapper::StartApplication()
    {
        this->app = new es::Application(isVerbose());
        this->app->setSendCallback(std::bind(&ApplicationWrapper::sendPkt, this, std::placeholders::_1));
        for (uint32_t i = 0; i < GetNode()->GetNDevices(); ++i)
        {
            GetNode()->RegisterProtocolHandler(MakeCallback(&ApplicationWrapper::recvPkt, this), 0x800, GetNode()->GetDevice(i));
        }
    }

    bool ApplicationWrapper::isVerbose()
    {
        return this->verbose;
    }
} 
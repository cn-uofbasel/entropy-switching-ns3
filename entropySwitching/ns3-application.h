#ifndef NS3APP_H
#define NS3APP_H


#include "util.h"
#include <vector>
#include <iostream>
#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "es-application.h"
#include <functional>
#include <sstream>
#include <iterator>


using namespace ns3;

namespace es
{
    class ApplicationWrapper : public ns3::Application
    {
    private:
        es::Application *app;
        static void *callbackWrap(void *context);
        bool verbose = true;

    public:
        ApplicationWrapper();

        ~ApplicationWrapper();
        
        void recvPkt(Ptr<NetDevice>, Ptr<const Packet>, uint16_t proto, const Address &from, const Address &to, NetDevice::PacketType pt);

        void sendPkt(std::vector<uint8_t> content);
        
        virtual void StartApplication(void);

        es::Application* getApp() {
            return this->app;
        }

        bool isVerbose();
    };
} // namespace es

#endif
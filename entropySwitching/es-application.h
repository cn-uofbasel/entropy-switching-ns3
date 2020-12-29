#ifndef ESAPP_H
#define ESAPP_H

#include "util.h"
#include <vector>
#include <cstring>
#include <iostream>
#include <functional>
#include <sstream>
#include <iterator>
#include <map>
#include "es-log.h"
#include "json.hpp"

namespace es
{
    using json = nlohmann::json;
    class Application
    {
    private:
        std::function<void(std::vector<uint8_t>)> send_callback;
        bool verbose;
        std::map<std::string, Log*> logMap;
        std::vector<std::string> subscribed;
        //TODO: LOG
        //NODE ABSTRACTION

    public:
        Application(bool isVerbose);
        ~Application();
        void recvPkt(std::vector<uint8_t> content);
        void sendPkt(std::vector<uint8_t> content);
        void setSendCallback(std::function<void(std::vector<uint8_t>)>);
        void subscribe(std::string feedId);
        void unsubscribe(std::string feedId);
        void createMCL(std::string feedId);
        void sendFrontier(std::string feedId, ulong seqNo);
        void handleFrontier(json packet);
    };
} // namespace es

#endif

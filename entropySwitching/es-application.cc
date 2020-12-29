#include "es-application.h"
#include "es-log.h"
#include "es-mcl.h"

namespace es
{

    Application::Application(bool isVerbose = true)
    {
        this->verbose = true;
    }

    Application::~Application()
    {
    }

    void Application::recvPkt(std::vector<uint8_t> content)
    {
        if (this->verbose)
        {

            std::string recvJson(content.begin(), content.end());
            using json = nlohmann::json;
            json recvPkt = json::parse(recvJson);

            std::string pktType = recvPkt["pkt_type"].get<std::string>();
            printf("[esApp]\t(%li) recvPkt: %s\n", content.size(), pktType.c_str());

            if (pktType == "frontier")
            {
                this->handleFrontier(recvPkt);
            }
        }
    }
    void Application::sendPkt(std::vector<uint8_t> content)
    {
        if (this->send_callback != nullptr)
        {

            if (this->verbose)
            {
                printf("[esApp]\t(%li) sendPkt\n", content.size());
                // printBytes(content);
            }
            this->send_callback(content);
        }
        else
        {
            printf("Callback was not set.");
        }
    }
    void Application::setSendCallback(std::function<void(std::vector<uint8_t>)> callback)
    {
        this->send_callback = callback;
    }

    void Application::createMCL(std::string feedId)
    {
        if (feedId.size() > 0)
        {
            if (!(this->logMap.find(feedId) != this->logMap.end()))
            {
                Log *test = new MCL(feedId);
                this->logMap[feedId] = test;
                this->sendFrontier(feedId, this->logMap[feedId]->getSeqNo());
            }
        }
    }

    void Application::subscribe(std::string feedId)
    {
        if (!(std::find(this->subscribed.begin(), this->subscribed.end(), feedId) != this->subscribed.end()))
        {
            this->subscribed.push_back(feedId);
            this->sendFrontier(feedId, 0);
        }
    }
    void Application::unsubscribe(std::string feedId)
    {
        std::vector<std::string>::iterator pos = std::find(this->subscribed.begin(), this->subscribed.end(), feedId);
        if (pos != this->subscribed.end())
        {
            this->subscribed.erase(pos);
        }
    }

    void Application::sendFrontier(std::string feedId, ulong seqNo)
    {
        using json = nlohmann::json;
        json frontierPkt;
        frontierPkt["pkt_type"] = "frontier";
        frontierPkt["feed_id"] = feedId;
        frontierPkt["seq_no"] = seqNo;
        std::string frontierJson = frontierPkt.dump();
        std::vector<uint8_t> payload(frontierJson.begin(), frontierJson.end());
        this->sendPkt(payload);
    }

    void Application::handleFrontier(json packet)
    {
        std::string feedId = packet["feed_id"].get<std::string>();
        std::vector<std::string>::iterator pos = std::find(this->subscribed.begin(), this->subscribed.end(), feedId);
        if (pos != this->subscribed.end())
        {
            if (this->logMap.find(feedId) != this->logMap.end())
            {
                ulong mySeqNo = this->logMap[feedId]->getSeqNo();
                ulong otherSeqNo = packet["seq_no"].get<ulong>();
                if (mySeqNo < otherSeqNo) {
                    //TODO REQUEST INFO;
                } else if (mySeqNo > otherSeqNo) {
                    //TODO SendUpdate
                } else {
                    //TODO Do nothing
                }
            }
        }
    }

} // namespace es
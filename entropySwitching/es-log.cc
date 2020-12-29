#include "es-log.h"

namespace es
{

    Log::Log(std::string feedId)
    {
        this->feedId = feedId;
        this->seqNo = 0;

        std::string initPayload = "create/";
        std::vector<uint8_t> initPayloadVec(initPayload.begin(), initPayload.end());
        Event *e = new Event(feedId, this->seqNo, std::vector<uint8_t>(feedId.begin(), feedId.end()), initPayloadVec);
        append(e);
    }

    Log::~Log()
    {
    }

    void Log::append(Event *event)
    {
        this->seqNo++;
        events.push_back(event);
    }

    void Log::append(std::vector<u_char> payload)
    {
        Event *oldEvent = events.back();

        uint8_t *verif1 = oldEvent->getVerif1();
        Event *e = new Event(feedId, this->seqNo, verif1, payload);
        this->append(e);
    }

    void Log::append(std::string payload)
    {
        this->append(std::vector<uint8_t>(payload.begin(), payload.end()));
    }

    void Log::drop(Event event)
    {
        //????
    }

    void Log::prune()
    {
        events.clear();
        this->seqNo = 0;
    }

    nlohmann::json Log::getJson()
    {
        using json = nlohmann::json;
        json logJson;
        logJson["feedId"] = this->feedId.c_str();
        std::list<Event *>::iterator it;
        for (it = this->events.begin(); it != this->events.end(); ++it)
        {
            Event *e = (*it);
            ulong seqNo = e->getSeqNo();
            logJson["event-" + std::to_string(seqNo)] = e->getJson();
        }
        return logJson;
    }

    std::string Log::serialize()
    {
        return this->getJson().dump();
    }

    ulong Log::getSeqNo() {
        return this->seqNo;
    }

} // namespace es
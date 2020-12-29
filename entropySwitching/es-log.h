#ifndef ESLOG_H
#define ESLOG_H
#include <list>
#include <string>
#include "es-event.h"

namespace es
{
    class Log
    {

    private:
        std::list<Event*> events;
        std::string feedId;
        ulong seqNo;

    public:
        void append(Event *event);
        void append(std::vector<u_char> payload);
        void append(std::string payload);
        void drop(Event event);
        void prune();
        Log(std::string feedId);
        std::string serialize();
        nlohmann::json getJson();
        ulong getSeqNo();
        ~Log();
    };
} // namespace es

#endif
#ifndef ESFRONTIER_H
#define ESFRONTIER_H
#include <string>
#include "es-event.h"
#include <ctime>
namespace es
{
    class Frontier
    {

    private:
        std::string feedId;
        ulong seqNo;
        // std::time_t rcvWindow; //Todo USE

    public:
        Frontier(std::string feedId, ulong seqNo);
    };
} // namespace es

#endif
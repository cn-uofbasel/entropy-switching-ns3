#ifndef ESEVENT_H
#define ESEVENT_H

#include "util.h"
#include "es-meta.h"

// #include <vector>
// #include <cstring>
// #include <iostream>
// #include <functional>
// #include <sstream>
// #include <iterator>
// #include <list>

namespace es
{
    class Event
    {
    private:
        Meta *meta;
        std::vector<u_char> payload;
        uint8_t* verif2;

        

    public:
        Event(std::string feedId, ulong seqNo, std::vector<u_char> verif1, std::vector<u_char> &payload);
        Event(std::string feedId, ulong seqNo, u_char* verif1_hash, std::vector<u_char> payload);
        std::string serialize();
        nlohmann::json getJson();
        ulong getSeqNo();
        uint8_t* getVerif1();
        ~Event();
    
        
       
    };
} // namespace es

#endif

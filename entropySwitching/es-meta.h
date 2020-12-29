#ifndef ESMETA_H
#define ESMETA_H


#include <string>
#include <vector>

#include "json.hpp"
#include "util.h"

namespace es
{
    class Meta
    {
    private:
        std::string source;
        ulong seqNo;
        uint8_t verif1[32];
        uint8_t payloadHash[32];
        

    public:
        Meta(std::string source, ulong seqNo, u_char *verif1, u_char *payloadHash);
        std::string serialize();
        nlohmann::json getJson();
        ulong getSeqNo();
        uint8_t* getVerif1();
        ~Meta();
    
        
       
    };
} // namespace es


#endif

#include "es-event.h"
#include "sha3.h"
#include <iostream>
#include <string>
namespace es
{
    Event::Event(std::string feedId, ulong seqNo, std::vector<u_char> verif1, std::vector<u_char> &payload)
    {
        this->payload = payload;
        uint8_t verif1_hash[32];
        sha3_HashBuffer(256, SHA3_FLAGS_KECCAK, &verif1[0], verif1.size(), verif1_hash, sizeof(verif1_hash));
        uint8_t payload_hash[32];
        sha3_HashBuffer(256, SHA3_FLAGS_KECCAK, &payload[0], payload.size(), payload_hash, sizeof(payload_hash));
        this->meta = new Meta(feedId, seqNo, verif1_hash, payload_hash);
        std::string metaSerialized = this->meta->serialize();
        uint8_t verif2_hash[32];
        sha3_HashBuffer(256, SHA3_FLAGS_KECCAK, metaSerialized.c_str(), metaSerialized.length(), verif2_hash, sizeof(verif2_hash));
        this->verif2 = verif2_hash;
    }

     Event::Event(std::string feedId, ulong seqNo, u_char* verif1_hash, std::vector<u_char> payload)
    {
        this->payload = payload;
        uint8_t payload_hash[32];
        sha3_HashBuffer(256, SHA3_FLAGS_KECCAK, &payload[0], payload.size(), payload_hash, sizeof(payload_hash));
        this->meta = new Meta(feedId, seqNo, verif1_hash, payload_hash);

        
        std::string metaSerialized = this->meta->serialize();
        uint8_t verif2_hash[32];
        sha3_HashBuffer(256, SHA3_FLAGS_KECCAK, metaSerialized.c_str(), metaSerialized.length(), verif2_hash, sizeof(verif2_hash));
        this->verif2 = verif2_hash;
    }

    Event::~Event()
    {
    }

    std::string Event::serialize() {
        return this->getJson().dump();
    }

    nlohmann::json Event::getJson() {
        using json = nlohmann::json;
        json metaJson = this->meta->getJson();
        json eventJson;
        eventJson["meta"] = metaJson;
        eventJson["verif2"] = std::vector<uint8_t>(&this->verif2[0], &this->verif2[32]);
        eventJson["payload"] = std::string(this->payload.begin(), this->payload.end());
        return eventJson;
    }

    ulong Event::getSeqNo() {
        return this->meta->getSeqNo();
    }


    uint8_t* Event::getVerif1() {
        return this->meta->getVerif1();
    }

} // namespace es
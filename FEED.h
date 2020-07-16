#ifndef FEED_H
#define FEED_H


#include <cstring>
#include <string>
#include <vector>
#include "EVENT.h"
#include "PCAP.h"
#include "sha3.h"



class FEED {
    private:
        std::string feedName;
        std::vector<u_char> fid;
        std::string hmacKey;
        bool cine = false;
        int seq = 0;
        PCAP* pcap;
        std::string metasig;
        bool corrupt;
        int lastPos;
        struct prevS{
            std::vector<u_char> hash;
            std::vector<u_char> homs;
        };
        prevS prev;
    
    public:
        FEED(std::string feedName, const std::vector<u_char> fid = std::vector<u_char>(), std::string hmacKey = "", bool cine = false) {
            this->feedName = feedName;
            this->fid = fid;
            this->hmacKey = hmacKey;
            this->cine = cine;
            this->pcap = new PCAP(feedName);
            this->metasig = "";
            this->corrupt = true;

            this->pcap->open('r');
            std::vector<u_char> w;
            //TRY?
            this->pcap->read_backwards(w, true);
            if (w.size() == 0) {
                this->pcap->close();
                if (this->cine == true) {
                    this->pcap->open('w');
                } 
            } else {
                //TODO EVENT STUFF
                EVENT *e = new EVENT(w);
                if (!fid.empty() && e->getFid() != fid) {
                    printf("pcap mismatch: instead of\n");
                }
                this->fid = e->getFid();
                this->seq = e->getSeq();
                // this->prev.hash = sha3_HashBuffer(256, SHA3_FLAGS_NONE, e->getMetaBits(), )
                this->prev.homs = e->getHoms();
                std::vector<u_char> w2;
                this->pcap->getItem(w2, this->seq - 1);
                if (w2 != w) {
                    printf("   offset file corrupt in %s\n", this->feedName.c_str());
                }
            }
            //CATCH? 

            this->lastPos = -1;
            this->corrupt = false;
        }

        ~FEED() {

        }

};

#endif
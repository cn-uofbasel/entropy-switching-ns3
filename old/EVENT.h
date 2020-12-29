#ifndef EVENT_H
#define EVENT_H


#include <vector>
#include <fstream>

class EVENT {
    private:
        std::vector<u_char> fid;
        std::vector<u_char> wire;
        std::vector<u_char> sopm; //TODO unknown
        std::vector<u_char> homs;
        std::vector<u_char> metabits;
        int seq;
    public:
        EVENT(std::vector<u_char> w,std::vector<u_char> fid, std::vector<u_char> sopm, std::vector<u_char> homs) {
            this->wire = w;
            if (w.empty()) {
                this->fid = fid;
                this->seq = 1;
                this->sopm = sopm;
                this->homs = homs;

            } else {

            }
        }

        EVENT(std::vector<u_char> w, int seq = 1) {
            this->wire = w;
            if (w.empty()) {
                this->fid = std::vector<u_char>();
                this->seq = seq;
                this->sopm = std::vector<u_char>();
                this->homs = std::vector<u_char>();
                //todo this->contbits = serialize();
            } else {
                
            }
        }

        std::vector<u_char> getFid() {
            return this->fid;
        }

        int getSeq() {
            return this->seq;
        }

        std::vector<u_char> getHoms() {
            return this->homs;
        }
};

#endif
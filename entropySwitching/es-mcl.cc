#include "es-mcl.h"

namespace es {
    MCL::MCL(std::string feedId) : Log(feedId){
    }

    void MCL::admit(std::string feedId) {
        this->append("admit/" + feedId);
    }

    void MCL::cease(std::string feedId) {
        this->append("cease/" + feedId);
    }

}
#ifndef ESMCL_H
#define ESMCL_H
#include "es-log.h"

namespace es {
    class MCL : public Log {
    
    private:
        

    public:
  
        MCL(std::string feedId);
        void admit(std::string feedId);
        void cease(std::string feedId);
    };
}


#endif
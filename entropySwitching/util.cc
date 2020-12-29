#include "util.h"

void printBytes(std::vector<uint8_t> toPrint) {
    
    printf("b\'");
    for (int i = 0; i < (int) toPrint.size(); i++) {
        printf("\\x%02hhx", toPrint[i]);
    }
    printf("\'\n");
}
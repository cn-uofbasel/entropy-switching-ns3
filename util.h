#ifndef UTIL_H
#define UTIL_H


#include <vector>
#include <string>

//returns 0 on success. -1 on failure;
int byteToInt(int &ret, u_char* bytes, std::string endianness) {
    int N = (int) sizeof(int);
    if (endianness == "big") {
        for (int i = 0; i < N; i++) {
            ret |= bytes[N-1-i] << (i * 8);
        }
    } else if (endianness == "little") {
        for (int i = 0; i < N; i++) {
            ret |= bytes[i] << (i * 8);
        }
    } else {
        return -1;
    }
    return 0;
} 

int byteToInt(int &ret, std::vector<u_char> bytes, std::string endianness) {
    int N = (int) sizeof(int);
    //TODO: Handle Size Mismatch
    if (endianness == "big") {
        for (int i = 0; i < N; i++) {
            ret |= bytes[N-1-i] << (i * 8);
        }
    } else if (endianness == "little") {
        for (int i = 0; i < N; i++) {
            ret |= bytes[i] << (i * 8);
        }
    } else {
        return -1;
    }
    return 0;
}

std::vector<u_char> intToByte(int num, std::string endianness) {
    std::vector<u_char> ret;
    ret.reserve(4);
    if (endianness == "big") {
        for (int i = 0; i < 4; i++) {            
            ret.push_back((num >> ((3 - i) *8)) & 0xFF);
        }   
     
    } else if (endianness == "little") {
        for (int i = 0; i < 4; i++) {            
            ret.push_back((num >> (i *8)) & 0xFF);
        }   
    } else {
        //TODO Handle Stuff
        return ret;
    }
    return ret;
}

std::vector<u_char> ulongToByte(ulong num, std::string endianness) {
    std::vector<u_char> ret;
    ret.reserve(8);
    if (endianness == "big") {
        for (int i = 0; i < 4; i++) {            
            ret.push_back((num >> ((3 - i) *8)) & 0xFF);
        }   
     
    } else if (endianness == "little") {
        for (int i = 0; i < 4; i++) {            
            ret.push_back((num >> (i *8)) & 0xFF);
        }   
    } else {
        //TODO Handle Stuff
        return ret;
    }
    return ret;
}

std::vector<u_char> uintToByte(uint num, std::string endianness) {
    std::vector<u_char> ret;
    ret.reserve(4);
    if (endianness == "big") {
        for (int i = 0; i < 4; i++) {            
            ret.push_back((num >> ((3 - i) *8)) & 0xFF);
        }   
     
    } else if (endianness == "little") {
        for (int i = 0; i < 4; i++) {            
            ret.push_back((num >> (i *8)) & 0xFF);
        }   
    } else {
        //TODO Handle Stuff
        return ret;
    }
    return ret;
}

std::vector<u_char> ushortToByte(ushort num, std::string endianness) {
    std::vector<u_char> ret;
    ret.reserve(2);
    if (endianness == "big") {
        for (int i = 0; i < 2; i++) {            
            ret.push_back((num >> ((1 - i) *8)) & 0xFF);
        }   
     
    } else if (endianness == "little") {
        for (int i = 0; i < 2; i++) {            
            ret.push_back((num >> (i *8)) & 0xFF);
        }   
    } else {
        //TODO Handle Stuff
        return ret;
    }
    return ret;
}


void printBytes(std::vector<u_char> toPrint) {
    
    printf("b\'");
    for (int i = 0; i < (int) toPrint.size(); i++) {
        printf("\\x%02hhx", toPrint[i]);
    }
    printf("\'\n");
}

#endif
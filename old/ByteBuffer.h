#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <vector>
#include <iostream>

class ByteBuffer {

private:
    std::vector<u_char> buffer;


public:
    ByteBuffer(u_long initialValue, int endianness);

    ByteBuffer(u_int initialValue, int endianness);

    ByteBuffer(u_short initialValue, int endianness);

    ByteBuffer(u_char initialValue, int endianness);

    ByteBuffer(std::string initialValue, int endianness);

    void append(u_long toAppend, int endianness);
    
    void append(u_int toAppend, int endianness);

    void append(u_short toAppend, int endianness);

    void append(u_char toAppend, int endianness);

    void append(std::string toAppend, int endianness);

    ByteBuffer sub(int start, int end);


};

#endif
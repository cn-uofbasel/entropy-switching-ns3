#include "ByteBuffer.h"


ByteBuffer::ByteBuffer(u_long initialValue, int endianness);

ByteBuffer::ByteBuffer(u_int initialValue, int endianness);

ByteBuffer::ByteBuffer(u_short initialValue, int endianness);

ByteBuffer::ByteBuffer(u_char initialValue, int endianness);

ByteBuffer::ByteBuffer(std::string initialValue, int endianness);

void ByteBuffer::append(u_long toAppend, int endianness);
void ByteBuffer::append(u_int toAppend, int endianness);
void ByteBuffer::append(u_short toAppend, int endianness);
void ByteBuffer::append(u_char toAppend, int endianness);
void ByteBuffer::append(std::string toAppend, int endianness);

ByteBuffer ByteBuffer::sub(int start, int end, int endianness);
#define CBOR_TYPE_MASK 0xE0 // top 3 bits
#define CBOR_INFO_BITS 0x1F // low 5 bits


#define CBOR_UINT    0x00
#define CBOR_NEGINT  0x20
#define CBOR_BYTES   0x40
#define CBOR_TEXT    0x60
#define CBOR_ARRAY   0x80
#define CBOR_MAP     0xA0
#define CBOR_TAG     0xC0
#define CBOR_7       0xE0// float and other types

#define CBOR_UINT8_FOLLOWS  24 // 0x18
#define CBOR_UINT16_FOLLOWS 25 // 0x19
#define CBOR_UINT32_FOLLOWS 26 // 0x1a
#define CBOR_UINT64_FOLLOWS 27 // 0x1b
#define CBOR_VAR_FOLLOWS    31 // 0x1f

#define CBOR_BREAK  0xFF

#define CBOR_FALSE  CBOR_7 | 20
#define CBOR_TRUE   CBOR_7 | 21
#define CBOR_NULL   CBOR_7 | 22
#define CBOR_UNDEFINED   CBOR_7 | 23// js 'undefined' value

#define CBOR_FLOAT16 CBOR_7 | 25
#define CBOR_FLOAT32 CBOR_7 | 26
#define CBOR_FLOAT64 CBOR_7 | 27

#define CBOR_TAG_DATE_STRING 0 // RFC3339
#define CBOR_TAG_DATE_ARRAY  1 // any number type follows, seconds since 1970-01-01T00:00:00 UTC
#define CBOR_TAG_BIGNUM      2 // big endian byte string follows
#define CBOR_TAG_NEGBIGNUM   3 // big endian byte string follows
#define CBOR_TAG_DECIMAL     4 // [ 10^x exponent, number ]
#define CBOR_TAG_BIGFLOAT    5 // [ 2^x exponent, number ]
#define CBOR_TAG_BASE64URL   21
#define CBOR_TAG_BASE64      22
#define CBOR_TAG_BASE16      23
#define CBOR_TAG_CBOR        24 //following byte string is embedded CBOR data

#define CBOR_TAG_URI             32
#define CBOR_TAG_BASE64URL_STR   33
#define CBOR_TAG_BASE64_STR      34
#define CBOR_TAG_REGEX           35
#define CBOR_TAG_MIME            36 //following text is MIME message, headers, separators and all
#define CBOR_TAG_CBOR_FILEHEADER 55799 //can open a file with 0xd9d9f7

#define CBOR_TAG_BIGNUM_BYTES (CBOR_TAG | CBOR_TAG_BIGNUM)
#define CBOR_TAG_NEGBIGNUM_BYTES (CBOR_TAG | CBOR_TAG_NEGBIGNUM)

#define MAX_DEPTH 100

#include <cstring>
#include <iostream>
#include <vector>
#include "util.h"


//UINT8
std::vector<u_char> _encode_type_num(u_char cbor_type, u_char val) {
    std::vector<u_char> ret;
    ret.reserve(1);
    if (val >= 0) {
        if (val <= 23) {
            ret.push_back(cbor_type | val);
            return ret;
        } else if (val <= 0x0ff){
            ret.push_back(cbor_type | CBOR_UINT8_FOLLOWS);
            ret.push_back(val);
            return ret;
        }
    }
    return std::vector<u_char>();
} 

//UINT16
std::vector<u_char> _encode_type_num(u_char cbor_type, ushort val) {
    std::vector<u_char> ret;
    ret.reserve(3);
    if (val >= 0) {
        if (val <= 0x0ff) {
           return _encode_type_num(cbor_type, (u_char) val);
        } else if (val <= 0x0ffff) {
            ret.push_back(cbor_type | CBOR_UINT16_FOLLOWS);
            std::vector<u_char> val_bytes = ushortToByte(val, "big");
            ret.push_back(val_bytes[0]);
            ret.push_back(val_bytes[1]);
            return ret;
        }
    }
    return std::vector<u_char>();
}

//UINT32
std::vector<u_char> _encode_type_num(u_char cbor_type, uint val) {
    std::vector<u_char> ret;
    ret.reserve(3);
    if (val >= 0) {
        if (val <= 0x0ffff) {
           return _encode_type_num(cbor_type, (ushort) val);
        } else if (val <= 0x0ffffffff) {
            ret.push_back(cbor_type | CBOR_UINT32_FOLLOWS);
            std::vector<u_char> val_bytes = uintToByte(val, "big");
            ret.insert(ret.end(), val_bytes.begin(), val_bytes.end());
            return ret;
        }
    }
    return std::vector<u_char>();
}

//UINT64
std::vector<u_char> _encode_type_num(u_char cbor_type, ulong val) {
    std::vector<u_char> ret;
    ret.reserve(3);
    if (val >= 0) {
        if (val <= 0x0ffffffff) {
           return _encode_type_num(cbor_type, (uint) val);
        } else if (val <= 0x0ffffffffffffffff) {
            ret.push_back(cbor_type | CBOR_UINT64_FOLLOWS);
            std::vector<u_char> val_bytes = ulongToByte(val, "big");
            ret.insert(ret.end(), val_bytes.begin(), val_bytes.end());
            return ret;
        }
    }
    return std::vector<u_char>();
}

std::vector<u_char> dumps_int(long val) {
    u_char cbor_type = CBOR_UINT;
    // u_char cbor_tag = CBOR_TAG_BIGNUM_BYTES;
    if (val < 0) {
        val = -1 - val;
        cbor_type = CBOR_NEGINT;
        // cbor_tag = CBOR_TAG_NEGBIGNUM_BYTES;
    }
    
    if (val <= (long) 0x0ffffffffffffffff) {
        return _encode_type_num(cbor_type, (ulong) val);
    }

    //ASK Chris is bignum handling necessesary for this sim?
    return std::vector<u_char>();
}


std::vector<u_char> dumps(bool b) {
    std::vector<u_char> ret;
    ret.reserve(1);
    
    if (b) {
        ret.push_back(CBOR_TRUE);
    } else {
        ret.push_back(CBOR_FALSE);
    }
    return ret;
}


void testingStuff() {
    std::vector<u_char> testBytes = _encode_type_num(CBOR_NEGINT, (uint) (0x0ffff + 1));
    printBytes(testBytes);
    
}

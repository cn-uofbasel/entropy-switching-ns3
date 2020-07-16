#ifndef PCAP_H
#define PCAP_H


#include <cstring>
#include <iostream>
#include "util.h"



class PCAP {
    private:

        std::string fileName;
        int rdOffset;
        int wrOffset;
        int len;
        FILE* fileOffsets;
        FILE* pFile;

        //returns the read value on succes;
        int readIntFromOffsets(int &ret, std::string endianness) {
            u_char intBytes[4];
            int nelems = fread(intBytes, 4, 1, this->fileOffsets);         
            byteToInt(ret, intBytes, endianness);
            if (nelems == 0) {
                ret = 0;
            }
            return 0;
        }

        //returns the read value on succes;
        int readIntFromFile(int &ret, std::string endianness) {
            u_char intBytes[4];
            int nelems = fread(intBytes, 4, 1, this->pFile);         
            byteToInt(ret, intBytes, endianness);
            if (nelems == 0) {
                ret = 0;
            }
            return 0;
        }

        void wrTypedBlock(int type, int* buff, int buffSize) {
            if (this->pFile != nullptr) {  
                char typeBytes[4];
                for (int i = 0; i < 4; i++) {            
                    typeBytes[i] = (type >> ((3 - i) *8)) & 0xFF;
                }

                fwrite(typeBytes, 1, 4, this->pFile);   
                char bufferBytes[buffSize];
                
                for (int i = 0; i < buffSize; i++) {
                    bufferBytes[i] = (buff[i/4] >> ((4 * buffSize - 1 - i) *8)) & 0xFF;
                }

                int l = (buffSize + 12);
                char lBytes[4];
                
                for (int i = 0; i < 4; i++) {            
                    lBytes[i] = (l >> ((3 - i) *8)) & 0xFF;
                }

                int totalSize = 8 + buffSize;
                char total[totalSize];
                memcpy(total, lBytes, 4);
                memcpy(total+4, bufferBytes, buffSize);
                memcpy(total+4+buffSize, lBytes, 4);
                fwrite(total, 1,totalSize, this->pFile);
                fflush(this->pFile); 
            }
        }

        void wrTypedBlock(int type, std::vector<u_char> toWrite) {
            if (this->pFile != nullptr) {

                int twLen = toWrite.size();
                char typeBytes[4];
                for (int i = 0; i < 4; i++) {            
                    typeBytes[i] = (type >> ((3 - i) *8)) & 0xFF;
                }
                fwrite(typeBytes, 1, 4, this->pFile);   
                
                
                u_char lenBytes[4];
                printf("Len: %i\n", twLen);
                for (int i = 0; i < 4; i++) {            
                    lenBytes[i] = (twLen >> ((3 - i) *8)) & 0xFF;
                }
                std::vector<u_char> toWriteWithLen(lenBytes, lenBytes + 4);
                toWriteWithLen.insert(toWriteWithLen.end(), toWrite.begin(), toWrite.end());
                
                int buffSize = toWriteWithLen.size();
                int padding = buffSize % 4;
                printf("Padding: %i\n", padding);
                if (padding > 0) {
                    for (int i = 0; i < 4 - padding; i++) {
                        toWriteWithLen.push_back((u_char) 0);
                    }
                }

                buffSize = toWriteWithLen.size();
                int l = (buffSize + 12);
                u_char lBytes[4];
                
                for (int i = 0; i < 4; i++) {            
                    lBytes[i] = (l >> ((3 - i) *8)) & 0xFF;
                }
                toWriteWithLen.insert(toWriteWithLen.begin(), lBytes, lBytes+4);
                toWriteWithLen.insert(toWriteWithLen.end(), lBytes, lBytes+4);
                
                fwrite(&toWriteWithLen[0], 1, toWriteWithLen.size(), this->pFile);
                fflush(this->pFile); 
            }
        }
    
    public:

        PCAP(std::string fileName, int rdOffset=0) {
            this->fileName = fileName;
            this->rdOffset = rdOffset;
            this->wrOffset = 0;
            this->fileOffsets = NULL;
            this->len = 0;
            this->pFile = NULL;
        }

        ~PCAP() {

        }

        void open(char mode, int offset=0) {
                std::string fileMode = "";
                fileMode.push_back(mode);
            if (fileMode == "a") {
                fileMode = "r+";
            }
            std::cout << fileMode;
            if (fileMode == "r+") {    
                std::cout << "Opening in mode: " << fileMode << std::endl;
                this->pFile = fopen(const_cast<char*>(this->fileName.c_str()), const_cast<char*>(fileMode.c_str()));
                if (this->pFile != nullptr) {
                    fclose(this->pFile);   
                    std::cout << "FAILED opening in mode: " << fileMode << std::endl;
                } else {
                    fileMode = "w";
                }
                std::cout << "Opening in mode: " << (fileMode + "b") << std::endl;
                this->pFile = fopen(const_cast<char*>(this->fileName.c_str()), const_cast<char*>((fileMode + "b").c_str()));
                this->fileOffsets = fopen(const_cast<char*>((this->fileName + ".offs").c_str()), const_cast<char*>((fileMode + "b").c_str()));
            }

            if (fileMode == "w") {

                int n0 = 0x1A2B3C4D;
                int n1 = 0x00010001;
                int n2 = 0x7fffffff;
                int n3 = 0xffffffff;
                int initialSect[] = {n0, n1, n2, n3};
                this->wrTypedBlock(0x0A0D0D0A, initialSect, sizeof(initialSect));
            
                n0 = 99 << 16;
                n1 = 0;
                int ifDescriptor[] = {n0, n1};
                this->wrTypedBlock(1, ifDescriptor, sizeof(ifDescriptor));
                
            } else if(fileMode == "r" || fileMode == "r+") {
                if (this->pFile != nullptr) {
                    fseek(this->pFile, 48, SEEK_SET);
                }
                if (this->fileOffsets != nullptr) {
                    fseek(this->fileOffsets, 0, SEEK_END);
                    this->len = ftell(this->fileOffsets) / 4;
                }
            } else {
                if (this->pFile != nullptr) {                
                    fseek(this->pFile, offset, SEEK_SET);
                }
            }
            if (this->pFile != nullptr) {                
                this->rdOffset = ftell(this->pFile);
            }
        }
            


        void close() {
            if (this->pFile != nullptr) {
                this->wrOffset = ftell(this->pFile);
                fclose(this->pFile);
                
                if (this->fileOffsets != nullptr) {
                    fclose(this->fileOffsets);
                }
                this->fileOffsets = nullptr;
                this->pFile = nullptr;
            }
        }

        int read(std::vector<u_char> buffer) {
            return read(buffer, -1);
        }

        //Returns 0 on success;
        //Needs improvement?
        int read(std::vector<u_char> ret, int lim) {
            while (lim == -1 || (lim > this->rdOffset && lim != -1)) {
                //last_read_block was stored, but never used ask chris. es_pcap.py
                // printf("  read at %i/%li\n", this->rdOffset, ftell(this->pFile));
                int t = 0;
                readIntFromFile(t, "big");
                printf("t: %i\n", t);

                // printf("  read at %i/%li\n", this->rdOffset, ftell(this->pFile));
                int l = 0;
                readIntFromFile(l, "big");
                printf("l: %i\n", l);
                if (l < 12) {
                    break;
                }

                u_char b[l - 12];
                fread(b, l - 12, 1, this->pFile); 
                fseek(this->pFile, ftell(this->pFile)+4,0);
                
                if (t == 3) {
                    l = 0;
                    byteToInt(l, b, "big");
                    printf("found len2=%u\n", l);
                    ret.insert(ret.end(), b+4, b+l+4);
                    break;
                }
                this->rdOffset += l-4;
            }
            this->rdOffset = ftell(this->pFile);
            return 0;
        }

        int read_at(std::vector<u_char> ret, int offs) {
            int old = ftell(this->pFile);
            fseek(this->pFile, offs, 0);
            int t = 0;
            readIntFromFile(t, "big");
            int l = 0;
            readIntFromFile(l, "big");
            if (l >= 12) {
                std::vector<u_char> b;
                b.reserve(l-12/sizeof(u_char));
                fread(&b[0], l-12, 1, this->pFile);
                fseek(this->pFile, ftell(this->pFile)+4,0);
                if (t == 3) {
                    l = 0;
                    byteToInt(l, b, "big");
                    ret.insert(ret.end(), b.begin()+4, b.begin()+l+4);
                    this->rdOffset = ftell(this->pFile);
                }
            }
            fseek(this->pFile, old, 0);
            return 0;
        }

        int read_backwards(std::vector<u_char> ret) {
            return read_backwards(ret, false);
        }
        
        int read_backwards(std::vector<u_char> ret, bool startAtEnd) {
            if (startAtEnd) {
                fseek(this->pFile, 0, 2);
                this->rdOffset = ftell(this->pFile);
            }
            while(this->rdOffset > 48) {
                fseek(this->pFile, -4, 1);
                int l = 0;
                readIntFromFile(l, "big");
                fseek(this->pFile, -l, 1);
                int t = 0;
                readIntFromFile(t, "big");
                fseek(this->pFile, -4, 1);
                this->rdOffset -= l;
                if (t == 3) {
                    this->read(ret);
                    this->rdOffset -= l;
                    return 0;
                }
                
            }
            return 1;
        }

        int getLen() {
            return this->len;
        }

        int getItem(std::vector<u_char> ret, int ndx) {
            if (ndx >= this->len || ndx < 0) {
                printf("[WARNING]%s: access beyond len (%i / %i)", this->fileName.c_str(), ndx, this->len);
                //TODO Raise Error
            }
            fseek(this->fileOffsets, 4*ndx, 0);
            int pos = 0;
            readIntFromOffsets(pos, "big");
            fseek(this->pFile, pos, 0);
            read(ret);
            return 0;
        }


        int write(std::string pkt) {
            std::vector<u_char> toWrite(pkt.begin(), pkt.end());
            return write(toWrite);
        }

        int write(std::vector<u_char> pkt) {
            fseek(this->pFile, 0, 2);
            int pos = ftell(this->pFile);
            wrTypedBlock(3, pkt);
            std::vector<u_char> posToWrite = intToByte(pos, "big");
            fwrite(&posToWrite[0], 1, posToWrite.size(), this->fileOffsets);
            fflush(this->fileOffsets);
            this->len += 1;
            return 0;
        }

};

#endif
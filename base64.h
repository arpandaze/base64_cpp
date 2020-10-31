#include <fstream>
#include <iostream>

using namespace std;

typedef unsigned int mode;

class Base64
{
    private:
        long buffer_size;
        string source_filename;
        string source_string;
        string base64_string;
        unsigned char *buffer;
        mode input_mode;

    public:
        static const mode TextEncodeMode = 0;
        static const mode FileEncodeMode = 1;
        static const mode TextDecodeMode = 2;
        static const mode FileDecodeMode = 3;
        Base64(string a_source, mode a_mode = Base64::TextEncodeMode)
        {
            input_mode = a_mode;
            if(a_mode == Base64::FileEncodeMode){
                source_filename = a_source;
            }
            else if(a_mode == Base64::FileDecodeMode){
                base64_string = a_source;
            }
            else if(a_mode == Base64::TextEncodeMode){
                source_string = a_source;
            }
            else if(a_mode == Base64::TextDecodeMode){
                base64_string = a_source;
            }
        }

        string encode();
        unsigned char* decode();
        void decode(string);

        //Int value of Base64 char
        unsigned int b64(char a_char)
        {
            if(a_char >= 0x30 && a_char <= 0x39)
            {
                return (a_char - 0x30 + 0x34);
            }
            else if(a_char >= 0x41 && a_char <= 0x5A)
            {
                return (a_char - 0x41);
            }
            else if(a_char >= 0x61 && a_char <= 0x7A)
            {
                return (a_char - 0x61 + 0x1A);
            }
            else if(a_char == '+')
            {
                return 0x3E;
            }
            else if(a_char == '/')
            {
                return 0x3F;
            }
            else
            {
                return 0xFF;
            }
        }
};

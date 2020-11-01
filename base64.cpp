#include "base64.h"

unsigned char* Base64::decode(string output_file)
{
    int paddding_size = 0;
    paddding_size += (base64_string[base64_string.length()-1]=='=')?1:0;
    paddding_size += (base64_string[base64_string.length()-2]=='=')?1:0;
    buffer_size = base64_string.length()* 3/4 - paddding_size + 1; // +1 for Null Terminator
    buffer = new unsigned char[buffer_size];
    for(int i = 0; i < base64_string.length(); i+=4)
    {
        int b_index = i*3/4;
        unsigned char b64_vals[4];
        unsigned char bytes[3];

        b64_vals[0] = b64(base64_string[i]);
        b64_vals[1] = b64(base64_string[i+1]);
        b64_vals[2] = b64(base64_string[i+2]);
        b64_vals[3] = b64(base64_string[i+3]);

        //6 bits from first base64_char and 2 bits from beginning of second base64_char
        buffer[b_index] = (b64_vals[0]<<0x2)^(b64_vals[1]>>0x4);
        //4 bits from end of second b64_char and 4 bits from beginning of third b64_char
        buffer[b_index+1] = ((b64_vals[1]&0xF)<<0x4)^(b64_vals[2]>>0x2);
        //2 bits from end of third b64_char and 6 bits from fourth b64_char
        buffer[b_index+2] = ((b64_vals[2]&0x3)<<0x6)^b64_vals[3];
    }

    //Null Terminator
    buffer[buffer_size-1] = 0x0;

    if(output_file != "")
    {
        ofstream output;
        output.open(output_file, ios::out | ios::binary);
        output.write((char *)buffer, buffer_size-1); //-1 To Skip Null Terminator
        output.close();
    }

    return buffer;
}

string Base64::encode()
{
    if(input_mode==Base64::FileEncode)
    {
        ifstream file;
        file.open(source_filename,ios::in | ios::binary);
        file.seekg(0,ios::end);
        buffer_size = file.tellg();
        file.seekg(0,ios::beg);

        buffer = new unsigned char[buffer_size];
        file.read((char *)buffer, buffer_size);
        file.close();
    }

    else if(input_mode==Base64::TextEncode)
    {
        buffer_size = source_string.length();
        buffer = new unsigned char[buffer_size];
        buffer = (unsigned char *)source_string.c_str();
    }

    int padding_length = (buffer_size % 3) ? (3 - buffer_size % 3) : 0;

    for(int i = 0; i < buffer_size; i+=3)
    {
        unsigned char separatedBits[4];

        /*
         * First b64_char = First 6 bits of first byte
         * Second b64_char = Last 2 bits of first byte and first four bits of second byte
         * Third b64_char = Last 6 bits of second byte and first 2 bits of third byte
         * Fourth b64_char = Last 6 bits of last byte
        */

        separatedBits[0] = buffer[i+0] >> 0x2;
        separatedBits[1] = ((buffer[i+0] & 0x3)<<0x4)^(buffer[i+1] >> 0x4);
        separatedBits[2] = ((buffer[i+1] & 0xF)<<0x2)^(buffer[i+2] >> 0x6);
        separatedBits[3] = (buffer[i+2] & 0x3F);

        char character;
        for(int j = 0; j <= 3; j++)
        {
           if(separatedBits[j] <= 0x19)
            {
                character = 'A' + separatedBits[j];
            }
            else if(separatedBits[j] >= 0x1A && separatedBits[j] <= 0x33)
            {
                character = 'a' + separatedBits[j] - 0x1A;
            }
            else if(separatedBits[j] >= 0x34 && separatedBits[j] <= 0x3D)
            {
                character = '0' + separatedBits[j] - 0x34;
            }
            else if(separatedBits[j] == 0x3E)
            {
                character = '+';
            }
            else if(separatedBits[j] == 0x3F)
            {
                character = '/';
            }
            base64_string += character;
        }

    }

    string pad = (padding_length==0) ? "" : ((padding_length==1) ? "=" : "==");
    base64_string = base64_string.substr(0,base64_string.length()-padding_length) + pad;

    return base64_string;
}

#include "base64.h"

inline unsigned int b64(char a_char)
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

string Base64::toBase64()
{
    return toBase64(source_filename);
}

unsigned char* Base64::toBinary(string s_b64)
{
    buffer_size = s_b64.length()*3/4;
    buffer = new unsigned char[buffer_size];
    cout << buffer_size << endl;
    for(int i = 0; i < s_b64.length(); i+=4)
    {
        int b_index = i*3/4;
        unsigned char b64_vals[4];
        unsigned char bytes[3];

        b64_vals[0] = b64(s_b64[i]);
        b64_vals[1] = b64(s_b64[i+1]);
        b64_vals[2] = b64(s_b64[i+2]);
        b64_vals[3] = b64(s_b64[i+3]);

        buffer[b_index] = (b64_vals[0]<<0x2)^(b64_vals[1]>>0x4);
        buffer[b_index+1] = ((b64_vals[1]&0xF)<<0x4)^(b64_vals[2]>>0x2);
        buffer[b_index+2] = ((b64_vals[2]&0x3)<<0x6)^b64_vals[3];
    }
    return buffer;
}

void Base64::toBinary(string s_b64, string filename)
{
    ofstream output;
    output.open(filename, ios::out | ios::binary);
    toBinary(s_b64);
    output.write((char *)buffer,buffer_size);
    output.close();
}

string Base64::toBase64(string filename)
{
    file.open(filename,ios::in | ios::binary);
    file.seekg(0,ios::end);
    buffer_size = file.tellg();
    file.seekg(0,ios::beg);

    buffer = new unsigned char[buffer_size];
    file.read((char *)buffer, buffer_size);

    int padding_length = buffer_size % 3;

    for(int i = 0; i < buffer_size; i+=3)
    {
        unsigned char separatedBits[4];

        separatedBits[0] = buffer[i+0] >>0x2;
        separatedBits[1] = ((buffer[i+0] & 0x3)<<0x4)^(buffer[i+1] >> 0x4);
        separatedBits[2] = ((buffer[i+1] & 0xF)<<0x2)^(buffer[i+2] >> 0x6);
        separatedBits[3] = (buffer[i+2] & 0x3F);

        char character;
        for(int j = 0; j <= 3; j++)
        {
            if(separatedBits[j] <= 25)
            {
                character = 'A' + separatedBits[j];
            }
            else if(separatedBits[j]>=26 && separatedBits[j] <= 51)
            {
                character = 'a' + separatedBits[j] - 26;
            }
            else if(separatedBits[j]>=52 && separatedBits[j] <= 61)
            {
                character = '0' + separatedBits[j] - 52;
            }
            else if(separatedBits[j] == 62)
            {
                character = '+';
            }
            else if(separatedBits[j] == 63)
            {
                character = '/';
            }
            finalBase += character;
        }

    }
    if(padding_length==1){
        finalBase = finalBase.substr(0,finalBase.length()-2) + "==";
    }
    else if(padding_length==2){
        finalBase = finalBase.substr(0,finalBase.length()-1) + "=";
    }
    return finalBase;
}
#include <fstream>
#include <deque>
#include <iostream>

using namespace std;

class Base64
{
    private:
        ifstream file;
        long buffer_size;
        string source_filename;
        string finalBase;
        unsigned char *buffer;
    public:
        Base64(string a_filename)
        {
            source_filename = a_filename; 
        }
        string toBase64(string);
        string toBase64();
        unsigned char* toBinary(string);
        void toBinary(string, string);
};
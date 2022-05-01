#include <string>
#include <iostream>
using namespace std;

string xorcipher(string input_text, string key);

int main(int argc, char *argv[])
{
    string input_text;
    string output_text;
    string key;
    char c;

    if(argc != 2)
    {
        cerr << "Usage: xorcipher [key]" << endl;
        return 0;
    }

    key = argv[1];

    while(cin.get(c))
    {
        input_text.push_back(c);
    }

    output_text = xorcipher(input_text, key);
    cout << output_text;
    cerr << endl;

    return 1;
}

/*
either encrypts a plaintext using the specified key, or decrypts a plaintext using the specified key
both are the same operation
key is used a stream-style, like the vigenere cipher
*/
string xorcipher(string input_text, string key)
{
    string output_text;
    char c;

    for(int i = 0; i < input_text.size(); i++)
    {
        c = input_text[i] ^ (key[i % key.size()]);
        output_text.push_back(c);
    }

    return output_text;
}
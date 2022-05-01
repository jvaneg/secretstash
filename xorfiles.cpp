#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    ifstream file1;
    ifstream file2;
    string text1;
    string text2;
    string output_text;
    char c;

    if(argc != 3)
    {
        cerr << "Usage: xorfiles [file1 name] [file2_name]" << endl;
        return 0;
    }

    string file1_name = string(argv[1]);
    string file2_name = string(argv[2]);

    file1.open(file1_name);
    if(!file1.good())
    {
        cerr << file1_name << " does not exist!" << endl;
        return 0;
    }
    while(file1.get(c))
    {
        text1.push_back(c);
    }
    file1.close();

    file2.open(file2_name);
    if(!file2.good())
    {
        cerr << file2_name << " does not exist!" << endl;
        return 0;
    }
    while(file2.get(c))
    {
        text2.push_back(c);
    }
    file2.close();

    if(text1.size() != text2.size())
    {
        cerr << "files must match in length" << endl;
        return 0;
    }

    for(int i = 0; i < text1.size(); i++)
    {
        c = text1[i] ^ text2[i];
        output_text.push_back(c);
    }

    cout << output_text;
    cerr << endl;

    return 1;
}
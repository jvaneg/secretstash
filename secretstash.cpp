#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>

using namespace std;

#define SECRETS_DIRECTORY "/home/joel.vanegmond/secretstash/secrets/"
#define KEY_FILE_NAME "/home/joel.vanegmond/secretstash/keyfile"
#define OWNER_ONLY_FOLDER 00770
#define OWNER_ONLY_FILE 00640
#define MAX_SECRET_LENGTH 16 // does not include null terminator
#define MAX_SECRET_BUFF MAX_SECRET_LENGTH + 1 // includes null terminator
#define MAX_KEY_BUFF 100

void stash(string base_key, string secret_name);
void pop(string base_key, string secret_name);
string xorcipher(char input_text[], int input_size, string key);
string load_base_key(string key_file_name);

int main(int argc, char *argv[])
{
    string input_text;
    string output_text;
    string base_key;
    bool success = false;
    char c;

    if(argc > 3 || argc < 2)
    {
        cerr << "Usage: secretstash [--pop/-p] [secret name]" << endl;
        cerr << "Secrets will be cut off at " << MAX_SECRET_LENGTH << " characters" << endl;
        cerr << "Secrets end on a newline" << endl;
        cerr << "Secret names cannot contain a '/' character - names will be truncated to before the slash" << endl; // '/' is an illegal char in linux filenames
        return 0;
    }

    if(((string)argv[1] == "--pop" || (string)argv[1] == "-p") && argc == 3)
    {
        
        pop(load_base_key(KEY_FILE_NAME), (string)argv[2]);
    }
    else if(argc == 2)
    {
        stash(load_base_key(KEY_FILE_NAME), (string)argv[1]);
    }
    else
    {
        cerr << "Usage: secretstash [--pop/-p] [secret name]" << endl;
        return 0;
    }

    return 1;
}


/*
Encrypts the input text from stdin and writes to a file named <secret name> in a folder named the user's uid
*/
void stash(string base_key, string secret_name)
{
    ofstream out_file;
    string input_text;
    string output_text;
    int uid = getuid();
    string uid_directory = SECRETS_DIRECTORY + to_string(uid);
    char* user_name = getpwuid(uid)->pw_name;
    char full_key[MAX_KEY_BUFF] = {};
    char secret[MAX_SECRET_LENGTH];
    char c;

    reverse(user_name, user_name + strlen(user_name));
    strcat((char*)full_key, (char*)base_key.c_str());
    strcat((char*)full_key, user_name);

    cin.getline(secret, MAX_SECRET_BUFF);
    output_text = xorcipher(secret, strlen(secret), string((char*)full_key));

    secret_name = secret_name.substr(0, secret_name.find('/'));
    mkdir(uid_directory.c_str(), OWNER_ONLY_FOLDER);
    out_file.open(uid_directory + "/" + secret_name);
    out_file << output_text;
    out_file.close();
	chmod((char*)(uid_directory + "/" + secret_name).c_str(), OWNER_ONLY_FILE);

    cerr << secret_name << " stashed!" << endl;
}


/*
Decrypts the secret named <secret_name> in the user's uid folder and pops it to stdout
*/
void pop(string base_key, string secret_name)
{
    ifstream in_file;
    string uid_directory;
    string user_name;
    string full_key;
    string input_text;
    string output_text;
    int* uid = new int;
    char c;
    *uid = getuid();
    user_name = string(getpwuid(*uid)->pw_name);

    fprintf(stderr, "Popping ");
    fprintf(stderr, secret_name.c_str());
    fprintf(stderr, "\n");

    uid_directory = SECRETS_DIRECTORY + to_string(*uid);
    reverse(user_name.begin(), user_name.end());
    full_key = base_key + user_name;
    secret_name = secret_name.substr(0, secret_name.find('/'));

    in_file.open(uid_directory + "/" + secret_name);
    if(!in_file.good())
    {
        cerr << secret_name << " does not exist!" << endl;
        return;
    }
    while(in_file.get(c))
    {
        input_text.push_back(c);
    }
    in_file.close();

    output_text = xorcipher((char*)input_text.c_str(), input_text.size(), full_key);

    cout << output_text;
    cerr << endl;
}

/*
either encrypts a plaintext using the specified key, or decrypts a plaintext using the specified key
both are the same operation
key is used in a stream-style, like the vigenere cipher
does nothing with an empty key
*/
string xorcipher(char input_text[], int input_size, string key)
{
    string output_text;
    char c;

    if(key.size() == 0)
        return string(input_text);
    for(int i = 0; i < input_size; i++)
    {
        c = input_text[i] ^ (key[i % key.size()]);
        output_text.push_back(c);
    }

    return output_text;
}

/*
reads the base_key from a file
*/
string load_base_key(string key_file_name)
{
    ifstream key_file;
    string base_key;

    key_file.open(key_file_name);
    getline(key_file, base_key);
    key_file.close();

    return base_key;
}
all: secretstash xorcipher xorfiles

secretstash: secretstash.cpp
	g++ -o secretstash secretstash.cpp -fno-stack-protector -Wno-format-security
	chmod 4555 secretstash

xorcipher: xorcipher.cpp
	g++ -o xorcipher xorcipher.cpp

xorfiles: xorfiles.cpp
	g++ -o xorfiles xorfiles.cpp


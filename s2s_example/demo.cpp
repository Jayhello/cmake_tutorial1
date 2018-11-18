#define _GLIBCXX_USE_CXX11_ABI 0
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "s2sClientIf.h"

using namespace std;
int main(int argc, char*argv[])
{

	S2sDataEncoder encoder;

	encoder.writeTcpPort(10009);
	std::vector<int64_t> value;
	value.push_back(6036180876LL);
	encoder.insert("iplist", value);

	std::string data = encoder.endToString();

	for(size_t i  = 0; i<data.size(); i++)
	{
		fprintf(stderr, "%d ", data[i]);
	}	
	
}




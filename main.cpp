#include <iostream>
#include <string>
#include <cstdlib>
#include "getopt.h"

using namespace std;

int main(int argc, char **argv){
	int opt;
	int port=80;
	const char* server_address = "127.0.0.1";
	const char* directory = "/temp/server/";
	while((opt = getopt(argc, argv, "h:p:d:")) !=-1){
		switch(opt){
			case 'h':
				if(optarg){
					server_address= optarg;
				}
				//server_address = optarg;
				//std::cout<<"blah: "<<optarg<<std::endl;
				break;
			case 'p':
				if(optarg){
					port = atoi(optarg);
				}
				break;
			case 'd':
				if(optarg){
					directory = optarg;
				}
				break;
			default:
				std::cerr<<"unknown param: "<<to_string(opt)<<std::endl;
			break;
		}
	}
	cout<<"Server address: "<<server_address<<endl;
	cout<<"Established port:"<<port<<endl;
	cout<<"Directory: "<<directory<<endl; 
	cout<<"test completed"<<std::endl;
	return 0;
}


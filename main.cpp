#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "getopt.h"


using namespace std;

int main(int argc, char **argv){
	int opt;
	int port=80;
	const char* server_address = "127.0.0.1";
	const char* directory = "/tmp/server/";
	pid_t daemon_pid;
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
//	cout<<"test completed"<<std::endl;

	daemon_pid = fork();
	if(daemon_pid<0){
		return -1;
	}
	//child process
	else if(daemon_pid==0){
		//new rules;
		umask(0);
		//new session
		setsid();
		//move to directory
		chdir(directory);
		//close IO descriptors
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		// open file for io
	}
	else{
		return 0;
	}

	while(1);

	return 0;
}


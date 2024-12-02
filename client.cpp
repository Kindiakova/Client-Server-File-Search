#include <string>
#include <iostream>
#if defined(_WIN32) 

#include <handleapi.h>
#define SLEEP_TIME 500
#define WIN(exp) exp
#define LIN(exp)

#include "headers/WinConnect.h"
#include "headers/WinFileSearch.h"

#include "windows.h"

#else
    #include <sys/types.h>
    #include <cstring>
    #include <pthread.h>
	
	#define MAX_PATH 260
	#define SLEEP_TIME 0.5
	#define WIN(exp) 
	#define LIN(exp) exp
    
    #include "headers/LinuxConnect.h"
    #include "headers/LinuxFileSearch.h"
#endif
using namespace std;


void error(const string &err, bool flag = 0){
    if (flag) cout << err <<"\n";
    else {
    	cout << err <<":\n";
    	cout << "Error's code: " << WIN (WSAGetLastError()) LIN (errno) << "  --- ";
    	WIN(
    	    wchar_t *s = NULL;
	    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
               NULL, WSAGetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
               (LPWSTR)&s, 0, NULL
            );
	cout << s << "\n";
    	)
    	LIN(cout << strerror(errno) << "\n";)
    }
    return;	
}

bool STOP_FLAG = true;

WIN(DWORD)LIN(void *) WIN(WINAPI) Processing(WIN(LPVOID)LIN(void *) sock) 
 {
	const string msg = "Processing";
	while(!STOP_FLAG){
		if ((*(Connection *)sock).Send(msg)  < 0){
			error("Sending error in Processing");
			break;
		}
		WIN(Sleep(SLEEP_TIME);)
		LIN(sleep(SLEEP_TIME);)
	}
	return 0;
}


bool Send(Connection& sock, const string &filename)
{
	string path = "";
	STOP_FLAG = false;
	WIN (
	 HANDLE proc = CreateThread(NULL,0,Processing,&sock, 0, NULL);
	 CloseHandle(proc);
	)
	LIN (
	 pthread_t proc;
	 pthread_create(&proc, NULL, Processing, &sock);
	 pthread_detach(proc);
	 )
	 
	 
	 
	bool success = FileSearch(filename, path);
	STOP_FLAG = true;
	if (success) {
		if (sock.Send("success___") < 0) {
			 error("Sending error"); return 1;
		}
		if (sock.Send(path) < 0) {
			 error("Sending error"); return 1;
		}
		
	}
	else if (sock.Send("failure___") < 0){
		error("Sending error"); 
		return 1;
	}
	
    return 0;
}

bool Recv(Connection & sock, string &filename){
	char result[MAX_PATH + 1];
	
        int nbytes = sock.Recv(result, MAX_PATH + 1);
       
        if (nbytes < 0) {
            error("Receive error"); return 1;
        }
        if (nbytes == 0) {
            error("No data was received", 1); return 1;
        }
		result[nbytes] = 0;
        string tmp(result);
	filename = tmp;	
    return 0;
}

int main()
{
    Connection sock;
	
    int tmp = sock.Create();
    if (tmp != 0) 
    {
        if (tmp == -1) error ("Error WSAStartup");
        else error("Socket was not created");
        return 0;
    }
    cout << "Socket is created \n";
    sock.Hint("127.0.0.1");
  
    if (sock.Connect()) {
        error("Connection was not established");
    	sock.EndWork();
    	return 0;
    }
    cout << "Connected\n";
    while (true)
    {
	string request = "";
	if (Recv(sock, request)) break;
	
        if (request == "/end") break;
        
	if (Send(sock, request)) break;

    }
    sock.EndWork();
    return 0;
}

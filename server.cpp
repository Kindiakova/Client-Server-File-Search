
#include <iostream>
#include <string>

#ifdef _WIN32

    #include "headers/WinConnect.h"
    
    #define WIN(exp) exp
    #define LIN(exp) 
#else
    #include <sys/types.h>
    #include <cstring>
    #include <cerrno>
    
    #define WIN(exp)
    #define LIN(exp) exp
    #define MAX_PATH 260
    
    #include "headers/LinuxConnect.h"
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
    	LIN(cout << strerror(errno) << "\n");
    }
    return;	
}

bool Send(Connection& sock, const string& filename)
{
    if (sock.Send(filename) < 0) {
         error("Sending error");
        return 1;
    }
    return 0;
}

bool Recv(Connection& sock) {
	
    char result[MAX_PATH + 1];
    bool successFlag = false;
    while (true)
    {
    	memset(result, 0, MAX_PATH + 1);
    	if (successFlag){
    	    int nbytes = sock.Recv(result, MAX_PATH + 1);
    	    
    	    if (nbytes <= 0) {
                if (nbytes == 0) error("No data was received", 1);
                else error("Receive error");
                return 1; 
            }
            string tmp(result);
            cout << tmp << "\n";
            break;
    	}
    	else {
    	    int nbytes = sock.Recv(result, 10);
    	    if (nbytes <= 0) {
                if (nbytes == 0) error("No data was received", 1);
                else error("Receive error");
                return 1; 
            }
	    string tmp(result);
	   
	    if (tmp == "Processing")        
                cout << "Processing...\n";                		            
	    else if (tmp == "success___")
		successFlag = true;
	    else if (tmp == "failure___")
            {
                cout << "file not found\n";
                break;
            }	
  		
	    else {
	    	error("Server receive unexpected data", 1);
	    	return 0;
	    }				
    	}       
    } 

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
    if (sock.Bind()) {
    	error ("Bind failed");
    	sock.EndWork();
    	return 0;
    }
    
    cout << "Listening...\n";	
    if (sock.ListenAndAccept()) {
    	error("Connection was not established");
    	sock.EndWork();
    	return 0;
    }
    cout << "Connected\n";
    
    
    string commandline = "";
    while (cin >> commandline)
    {
        if (commandline == "end")
        {
            Send(sock, "/end");
            break;
        }
        else if (commandline == "find"){
            string filename = "";
	    cin >> filename;
	    if (Send(sock, filename)) break;
       
	    if (Recv(sock)) break;		
        }
        else if (commandline == "help") {
            cout << "CommandList :\n end --stop work\n find filename --find filepath\n";
        }
        else cout << "unknown command\n";
       
    }
    sock.EndWork();
    return 0;
}

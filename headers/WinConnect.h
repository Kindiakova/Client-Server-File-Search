#pragma once
#include<WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include<ws2tcpip.h>
#include<string>
class Connection {
private:
    const int PORT = 1027;
    sockaddr_in hint;
    WSADATA wsaData;
	SOCKET sock;

public:
    

    int Create();
    void Hint(const std::string & ipAddress);
    bool Bind();
    bool ListenAndAccept();
    bool Connect();
	int Send(const std::string &msg);
    int Recv(char (&result)[MAX_PATH + 1], int sz);
    void EndWork();
};

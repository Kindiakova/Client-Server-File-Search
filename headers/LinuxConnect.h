#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
//#include <netdb.h>
#include <arpa/inet.h>
#include <string>


class Connection {
private:
    const int PORT = 1027;
    sockaddr_in hint;
	int sock;
public:

    int Create();
    void Hint(const std::string & ipAddress);
    bool Bind();
    bool ListenAndAccept();
    bool Connect();
	int Send(const std::string &msg);
    int Recv(char (&result)[261], int sz);
    void EndWork();
};

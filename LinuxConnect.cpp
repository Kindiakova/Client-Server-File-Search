#include "headers/LinuxConnect.h"

int Connection::Create()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 1;
    return 0;
}

void Connection::Hint(const std::string & ipAddress){
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    
}

bool Connection::Bind(){
    if (bind(sock, (sockaddr*)&hint, sizeof(hint)) < 0)
    	return true;
    return false;
}

bool Connection::ListenAndAccept(){
    if (listen(sock, SOMAXCONN) == -1){
    	return true;
    }
	
	// Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(sock, (sockaddr*)&client, &clientSize);

    	// Close listening socket
    close(sock);
    sock = clientSocket;
    return false;
	
}
bool Connection::Connect(){
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes < 0) return true;
    return false;
}
int Connection::Send(const std::string &msg){
	return send(sock, msg.c_str(), msg.length(), 0);
}
int Connection::Recv(char (&result)[261], int sz){
	return recv(sock, result, sz, 0);
}

void Connection::EndWork() {
    close(sock);
}

#include "headers/WinConnect.h"
int Connection::Create(){
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        return -1; /// Error WSAStartup
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        return WSAGetLastError();
    }
    return 0;
}
void Connection::Hint(const std::string & ipAddress){	
   
    hint.sin_family = AF_INET;
    InetPton(AF_INET, ipAddress.c_str(), &hint.sin_addr.s_addr);
    hint.sin_port = htons(PORT);
}
bool Connection::Bind(){
    if (bind(sock, (SOCKADDR*)&hint, sizeof(hint)) == SOCKET_ERROR) 
	return true; //Bind failed

    return false;
}
bool Connection::ListenAndAccept(){   

    if (listen(sock, 1) == SOCKET_ERROR) {
        ////Error listening on socket
        return true;
    }

    /// Listening on socket...

    SOCKET sAccept;
    ///Waiting for client to connect...

    while (true) {
        sAccept = SOCKET_ERROR;
        while (sAccept == SOCKET_ERROR) {
            sAccept = accept(sock, NULL, NULL);
        }
        /// Client connected
        sock = sAccept;
        break;
    }
    return false;

}
bool Connection::Connect(){
    if (connect(sock, (SOCKADDR*)&hint, sizeof(hint)) == SOCKET_ERROR)
        return true; 
        
    return false;
}
int Connection::Send(const std::string &msg){
	return send(sock, msg.c_str(), msg.length(), 0);
	
}
int Connection::Recv(char (&result)[MAX_PATH + 1], int sz){
	return recv(sock, result, sz, 0);
}

void Connection::EndWork() {
	closesocket(sock);
    WSACleanup();
}

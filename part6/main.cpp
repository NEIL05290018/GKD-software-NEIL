#include"gui.hpp"
using namespace std;
int main(){
    WSADATA wsadata;
    int result = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(result!=0){
        return 1;
    }
    SOCKET clientsocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(clientsocket==INVALID_SOCKET){
        WSACleanup();
        return 1;
    }
    sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serveraddr.sin_port=htons(8888);
    int connectresult=connect(clientsocket,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr));
    if(connectresult==SOCKET_ERROR){
        closesocket(clientsocket);
        WSACleanup();
        return 1;
    }
    else{
        cout<<"connected to server"<<endl;
    }
    drawapp app(clientsocket);
    app.run("GKD");
    closesocket(clientsocket);
    WSACleanup();
    return 0;
}
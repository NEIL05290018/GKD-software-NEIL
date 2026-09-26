#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<cstring>
using namespace std;
int main(){
    WSADATA wsadata;
    int result=WSAStartup(MAKEWORD(2,2),&wsadata);
    if (result!=0)
    {
        return 1;
    }
    SOCKET clientsocket =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(clientsocket==INVALID_SOCKET){
     WSACleanup();
     return 1;   
    }
    sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serveraddr.sin_port=htons(8888);
    int connectresult=connect(clientsocket,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr));
    if (connectresult==SOCKET_ERROR)
    {
        closesocket(clientsocket);
        WSACleanup();
        return 1;
    }
    else{
        cout<<"conneted to server"<<endl;
    }
    const char* message="hello the serversocket";
    send(clientsocket,message,strlen(message),0);
    char buffer[1024]="";
    int bytes=recv(clientsocket,buffer,sizeof(buffer)-1,0);
    if (bytes>0)
    {
        buffer[bytes]='\0';
        cout<<"recive:"<<buffer<<endl;
    }
    else if (bytes==0){
        cout<<"connetion closed"<<endl;
    }
    else{
        cout<<"recv failed"<<endl;
    }
    closesocket(clientsocket);
    WSACleanup();
}
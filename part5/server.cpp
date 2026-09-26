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
    SOCKET serversocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (serversocket==INVALID_SOCKET)
    {
        WSACleanup();
        return 1;
    }
    else{
        cout<<"socket created succesfully"<<endl;
    }
    sockaddr_in serveraddr;
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=INADDR_ANY;
    serveraddr.sin_port=htons(8888);
    int bindresult =bind(serversocket,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr));
    //将sockaddr_in*转换为sockaddr*类型，为了统一接口，便于ipv4和ipv6等结构的统一调用
    if (bindresult==SOCKET_ERROR)
    {
        closesocket(serversocket);
        WSACleanup();
        return 1;
    }
    if (listen(serversocket,SOMAXCONN)==SOCKET_ERROR)
    {
        closesocket(serversocket);
        WSACleanup();
        return 1;
    }
    else{
        cout<<"serversocket is listenning"<<endl;
    }
    SOCKET clientsocket=accept(serversocket,nullptr,nullptr);
    if (clientsocket==INVALID_SOCKET)
    {
        closesocket(serversocket);
        WSACleanup();
        return 1;
    }
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
    const char* message="hello! client";
    send(clientsocket,message,strlen(message),0);
    closesocket(clientsocket);
    closesocket(serversocket);
    WSACleanup();
}
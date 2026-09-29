#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<cstring>
#include<vector>
#include"matrix.hpp"
using namespace std;
bool sendall(SOCKET s,const char* data,int totalbytes){
    int sent = 0;
    while (sent<totalbytes)
    {
        int bytes=send(s,data+sent,totalbytes-sent,0);
        if (bytes==0)
        {
            cout<<"connect closed"<<endl;
            return false;
        }
        else if (bytes<0)
        {
            cout<<"sent fail"<<endl;
            return false;
        }
        sent+=bytes;
    }
    cout<<"send has compelete"<<endl;
    return true;
}
bool recvall(SOCKET s,char* data,int totalbytes){
    int received=0;
    while (received<totalbytes)
    {
        int bytes =recv(s,data+received,totalbytes-received,0);
        if (bytes==0)
        {
            cout<<"connect closed"<<endl;
            return false;
        }
        else if (bytes<0)
        {
            cout<<"receive fail"<<endl;
            return false;
        }
        received+=bytes;
        
    }
    return true;
}
void sendmatrix(SOCKET s,const matrix<float>& m){
    int rows=m.getrows();
    int cols=m.getcols();
    vector<float>data(rows*cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            data[r*cols+c]=m.get(r,c);
        }
        
    }
    sendall(s,reinterpret_cast<const char*>(&rows),sizeof(rows));
    sendall(s,reinterpret_cast<const char*>(&cols),sizeof(cols));
    sendall(s,reinterpret_cast<const char*>(data.data()),rows*cols*sizeof(float));
}
matrix<float> recvmatrix(SOCKET s){
    int rows;
    int cols;
    recvall(s,reinterpret_cast<char*>(&rows),sizeof(rows));
    recvall(s,reinterpret_cast<char*>(&cols),sizeof(cols));
    vector<float>data(rows*cols);
    recvall(s,reinterpret_cast<char*>(data.data()),rows*cols*sizeof(float));
    matrix<float> m(rows,cols);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            m.set(r,c,data[r*cols+c]);
        }
        
    }
    return m;
}
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
    int bindresult =::bind(serversocket,reinterpret_cast<sockaddr*>(&serveraddr),sizeof(serveraddr));
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
    cout<<"client is connecting"<<endl;
    model_base* f=nullptr;
    try{
    f=creatmodel("../mnist-fc/");
    matrix<float> input =recvmatrix(clientsocket);
    matrix<float> output=f->doforward(input);
    sendmatrix(clientsocket,output);
    }
    catch(const exception& e){
        cout<<e.what()<<endl;
    };
    delete f;
    closesocket(clientsocket);
    closesocket(serversocket);
    WSACleanup();
}
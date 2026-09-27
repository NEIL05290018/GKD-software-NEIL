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
    matrix<float>m(1,784);
    sendmatrix(clientsocket,m);
    matrix<float> output=recvmatrix(clientsocket);
    output.print();
    closesocket(clientsocket);
    WSACleanup();
}
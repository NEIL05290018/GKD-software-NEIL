#include<iostream>
#include<opencv2/opencv.hpp>
#include<winsock2.h>
#include<ws2tcpip.h>
#include"matrix.hpp"
using namespace std;

bool sendall(SOCKET s,const char* data,int totalbytes);
bool recvall(SOCKET s,char* data,int totalbytes);
void sendmatrix(SOCKET s,const matrix<float>& m);
matrix<float> recvmatrix(SOCKET s);
matrix<float> preprocess(const cv::Mat& image);
class drawapp{
private:
    SOCKET clientsocket;
    cv::Mat canvas;
    bool drawing;
    cv::Point lastpoint;
    static void onmouse(int event,int x,int y,int flags, void*userdata);
public:
    drawapp(const SOCKET& c);
    void run(const string& windowname);
    
};
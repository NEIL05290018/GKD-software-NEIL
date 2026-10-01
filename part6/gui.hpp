#include<iostream>
#include<opencv2/opencv.hpp>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<thread>
#include<mutex>
#include<atomic>
#include<array>
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
    cv::Mat display;
    bool drawing;
    cv::Point lastpoint;
    mutex canvasmutex,probmutex;
    atomic<bool>running{false};
    array<float,10> probability{};
    static void onmouse(int event,int x,int y,int flags, void*userdata);
    void infer();
    void makedisplay();
public:
    drawapp(const SOCKET& c);
    void run(const string& windowname);
};
#include"gui.hpp"
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
matrix<float> preprocess(const cv::Mat& image){
    if (image.empty())
    {
        throw invalid_argument("readimage failed");
    }
    cv::Mat resized;
    cv::resize(image,resized,cv::Size(28,28));
    matrix<float> input(1,784);
    for (int r = 0; r < 28; r++)
    {
        for (int c = 0; c < 28; c++)
        {
            input.set(0,r*28+c,resized.at<uchar>(r,c)/255.0f);
        }
        
    }
    return input;
}
    void drawapp::onmouse(int event,int x,int y,int flags, void*userdata){
        drawapp* app=static_cast<drawapp*>(userdata);  
        if(event==cv::EVENT_LBUTTONDOWN){
            app->drawing=true;
            app->lastpoint=cv::Point(x,y);
        }
        else if (event==cv::EVENT_MOUSEMOVE&&app->drawing==true){
            cv::line(app->canvas,app->lastpoint,cv::Point(x,y),cv::Scalar(0),20);
            app->lastpoint=cv::Point(x,y);
        }
        else if(event==cv::EVENT_LBUTTONUP){
            app->drawing=false;
        }
    }
    drawapp::drawapp(const SOCKET& c):canvas(560,560,CV_8UC1,cv::Scalar(255)),drawing(false),clientsocket(c){
        
    }
    void drawapp::run(const string& windowsname){
        cv::namedWindow(windowsname);
        cv::setMouseCallback(windowsname,onmouse,this);
        while (true)
        {
            cv::imshow(windowsname,canvas);
            int key=cv::waitKey(10);
            if (key==27) break;
        }
        
    }
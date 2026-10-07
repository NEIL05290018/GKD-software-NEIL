#include"gui.hpp"
using namespace std;
bool sendall(SOCKET s,const char* data,int totalbytes){
    int sent = 0;
    while (sent<totalbytes)
    {
        int bytes=send(s,data+sent,totalbytes-sent,0);
        if (bytes==0)
        {
            return false;
        }
        else if (bytes<0)
        {
            cout<<"sent fail"<<endl;
            return false;
        }
        sent+=bytes;
    }
    return true;
}
bool recvall(SOCKET s,char* data,int totalbytes){
    int received=0;
    while (received<totalbytes)
    {
        int bytes =recv(s,data+received,totalbytes-received,0);
        if (bytes==0)
        {
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
    if(!sendall(s,reinterpret_cast<const char*>(&rows),sizeof(rows))){
        throw runtime_error("connect close");
    }
    if(!sendall(s,reinterpret_cast<const char*>(&cols),sizeof(cols))){
        throw runtime_error("connect close");
    }
    if(!sendall(s,reinterpret_cast<const char*>(data.data()),rows*cols*sizeof(float))){
        throw runtime_error("connect close");
    }
}
matrix<float> recvmatrix(SOCKET s){
    int rows;
    int cols;
    if(!recvall(s,reinterpret_cast<char*>(&rows),sizeof(rows))){
        throw runtime_error("connect close");
    }
    if(!recvall(s,reinterpret_cast<char*>(&cols),sizeof(cols))){
        throw runtime_error("connect close");
    }
    vector<float>data(rows*cols);
    if(!recvall(s,reinterpret_cast<char*>(data.data()),rows*cols*sizeof(float))){
        throw runtime_error("connect close");
    }
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
        if (x < 0 || x >= 560 || y < 0 || y >= 560)
        {
            app->drawing = false;
            return;
        }
        if(event==cv::EVENT_LBUTTONDOWN){
            app->drawing=true;
            app->lastpoint=cv::Point(x,y);
        }
        else if (event==cv::EVENT_MOUSEMOVE&&app->drawing==true){
            app->canvasmutex.lock();
            cv::line(app->canvas,app->lastpoint,cv::Point(x,y),cv::Scalar(0),20);
            app->canvasmutex.unlock();
            app->lastpoint=cv::Point(x,y);
        }
        else if(event==cv::EVENT_LBUTTONUP){
            app->drawing=false;
        }
    }
    drawapp::drawapp(const SOCKET& c):canvas(560,560,CV_8UC1,cv::Scalar(255)),drawing(false),clientsocket(c){
        
    }
    void drawapp::makedisplay(){
            display = cv::Mat(560, 860, CV_8UC3, cv::Scalar(255,255,255));
            canvasmutex.lock();
            cv::Mat displaycanvas =canvas.clone();
            canvasmutex.unlock();
            array<float,10> displayprobability;
            probmutex.lock();
            displayprobability=probability;
            probmutex.unlock();
            cv::Mat displaycanvasBGR;
            cv::cvtColor(
            displaycanvas,
            displaycanvasBGR,
            cv::COLOR_GRAY2BGR);
            cv::Mat left=display(cv::Rect(0,0,560,560));
            displaycanvasBGR.copyTo(left);
            cv::rectangle(display,cv::Point(580,0),cv::Point(580+250*displayprobability[0],30),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,40),cv::Point(580+250*displayprobability[1],70),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,80),cv::Point(580+250*displayprobability[2],110),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,120),cv::Point(580+250*displayprobability[3],150),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,160),cv::Point(580+250*displayprobability[4],190),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,200),cv::Point(580+250*displayprobability[5],230),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,240),cv::Point(580+250*displayprobability[6],270),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,280),cv::Point(580+250*displayprobability[7],310),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,320),cv::Point(580+250*displayprobability[8],350),cv::Scalar(0,255,0),cv::FILLED);
            cv::rectangle(display,cv::Point(580,360),cv::Point(580+250*displayprobability[9],390),cv::Scalar(0,255,0),cv::FILLED);
            cv::putText(display,to_string(0),cv::Point(570,15),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(1),cv::Point(570,55),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(2),cv::Point(570,95),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(3),cv::Point(570,135),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(4),cv::Point(570,175),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(5),cv::Point(570,215),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(6),cv::Point(570,255),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(7),cv::Point(570,295),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(8),cv::Point(570,335),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
            cv::putText(display,to_string(9),cv::Point(570,375),cv::FONT_HERSHEY_SIMPLEX,0.6,cv::Scalar(0),1);
    }
    void drawapp::run(const string& windowsname){
        cv::namedWindow(windowsname);
        cv::setMouseCallback(windowsname,onmouse,this);
        running=true;
        thread worker(&drawapp::infer,this);
        while (true)
        {   
            makedisplay();
            cv::imshow(windowsname,display);
            int key=cv::waitKey(10);
            if(key==32){
                canvasmutex.lock();
                canvas.setTo(cv::Scalar(255));
                canvasmutex.unlock();
                probmutex.lock();
                for (int i = 0; i < 10; i++)
                {
                probability[i] = 0.0f;
                }
                probmutex.unlock();
            }
            if(!running){
                break;
            }
            if (key==27){ 
                running=false;
                break;
            }
        }
        worker.join();
        
    }
    void drawapp::infer(){
        try{
        while(running){
            cv::Mat calculatecanvas;
            canvasmutex.lock();
            calculatecanvas=canvas.clone();
            canvasmutex.unlock();
            matrix<float>input=preprocess(calculatecanvas);
            sendmatrix(clientsocket,input);
            matrix<float> output=recvmatrix(clientsocket);
            probmutex.lock();
            for ( int c= 0; c < output.getcols(); c++)
            {
                    probability[c]=output.get(0,c);
            }
            probmutex.unlock();
            
        }
    }
    catch(const exception& e){
        cout<<e.what()<<endl;
        running=false;
    }
    }
#include<iostream>
#include<vector>
#include<stdexcept>
#include<cmath>
#include<fstream>
#include"nlohmann/json.hpp"
#include<string>
using namespace std;
using json = nlohmann::json;
template<typename T>
class matrix {
private:
    int rows;
    int cols;
    vector<vector<T>>data;
public:
    matrix(int rows,int cols):rows(rows),cols(cols){
        if (rows<=0||cols<=0)
        {
            throw invalid_argument("非法构造！");
        }
        else{
           data=vector<vector<T>>(rows,vector<T>(cols,T{})); 
        }
        
    }
    
    void set (int row,int col,T value){
        if (0<=row&&row<rows&&0<=col&&col<cols)
        {
        data[row][col]=value;
        }
        else
        throw invalid_argument("非法设置！");
    }
    T get (int row,int col)const{
        if (0<=row&&row<rows&&0<=col&&col<cols){
        return data[row][col];
        }
        else
        throw invalid_argument("非法获取！");
    }
    void print()const{
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                cout<<data[r][c]<<" ";
            }
            cout<<endl;
            
        }
        
    }
    matrix operator+(const matrix& other)const{
        if (rows!=other.rows||cols!=other.cols)
        {
            throw invalid_argument("matrx dimentions must match");
        }
        else{
        matrix result(rows,cols);
        for (int r = 0; r< rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                result.data[r][c]=data[r][c]+other.data[r][c];
            }
            
        }
        
        return result;}
    }
    matrix operator*(const matrix& other)const{
     if (cols!=other.rows)
     {
        throw invalid_argument("Matrices cannot be mutiplied");
     }
     else{
        matrix result(rows,other.cols);
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < other.cols; c++)
            {
                for (int k = 0; k < cols; k++)
                {
                    result.data[r][c]+=data[r][k]*other.data[k][c];
                }
                
            }
            
        }
        
        return result;
     }
     
        
    }
    int getrows()const{
        return rows;
    }
    int getcols()const{
        return cols;
    }
    matrix relu()const{
        matrix result (rows,cols);
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                if (data[r][c]>T{})
                {
                    result.data[r][c]=data[r][c];
                }
                else
                    result.data[r][c]=T{};
                
            }
            
        }
        return result;
    }
    matrix softmax()const{
        matrix result (rows,cols);
        T sum=T{};
        T maxvalue =data[0][0];
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                if (data[r][c]>maxvalue)
                {
                    maxvalue =data[r][c];

                }
                
            }
            
        }
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                sum+=exp(data[r][c]-maxvalue);
            }
            
        }
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                result.data[r][c]=exp(data[r][c]-maxvalue)/sum;
            }
            
        }
        return result;   
    }
};
template<typename T>
class model{
private:
    matrix<T> weight1;
    matrix<T> bias1;
    matrix<T> weight2;
    matrix<T> bias2;
public:
    model(const matrix<T>& w1,const matrix<T>& b1,const matrix<T>& w2,const matrix<T>& b2)
    :weight1(w1),bias1(b1),weight2(w2),bias2(b2){

    }
    matrix<T> forward(const matrix<T>& x)const{
        matrix<T> layer1=x*weight1+bias1;
        matrix<T> hidden=layer1.relu();
        matrix<T> layer2=hidden*weight2+bias2;
        matrix<T> output=layer2.softmax();
        return output;
    }
    
};
template<typename T>
matrix<T> readmatrix(const string& filepath,int rows,int cols){
        ifstream file(filepath,ios::binary);
        if (file.is_open())
        {
        }
        else{
            throw runtime_error("文件打开失败");
        }
        matrix<T> x(rows,cols);
        T value;
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                if(!file.read(reinterpret_cast<char*>(&value),sizeof(T))){
                    throw runtime_error("模型文件数据不足");
                }
                
                x.set(r,c,value);
                
            }
            
        }
        return x;
        
    }
int main()
{ 
    try
    {
        ifstream metafile("../mnist-fc/meta.json");
        if (!metafile.is_open())
        {
            throw runtime_error("JSON文件打开失败");
        }
            cout<<"文件打开成功"<<endl;
        json metadata;
        metafile>>metadata;
        int weight1rows=metadata["fc1.weight"][0].get<int>();
        int weight1cols=metadata["fc1.weight"][1].get<int>();
        int weight2rows=metadata["fc2.weight"][0].get<int>();
        int weight2cols=metadata["fc2.weight"][1].get<int>();
        int bias1rows=metadata["fc1.bias"][0].get<int>();
        int bias1cols=metadata["fc1.bias"][1].get<int>();
        int bias2rows=metadata["fc2.bias"][0].get<int>();
        int bias2cols=metadata["fc2.bias"][1].get<int>();
        matrix<float> bias1 = readmatrix<float>("../mnist-fc/fc1.bias", bias1rows, bias1cols);
        matrix<float> bias2 = readmatrix<float>("../mnist-fc/fc2.bias", bias2rows, bias2cols);
        matrix<float> weight1 = readmatrix<float>("../mnist-fc/fc1.weight", weight1rows, weight1cols);
        matrix<float> weight2 =readmatrix<float>("../mnist-fc/fc2.weight", weight2rows, weight2cols);
        /*cout<<"bias1:"<<endl<<bias1.getrows()<<" "<<bias1.getcols()<<endl;
        cout<<"bias2:"<<endl<<bias2.getrows()<<" "<<bias2.getcols()<<endl;
        cout<<"weight1:"<<endl<<weight1.getrows()<<" "<<weight1.getcols()<<endl;
        cout<<"weight2:"<<endl<<weight2.getrows()<<" "<<weight2.getcols()<<endl;*/
        model<float> f (weight1,bias1,weight2,bias2);
        cout<<"模型加载成功"<<endl;
        matrix<float> input(1, weight1rows);  // 默认全部为 0
matrix<float> output = f.forward(input);

cout << "输出尺寸："
     << output.getrows() << " × "
     << output.getcols() << endl;

float probabilitySum = 0.0f;

for (int c = 0; c < output.getcols(); c++)
{
    cout << output.get(0, c) << " ";
    probabilitySum += output.get(0, c);
}

cout << endl;
cout << "概率总和：" << probabilitySum << endl;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}
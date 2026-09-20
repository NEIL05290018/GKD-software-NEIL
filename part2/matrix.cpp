#include<iostream>
#include<vector>
#include<stdexcept>
#include<cmath>
#include<fstream>
using namespace std;
class matrix {
private:
    int rows;
    int cols;
    vector<vector<float>>data;
public:
    matrix(int rows,int cols):rows(rows),cols(cols){
        if (rows<=0||cols<=0)
        {
            throw invalid_argument("非法构造！");
        }
        else{
           data=vector<vector<float>>(rows,vector<float>(cols,0.0f)); 
        }
        
    }
    
    void set (int row,int col,float value){
        if (0<=row&&row<rows&&0<=col&&col<cols)
        {
        data[row][col]=value;
        }
        else
        throw invalid_argument("非法设置！");
    }
    float get (int row,int col)const{
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
                if (data[r][c]>0)
                {
                    result.data[r][c]=data[r][c];
                }
                else
                    result.data[r][c]=0.0f;
                
            }
            
        }
        return result;
    }
    matrix softmax()const{
        matrix result (rows,cols);
        float sum=0.0f;
        float maxvalue =data[0][0];
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
class model{
private:
    matrix weight1;
    matrix bias1;
    matrix weight2;
    matrix bias2;
public:
    model(const matrix& w1,const matrix& b1,const matrix& w2,const matrix& b2)
    :weight1(w1),bias1(b1),weight2(w2),bias2(b2){

    }
    matrix forward(const matrix& x)const{
        matrix layer1=x*weight1+bias1;
        matrix hidden=layer1.relu();
        matrix layer2=hidden*weight2+bias2;
        matrix output=layer2.softmax();
        return output;
    }
    
};
matrix readmatrix(const string& filepath,int rows,int cols){
        ifstream file(filepath,ios::binary);
        if (file.is_open())
        {
            cout<<"打开成功"<<endl;
        }
        else{
            throw runtime_error("文件打开失败");
        }
        matrix x(rows,cols);
        float value;
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                if(!file.read(reinterpret_cast<char*>(&value),sizeof(float))){
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
        matrix bias1 = readmatrix("../mnist-fc/fc1.bias", 1, 500);
        matrix bias2 = readmatrix("../mnist-fc/fc2.bias", 1, 10);
        matrix weight1 = readmatrix("../mnist-fc/fc1.weight", 784, 500);
        matrix weight2 =readmatrix("../mnist-fc/fc2.weight", 500, 10);
        cout<<"bias1:"<<endl<<bias1.getrows()<<" "<<bias1.getcols()<<endl;
        cout<<"bias2:"<<endl<<bias2.getrows()<<" "<<bias2.getcols()<<endl;
        cout<<"weight1:"<<endl<<weight1.getrows()<<" "<<weight1.getcols()<<endl;
        cout<<"weight2:"<<endl<<weight2.getrows()<<" "<<weight2.getcols()<<endl;
        model f (weight1,bias1,weight2,bias2);
        cout<<"模型加载成功"<<endl;
        



    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}
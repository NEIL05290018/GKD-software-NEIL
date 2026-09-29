#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<vector>
#include<stdexcept>
#include<cmath>
#include<fstream>
#include"nlohmann/json.hpp"
#include<string>
#include<type_traits>
#include<thread>
#include<chrono>
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
        if ((rows!=other.rows&&other.rows!=1)||cols!=other.cols)
        {
            throw invalid_argument("matrx dimentions must match");
        }
        else{
        matrix result(rows,cols);
        if (rows==other.rows)
        {
        for (int r = 0; r< rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                result.data[r][c]=data[r][c]+other.data[r][c];
            }
            
        }   
        }
        else if(other.rows==1){
        for (int r = 0; r< rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                result.data[r][c]=data[r][c]+other.data[0][c];
            }
            
        }
        }
        return result;
    }
    }
    matrix operator*(const matrix& other)const{
     if (cols!=other.rows)
     {
        throw invalid_argument("Matrices cannot be mutiplied");
     }
     else{
        unsigned int count =thread::hardware_concurrency();
        if (count==0)
        {
            count=1;
        }
        else if(count>other.cols){
            count=other.cols;
        }
        vector<thread>workers;
        
        matrix result(rows,other.cols);
        auto calculate =[&](int begin,int end){
            for (int r = 0; r < rows; r++)
            {   
                    for (int k = 0; k < cols; k++)
                    {
                        for (int c = begin; c < end; c++)
                        {
                            result.data[r][c]+=data[r][k]*other.data[k][c];
                        }
                        
                    }
                
                
            }
            
        };
        if (count==1)
        {
            calculate(0,other.cols);
        }
        else{
        for (int w = 0; w < count; w++)
        {
         workers.emplace_back(calculate,w*other.cols/count,(w+1)*other.cols/count);   
        }
        for (auto&worker:workers)
        {
            worker.join();
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
        for (int r = 0; r < rows; r++)
        {
        T sum=T{};
        T maxvalue =data[r][0];
        for (int c = 0; c < cols; c++)
            {
                if (data[r][c]>maxvalue)
                {
                    maxvalue =data[r][c];

                }
                
            }
            for (int c = 0; c < cols; c++)
            {
                sum+=exp(data[r][c]-maxvalue);
            }
            for (int c = 0; c < cols; c++)
            {
                result.data[r][c]=exp(data[r][c]-maxvalue)/sum;
            }
    }   
    return result;
    }
};
class model_base{
public:
    virtual matrix<double> doforward (const matrix<double>& input) const =0;
    virtual matrix<float> doforward (const matrix<float>& input) const =0;
    virtual ~model_base()=default;
};
template<typename T>
class model:public model_base{
private:
    matrix<T> weight1;
    matrix<T> bias1;
    matrix<T> weight2;
    matrix<T> bias2;
public:
    matrix<float> doforward(const matrix<float>& input)const override{
        if constexpr(std::is_same_v<T,float>)
        {
            return forward(input);
        }
        else{
            matrix<double> temp(input.getrows(),input.getcols());
            for (int r = 0; r < input.getrows(); r++)
            {
                for (int c = 0; c < input.getcols(); c++)
                {
                    temp.set(r,c,input.get(r,c));
                }
                
            }
            
            
            matrix<double> finaloutput (forward(temp));
            matrix<float> output (finaloutput.getrows(),finaloutput.getcols());
            for (int r = 0; r < finaloutput.getrows(); r++)
            {
                for (int c = 0; c < finaloutput.getcols(); c++)
                {
                    output.set(r,c,finaloutput.get(r,c));
                }
                
            }
            return output;
        }
        
    }
    matrix<double> doforward(const matrix<double>& input)const override{
        if constexpr(std::is_same_v<T,double>)
        {
            return forward(input);
        }
        else{
            matrix<float> temp(input.getrows(),input.getcols());
            for (int r = 0; r < input.getrows(); r++)
            {
                for (int c = 0; c < input.getcols(); c++)
                {
                    temp.set(r,c,input.get(r,c));
                }
                
            }
            matrix<float> finaltemp(forward(temp));
            matrix<double> output (finaltemp.getrows(),finaltemp.getcols());
            for (int r = 0; r < finaltemp.getrows(); r++)
            {
                for (int c = 0; c < finaltemp.getcols(); c++)
                {
                    output.set(r,c,finaltemp.get(r,c));
                }
                
            }
            return output; 
        }
        
    }
    model(const matrix<T>& w1,const matrix<T>& b1,const matrix<T>& w2,const matrix<T>& b2)
    :weight1(w1),bias1(b1),weight2(w2),bias2(b2){

    }    
    matrix<T> forward(const matrix<T>& input)const{
            matrix<T> layer1=input*weight1+bias1;
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
    model_base* creatmodel(const string&folder);
    #endif

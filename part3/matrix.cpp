#include<iostream>
#include<vector>
#include<stdexcept>
#include<cmath>
#include<fstream>
#include"nlohmann/json.hpp"
#include<string>
#include<type_traits>
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
    model_base* creatmodel(const string& folder){
        ifstream metafile (folder+"meta.json");
        if (!metafile.is_open())
        {
            throw runtime_error("Json文件打开失败");
        }
        cout<<"File opened successfully"<<endl;
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
        string type =metadata["type"].get<string>();
        if (type=="fp32")
        {
            matrix<float> bias1 = readmatrix<float>(folder+"fc1.bias", bias1rows, bias1cols);
            matrix<float> bias2 = readmatrix<float>(folder+"fc2.bias", bias2rows, bias2cols);
            matrix<float> weight1 =readmatrix<float>(folder+"fc1.weight", weight1rows, weight1cols);
            matrix<float> weight2 =readmatrix<float>(folder+"fc2.weight", weight2rows, weight2cols);
            return new model<float>(weight1,bias1,weight2,bias2);
        }
        else if(type=="fp64"){
            matrix<double> bias1 = readmatrix<double>(folder+"fc1.bias", bias1rows, bias1cols);
            matrix<double> bias2 = readmatrix<double>(folder+"fc2.bias", bias2rows, bias2cols);
            matrix<double> weight1 =readmatrix<double>(folder+"fc1.weight", weight1rows, weight1cols);
            matrix<double> weight2 =readmatrix<double>(folder+"fc2.weight", weight2rows, weight2cols);
            return new model<double>(weight1,bias1,weight2,bias2);
        }
        else{
            throw runtime_error("未知模型类型");
        }
        
    }
int main(int argc,char* argv[])
{   string folder="../mnist-fc/";
    if (argc>1 && string(argv[1])=="plus")
    {
        folder="../mnist-fc-plus/";
    }
    
    model_base* f=nullptr;
    try
    {
        f =creatmodel(folder);
        matrix<float> input(1, 784); 
        auto start=chrono::steady_clock::now();
        matrix<float> output=f->doforward(input);
        auto end=chrono::steady_clock::now();

double probabilitySum = 0.0;

for (int c = 0; c < output.getcols(); c++)
{
    cout << output.get(0, c) << " ";
    probabilitySum += output.get(0, c);
}

cout << endl;
cout << "Sum of probabilities:" << probabilitySum << endl;
chrono::duration<double,milli> times=end-start;
cout<<"forward:"<<times.count()<<endl;
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }
    delete f;
    return 0;
}
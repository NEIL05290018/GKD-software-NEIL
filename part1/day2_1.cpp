#include<iostream>
#include<vector>
#include<stdexcept>
#include<cmath>
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
        throw invalid_argument("Matrices can not be mutiplied");
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
                    result.data[r][c]=0;
                
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
        /*return (((x*weight1+bias1).relu())*weight2+bias2).softmax();*/
        matrix layer1=x*weight1+bias1;
        matrix hidden=layer1.relu();
        matrix layer2=hidden*weight2+bias2;
        matrix output=layer2.softmax();
        return output;
    }

};
int main()
{
    // 输入：[1, 2]
    matrix x(1, 2);
    x.set(0, 0, 1.0f);
    x.set(0, 1, 2.0f);

    // 第一层权重：单位矩阵
    matrix w1(2, 2);
    w1.set(0, 0, 1.0f);
    w1.set(0, 1, 0.0f);
    w1.set(1, 0, 0.0f);
    w1.set(1, 1, 1.0f);

    // 第一层偏置：[-2, 1]
    matrix b1(1, 2);
    b1.set(0, 0, -2.0f);
    b1.set(0, 1, 1.0f);

    // 第二层权重：单位矩阵
    matrix w2(2, 2);
    w2.set(0, 0, 1.0f);
    w2.set(0, 1, 0.0f);
    w2.set(1, 0, 0.0f);
    w2.set(1, 1, 1.0f);

    // 第二层偏置：[0, 0]
    matrix b2(1, 2);

    model testModel(w1, b1, w2, b2);
    matrix output = testModel.forward(x);

    cout << "模型输出：" << endl;
    output.print();

    return 0;
}
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
int main()
{
    matrix a(1, 3);
    a.set(0, 0, 1001.0f);
    a.set(0, 1, 1002.0f);
    a.set(0, 2, 1003.0f);

    matrix result = a.softmax();

    cout << "SoftMax结果：" << endl;
    result.print();

    float sum = 0.0f;
    for (int c = 0; c < result.getcols(); c++)
    {
        sum += result.get(0, c);
    }

    cout << "概率总和：" << sum << endl;

    return 0;
}
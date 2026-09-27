    #include"matrix.hpp"
    #include"nlohmann/json.hpp"
    using json = nlohmann::json;
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
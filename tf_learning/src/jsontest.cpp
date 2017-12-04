// testJson.cpp : 定义控制台应用程序的入口点。
//

// #include "stdafx.h"
#include <iostream>
#include <fstream>
//添加需要的头文件
#include "json/json.h"

using namespace std;

//链接需要的库文件
#ifdef _DEBUG
#pragma comment(lib,"lib_json_d.lib")
#else
#pragma comment(lib,"lib_json.lib")
#endif

int main(int argc, char** argv)
{
    cout<<"测试json写入"<<endl;
    Json::Value jsonRoot;
    Json::Value jsonItem;
    jsonItem["item1"] = "第一个条目";
    jsonItem["item2"] = "第二个条目";
    jsonItem["item3"] = 3;
    jsonRoot.append(jsonItem);
    jsonItem.clear();//清除上面已经赋值的项
    jsonItem["item1"]="1";
    jsonItem["item2"]="oo";
    jsonItem["item3"]=3.0F;
    jsonRoot.append(jsonItem);
    cout<<jsonRoot.toStyledString()<<endl;

    cout<<"测试json写入到文件"<<endl;

    ofstream ofs;
    ofs.open("/home/relaybot/api_ws/src/tf_learning/src/test1.json");
    ofs<<jsonRoot.toStyledString();
    ofs.close();

    cout<<"测试json读取"<<endl;
    string sJson = jsonRoot.toStyledString();
    jsonRoot.clear();

    ifstream ifs;
    ifs.open("/home/relaybot/api_ws/src/tf_learning/src/test1.json");
    Json::Reader reader;
    Json::Value root;
    Json::Value test_root; 
    
    if (!reader.parse(ifs, root, false))
    {
        return -1;
        cout<<"wrong input parse";
    }
    ifs.close();
    string item1,item2;
    int size = root.size();
    cout<<"size: "<<size;
    for (int i=0; i<size; ++i)
    {
        item1 = root[i]["item1"].asString();
        item2 = root[i]["item2"].asString();
        if(root[i].isMember("item3")){
            cout<<"heihei:"<<root[i]["item3"]<<endl;
            if(root[i]["item3"].asInt()==3){
                cout<<"we found it@@@@@@@@@@@@@@"<<endl;
                
            }
        }
 
       cout<<item1<<" "<<item2<<endl;
       
    }
    


    cout<<root.toStyledString()<<endl;
    root[1].removeMember("item3"); 
    cout<<root.toStyledString()<<endl;
   
    cout<<test_root.toStyledString()<<endl;
    return 0;
    
}

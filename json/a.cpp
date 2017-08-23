//json.cpp
#include<iostream>
#include<json/json.h>

using namespace std;

int main()
{
    string test="{\"id\":1,\"name\":\"kurama\"}";
    Json::Reader  reader;
    Json::Value   value;

    if(reader.parse(test,value))
    {
        if(!value["id"].isNull())
        {
            cout<<value["id"].asInt()<<endl;
            cout<<value["name"].asString()<<endl;
        }
    }
    return 0;
}
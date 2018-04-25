#pragma once
#include <string>
#include <jsoncppp/json/json.h>

//此处的Dara相当于服务给客户端提供的接口
struct Data{
    std::string name;
    std::string school;
    std::string msg;
    std::string cmd;

    //序列化函数和反序列化函数
    void UnSerialize(const std::string& input){
        //char buf[1024] = {0};
        //sprintf(buf,"{name:%s,school:%s,msg:%s,cmd:%s}",
        //        name.c_str(),school.c_str(),msg.c_str(),cmd.c_str());
        //*output = buf;
         
        //用第三方库jsoncpp来进行序列化
        //可以把Json::Value 近似理解成一个unordered_map
        Json::Value value;
        value["name"] = name;
        value["school"] = school;
        value["msg"] = msg;
        value["cmd"] = cmd;
        Json::FastWriter writer;
        *output = writer.write(value);
        return;
    }

    //反序列化,字符串变对象 
    void Serialize(std::string* output){
        Json::Value value;
        Json::Reader reader;
        reader.parse(input,value);
        //判断是否是string类型
        if(value["name"].isString()){
            name = value["name"].asString();
        }
        if(value["name"].isString()){
            school = value["school"].asString();
        }
        if(value["name"].isString()){
            msg = value["value"].asString();
        }
        if(value["name"].isString()){
            cmd = value["cmd"].asString();
        }
        return;
    }
}；

#pragma once
#pragma once
#include <iostream>
#include <sys/time.h>
////////////////////////////////
//此文件放置一些工具类和函数///
///////////////////////////////


class TimeUtil{
public:
    //获取到秒级时间戳
    static int64_t TimeStamp(){
       struct timeval tv;
       gettimeofday(&tv, NULL);
       return tv.tv_sec;
    }

};

enum LogLevel{
    INFO,    //日志
    WARNING, //警告
    ERROR,   //一般错误
    CRITIAL, //致命错误
};

//inline 防止与其他函数发生冲定义
inline std::ostream& Log(LogLevel level, const char* file, int line){
    std::string prefix = "I";
    if(level == WARNING){
        prefix = "W";
    }else if(level == ERROR){
        prefix = "E";
    }else if(level == CRITIAL){
        prefix = "C";
    }

    TimeUtil t;
    std::cout<< "[" << prefix << TimeUtil::TimeStamp() << " "
            << file << ":" << line << "] ";
    return std::cout;
}

//为了正确获取行号和文件，所以用宏
//__LINE__ __FILE__
#define LOG(level) Log(level, __FILE__, __LINE)


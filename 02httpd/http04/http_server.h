#pragma once
#pragma once
#include <string>
#include <unordered_map>

namespace http_server{

typedef std::unordered_map<std::string, std::string> Header;

struct Request{
    //反序列化转换成Request对象
    std::string method;//请求方法
    std::string url;

    //例如www.baidu.com/index.html?kwd="cpp"
    std::string url_path;     //路径 例如index.html
    std::string query_string; //kwd="cpp"
    //std::string version;    //暂时不考虑这个版本好
    Header header;            //一组字符串键值对
    std::string body;         //http请求body
};

struct Response{
    int code;                 //状态码
    std::string desc;         //状态码描述
    //std::sting version
    Header header;            //响应报文中键值对，即header数据
    std::string body;         //http响应body数据
};

//当前请求的上下文，包含了这次
//当前请求的上下文
//方便进行扩展
//为了参数通一和扩展性好。
struct Context{
    Request req;
    Response resp;
};

//实现核心流程的类
class HttpServer{
public:
    //初始化，启动服务器
    int Start(const std::string& ip, short port);
    //根据HTTP请求字符串，进行反序列化
    //输入一个字符串，输出respquest对象
    //这部分和业务逻辑没关系
    int ReadOneRequest(Context *context);

    //根据Response对象，拼接成一个字符串，写回到客户端，反序列化。
    int WriteOneResponse(Context *context);

    //根据Request对象，构造Response对象（对于相应报头的格式，是核心的计算）
    //是整个项目的业务逻辑！
    int HandlerRequest(Context *context);
};

}//end http_server


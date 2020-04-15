#ifndef _MYJSON_HPP
#define _MYJSON_HPP


#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cqcppsdk/cqcppsdk.hpp>
#include <windows.h>

using namespace nlohmann;
using namespace std;

class MyJson 
    {
        public:
            //conf_json 在init.cpp中定义 调用时务必初始化 该json仅用于存储与本地相关的设置
            json conf_json;
            //搜索结果json
            json search_json;


        public:
            //许可群
            vector<int64_t> Enable_Group;
            //adminer
            vector<int64_t> Adminer_List;
            //API_KEY
            string API_KEY;
            //相似度
            double Filter;
            //return
            string LowSim;
            //number
            int num;

        public:
            string imgurl;

            double sim;
            int long_limit;
            int short_limit;

            string res;

        private:
            //json写入文件
            bool write_json(string path, json jroot);
            //文件读取json
            bool read_json(string path, json jroot);
            //解析json中的数组到 vector
            template <typename T > bool dis_jarray (vector<T> &Varray, json Jarray);
            
        public:
            //将conf_json写到文件里
            void json2file();
            //从文件中读取json_conf
            void file2json();
            void all2read();
            void all2write();

            
            void write_num();
        public:
            void get_serch(string str);
            

        public:
            //从conf_json读取许可群列表
            void read_EnbGrp();
            //从conf_json读取管理员列表
            void read_AdLis();
            //从conf_json读取API_KEY
            void read_APIkey();
            void read_Filter();
            void read_LowSim();
            void read_num();
            
            //写入群列表到 conf_json
            void write_EnbGrp();
            //写入管理员列表到 conf_json
            void write_AdLis();
            //写入API_KEY到 conf_json
            void write_APIkey();
            void write_Filter();
            void write_LowSim();
            

        private:
            bool del_rjson();

        public:
            MyJson();
    };









#endif
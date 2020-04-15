#include "myJson.hpp"
#include "cqfun.hpp"
#include "mycurl.hpp"

#include <cqcppsdk/cqcppsdk.hpp>
#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace std;

//初始化于init.cpp
extern string appDir;

MyJson::MyJson()
    {
        long_limit = 100;
        short_limit = 3;
    }

//json写入文件
bool MyJson::write_json(string path, json jroot) 
    {
        ofstream file;
        file.open(cq::utils::ansi(path));
        if (!file.good()) {
            return false;
        } else {
            //文件可读性考虑
            file << this->conf_json.dump(4);
            file.close();
            return true;
        }
    }

//文件读取json
bool MyJson::read_json(string path, json jroot) 
    {
        ifstream file;
        file.open(cq::utils::ansi(path));
        if (!file.good()) {
            //如果未成功打开文件，新建文件
            fstream newfile;
            newfile.open(cq::utils::ansi(path), ios::out);
            json basejson = R"(
            {
                "API-key": "",
                "Adminer": [],
                "EnableGroup": [],
                "Filter": 33.33,
                "LowSimRetuen": "识图是有极限,所以,我不识图了,JOJO!",
                "num":0
            })"_json;
            newfile << basejson.dump(4);
            newfile.close();
            this->conf_json = basejson;
        } else {
            this->conf_json << file;
            file.close();
            return true;
        }
    }

//解析json数组 调用之前务必确保 vector<类型> 与 json_array 内的值保持一致
template <typename T >
bool MyJson::dis_jarray ( vector<T> &Varray, json Jarray) 
    {

        if( !Jarray.is_array() || Jarray.is_null() ) {
            return false;
        }

        for( auto jit : Jarray ){
            Varray.emplace_back(static_cast<T> (jit));
        }

    }



//从conf_json读取许可群列表到变量
void MyJson::read_EnbGrp() 
    {
        cq::logging::debug("redE","do");
        //得到conf_json中的关键数组
        auto jarray = this -> conf_json.at("/EnableGroup"_json_pointer);

        //清除滞留数据
        this -> Enable_Group.clear();

        //解析关键数组到 EnbGrp
        try {
            dis_jarray(this->Enable_Group,jarray);
        } catch (exception& e) {
            cq::logging::error("read_EnbGrp", e.what());
        }
        
    }

//从conf_json读取管理员列表到变量
void MyJson::read_AdLis() 
    {
        //得到conf_json中的关键数组
        auto jarray = this -> conf_json.at("/Adminer"_json_pointer);

        //清除滞留数据
        this -> Adminer_List.clear();

        //解析关键数组到 AdLis
        try {
            dis_jarray(this->Adminer_List,jarray);
        } catch (exception& e) {
            cq::logging::error("read_AdLis", e.what());
        }
    }

//从conf_json读取 API_KEY
void MyJson::read_APIkey() 
    {
        API_KEY = conf_json.value("API-key","null");
    }

void MyJson::read_Filter() 
    {
        Filter = conf_json.value("Filter",80);
    }

void MyJson::read_LowSim() 
    {
        LowSim = conf_json.value("LowSimRetuen","识图是有极限的,所以，我不识图了！");
    }

void MyJson::read_num() 
    {
        num = conf_json.value("num",0);
    }


//从变量保存到conf_json
void MyJson::write_EnbGrp() 
    {
        json j_temp(Enable_Group);
        conf_json.at("/EnableGroup"_json_pointer) = j_temp;
    }

// 从变量保存到conf_json
void MyJson::write_AdLis() 
    {
        json j_temp(Adminer_List);
        conf_json.at("/Adminer"_json_pointer) = j_temp;
    }

void MyJson::write_APIkey() 
    {
        conf_json["API-key"] = API_KEY;
    }

void MyJson::write_Filter() 
    {
        conf_json["Filter"] = Filter;
    }

void MyJson::write_LowSim() 
    {
        conf_json["LowSimRetuen"] = LowSim;
    }

void MyJson::write_num() 
    {
        conf_json["num"] = num;
    }




//将conf_json写到文件里
void MyJson::json2file() 
    {
        try {
            write_json((appDir + "conf.json"), conf_json);
        } catch (exception& e) {
            cq::logging::error("read_json", e.what());
        }
        cq::logging::debug("json2file","done");
    }

//从文件中读取conf_json
void MyJson::file2json() 
    {
        try {
            read_json((appDir + "conf.json"), conf_json);
        } catch (exception& e) {
            cq::logging::error("read_json", e.what());
        }
    }


void MyJson::all2read() 
    {
        read_EnbGrp();
        read_AdLis();
        read_APIkey();
        read_Filter();
        read_LowSim();
        read_num();
    }

void MyJson::all2write() 
    {
        write_EnbGrp();
        write_AdLis();
        write_APIkey();
        write_Filter();
        write_LowSim();
        write_num();
    }



void MyJson::get_serch(string str) 
    {
        num++;
        write_num();
        imgurl = cqfun::get_img_url(str);
        stringstream buf,hbuf;
        if( mycurl::get_search_result(hbuf,buf,imgurl) ) {
            search_json << buf;
            cq::logging::debug("result_head" , hbuf.str());
            del_rjson();
        } else {
            res = "看来哪里出现问题了呢，查看日志报告给开发者吧";
            cq::logging::debug("result_head" , hbuf.str());
        }
    }

bool MyJson::del_rjson () 
    {
        json jheader = search_json.at("/header"_json_pointer);
        json jresults = search_json.at("/results"_json_pointer);

        long_limit = jheader.value("long_remaining",0);
        short_limit = jheader.value("short_remaining",0);
        
        std::string similarity = jresults.at("/0/header/similarity"_json_pointer);
        sim = stod(similarity);

        if( sim < Filter ) {
            res = LowSim;
            return true;
        }

        auto data = jresults.at("/0/data"_json_pointer);
        res = "相似度:" + similarity ;

        if(data.contains("pixiv_id")){
            auto title = data.value("title","null");
            auto pixiv_id = to_string(data.value("pixiv_id",0));
            auto member_name = data.value("member_name","null");
            auto member_id = to_string(data.value("member_id",0));
            std::string ext_urls = data.at("/ext_urls/0"_json_pointer);
            res.append("%\r\npixiv_id:" + pixiv_id + "\r\n标题:" + title + "\r\n作者名:" + member_name  + "\r\n作者id:" + member_id +"\r\n原图链接:" + ext_urls);
        } else if(data.contains("konachan_id")||data.contains("danbooru_id")||data.contains("gelbooru_id")){
            auto konachan_id = to_string(data.value("konachan_id",0));
            auto danbooru_id = to_string(data.value("danbooru_id",0));
            auto gelbooru_id = to_string(data.value("gelbooru_id",0));
            auto creator = data.value("creator","null");
            std::string ext_urls = data.at("/ext_urls/0"_json_pointer);
            res.append("%\r\nkonachan_id:" + konachan_id + "\r\ndanbooru_id:" + danbooru_id + "\r\ngelbooru_id:" + gelbooru_id +"\r\n作者:" + creator + "\r\n原图链接:" + ext_urls);
        } else {
            std::string ext_urls = data.at("/ext_urls/0"_json_pointer);
            res = "未能在主流站点找到该图片,可尝试以下结果:\r\n" + ext_urls;
        }
        return true;
    }
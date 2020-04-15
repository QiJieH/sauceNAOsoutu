#include "cqfun.hpp"
#include "mycurl.hpp"
#include "myJson.hpp"
#include "gui.hpp"

extern MyJson conf;

auto cqfun::get_image_name(string e , int requestcode ) -> string
    {
        auto keywordindex = e.find("[CQ:image,file=");
        std::string picname;
        picname.assign(e, keywordindex+15, e.length()-keywordindex-16);
        if(requestcode==1){return cq::get_image(picname);}
            else{return picname;}
    }
auto cqfun::get_img_url(string e) -> string
    {
        auto filepath =".//data//image//" + get_image_name(e,0) + ".cqimg";
        string res;
        fstream f(filepath,ios::in||ios::out);
        if(f){
            for(int i=0;i<6;i++){
                getline(f,res);
            }
            f.close();
            // res = res.substr(4);
            return res;
        } else {
            return "failed";
        }
    }


bool msfun::is_adminer( int64_t uid ) 
    {
        for( auto &it : conf.Adminer_List )
        {
            if( it == uid ) { return true; }
        }
        return false;
    }

bool msfun::is_enablegrp( int64_t gid )
    {
        for( auto &it : conf.Enable_Group )
        {
            if( it == gid ) { return true; }
        }
        return false;
    }

bool msfun::is_command( const GroupMessageEvent &e  )
    {   
        if( e.message.find("#搜图") == 0 )
            {
                const auto msg = Message(e.message);
                for (auto &seg : msg) 
                    {
                        if(seg.type == "image")
                            {
                                auto str = e.message.substr(7);
                                conf.get_serch(str);
                                send_group_message(e.group_id,conf.res);
                                e.block();
                                return true;
                            }
                    }
                // return false; 关键词重复
            }


        if( e.message.find("#搜图余量") == 0 && is_adminer(e.user_id) )
            {
                string res;
                res = "24小时余量：" + to_string(conf.long_limit) + "\r\n30秒内余量：" + to_string(conf.short_limit) + "\r\n累计搜图：" + to_string(conf.num);
                send_group_message(e.group_id,res);
                return true;
            }
        
        if(e.message.find("#搜图developer") == 0)
            {
                send_group_message(e.group_id,"developed by QiJieH\r\npowered by CoolQ\r\nthank for richardchien\r\nGithub:https://github.com/QiJieH");
                return true;
            }
        
        return false;
    }

bool msfun::is_switch( const GroupMessageEvent &e)
    {
        if( e.message.find("#搜图开启") == 0 )
        {   
            if( is_adminer( e.user_id ) )
            {   
                if(!is_enablegrp(e.group_id)) 
                {
                    conf.Enable_Group.emplace_back(e.group_id);
                    conf.write_EnbGrp();
                    conf.json2file();
                    send_group_message(e.group_id,mycurl::get_hitokoto());
                    return true;
                }
                send_group_message(e.group_id,"已经开启了哦");
                return false;
            }
            return false;
        }

        if( e.message.find("#搜图关闭") == 0 )
        {   
            if( is_adminer( e.user_id ) )
            {   
                if(is_enablegrp(e.group_id)) 
                {
                    conf.Enable_Group.erase(std::remove(std::begin(conf.Enable_Group), std::end(conf.Enable_Group),e.group_id), std::end(conf.Enable_Group));
                    conf.write_EnbGrp();
                    conf.json2file();
                    send_group_message(e.group_id,mycurl::get_hitokoto());
                    return true;
                }
                send_group_message(e.group_id,"已经关闭了哦");
                return false;
            }
            return false;
        }

        return false;
    }


bool msfun::is_register( const PrivateMessageEvent &e ) 
    {   
        if(e.message.find("#搜图管理员注册") == 0)
            {
                if( ! msfun::is_adminer(e.user_id))
                    {
                        conf.Adminer_List.emplace_back(e.user_id);
                        conf.write_AdLis();
                        conf.json2file();
                        send_private_message(e.user_id,"已注册为搜图管理员\r\n发送群聊消息\r\n   #搜图开启\r\n   #搜图关闭\r\n来开关该群搜图功能");
                        e.block();
                    }
                    send_private_message(e.user_id,"已存在于管理列表");
                    return false;
            }
            return false;
        
    }

bool msfun::is_check(const PrivateMessageEvent &e)
    {
        if(e.message.find("#搜图余量") == 0 && is_adminer(e.user_id))
            {
                string res;
                res = "24小时余量：" + to_string(conf.long_limit) + "\r\n30秒内余量：" + to_string(conf.short_limit) + "\r\n累计搜图：" + to_string(conf.num);
                send_private_message(e.user_id,res);
                return true;
            }
    }



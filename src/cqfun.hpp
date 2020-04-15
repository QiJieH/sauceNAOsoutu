#ifndef _CQFUN_HPP
#define _CQFUN_HPP

#include <cqcppsdk/cqcppsdk.hpp>
#include <iostream>
#include <fstream>

#include "myJson.hpp"


extern MyJson conf;

using namespace std;
using namespace cq;
using Message = cq::message::Message;

namespace cqfun 
    {
        auto get_image_name(string e , int requestcode )-> string;
        auto get_img_url(string e)-> string;
    };

namespace msfun 
    {
        bool is_adminer( int64_t uid );
        bool is_enablegrp( int64_t gid );
        bool is_command(const GroupMessageEvent &e );
        bool is_switch(const GroupMessageEvent &e);
        bool is_register( const PrivateMessageEvent &e );
        bool is_check(const PrivateMessageEvent &e);
    };



#endif
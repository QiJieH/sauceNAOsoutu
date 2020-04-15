#ifndef _MYCURL_HPP
#define _MYCURL_HPP

#include <cqcppsdk/cqcppsdk.hpp>
#include <curl/curl.h>
#include <string>

namespace mycurl 
    {

    bool get_search_result(std::stringstream &http_head, std::stringstream &http_result, std::string imgurl);

    std::string get_hitokoto();

    }












#endif
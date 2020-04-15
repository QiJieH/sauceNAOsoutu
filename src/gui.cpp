#include "myJson.hpp"
#include "mycurl.hpp"
#include "gui.hpp"

#include <regex>
#include <string.h>


using namespace nana;
using namespace std;

extern MyJson conf;

//窗口布局
void MainWin::init_place_() 
    {
        place_.div(
            "margin=[0,6,4,6] vert "
            "< margin=[4,0,3,0]"
                "<margin=[0,10,0,0] left_area> <vert right_area "
                    "<margin=[0,0,3,0] right_top> <right_bot> "
                "> "
            "> "
            "< weight=10% < weight=13% margin=[6,0,7,0] moreapp> < weight=13% margin=[6,0,7,13] feed>  <  > <weight=28% margin=[6,80,7,0] refresh> <weight=8% margin=[6,0,7,0] version > < weight=13% margin=[6,0,7,0] savebto> > "
        );
    }


//构造组件 注意顺序，先绑定组件再定义组件细节
void MainWin::init_group_EG() 
    {
        EnableGroup_.create(*this);
        EnableGroup_.caption("群控开关");

        EnableGroup_.div("<margin=[3,8] list >");
        //构造组内组件
        GroupList_.create(EnableGroup_);
        GroupList_.checkable(true);
        GroupList_.append_header(u8"群名");
        GroupList_.append_header(u8"群ID");

        GroupList_.sortable(false);
        GroupList_.bgcolor(static_cast<color_rgb>(0xEEEEEE));

        GroupList_.scheme().header_bgcolor = nana::colors::white;
        //读取支持群组
        //conf.read_EnbGrp();
        //载入所有群组
        auto grouplist = cq::get_group_list();
        for (auto temp : grouplist) {
            GroupList_.at(0).append({temp.group_name, to_string(temp.group_id)});
        }
        //在所有群组中勾选已支持群组
        auto size = GroupList_.size_item(0);
        for(size_t i = 0; i < size; i++){
            string buf = GroupList_.at(0).at(i).text(1);
            for(auto it : conf.Enable_Group){
                if( to_string(it) == buf ) {
                    GroupList_.at(0).at(i).check(true);
                }
            }
        }
        //插入布局并刷新
        EnableGroup_["list"] << GroupList_;
        EnableGroup_.collocate();
    }

void MainWin::init_group_AS() 
    {
        OtherSet_.create(*this);
        OtherSet_.caption(u8"其它设置");

        OtherSet_.div(
            "margin=[3,8]"
            "< weight=24%  vert item>"
                "< input vert"
                    "< vert <> <weight=70% ADinput> <> >"
                    "< vert <> <weight=70% Akyinput> <> >"
                    "< vert <> <weight=70% Filinput> <> >"
                    "< vert <> <weight=70% Lowsiminput> <> >"
                    ">"
        );

        //项目名
        lab_Adminer_.create(OtherSet_);
        lab_Adminer_.caption(u8"管理员ID:");
        lab_Adminer_.text_align(align::left,align_v::center);
        lab_Adminer_.tooltip(u8"多位管理员使用空格隔开");
        lab_Adminer_.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        OtherSet_["item"] << lab_Adminer_;

        lab_APIkey_.create(OtherSet_);
        lab_APIkey_.caption(u8"API-KEY:");
        lab_APIkey_.text_align(align::left,align_v::center);
        lab_APIkey_.tooltip(u8"如果您不知道什么是key,请勿填写");
        lab_APIkey_.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        OtherSet_["item"] << lab_APIkey_;

        lab_Filter_.create(OtherSet_);
        lab_Filter_.caption(u8"最低相似度:");
        lab_Filter_.text_align(align::left,align_v::center);
        lab_Filter_.tooltip(u8"低于该值的图片将不展示结果");
        lab_Filter_.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        OtherSet_["item"] << lab_Filter_;

        lab_Return_.create(OtherSet_);
        lab_Return_.caption(u8"过低返回:");
        lab_Return_.text_align(align::left,align_v::center);
        lab_Return_.tooltip(u8"当低于设置相似度时,发送本消息");
        lab_Return_.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        OtherSet_["item"] << lab_Return_;

        //conf.all2read();
        // conf.read_AdLis();
        // conf.read_APIkey();
        // conf.read_EnbGrp();
        // conf.read_Filter();
        // conf.read_LowSim();

        //管理员设置
        AdSet_.create(OtherSet_);
        AdSet_.line_wrapped(false);
        AdSet_.multi_lines(false);
        AdSet_.fgcolor(static_cast<color_rgb>(0x333333));

        string list;
        for( auto temp : conf.Adminer_List ) {
            list.append( to_string(temp) + " ");
        }
        AdSet_.caption(list);


        //KEY设置
        AkySet_.create(OtherSet_);
        AkySet_.line_wrapped(false);
        AkySet_.multi_lines(false);
        AkySet_.fgcolor(static_cast<color_rgb>(0x333333));
        AkySet_.caption(conf.API_KEY);


        //相似度设置
        FilSet_.create(OtherSet_);
        FilSet_.line_wrapped(false);
        FilSet_.multi_lines(false);
        FilSet_.fgcolor(static_cast<color_rgb>(0x333333));
        FilSet_.caption(to_string(conf.Filter));


        //return
        RetSet_.create(OtherSet_);
        RetSet_.line_wrapped(false);
        RetSet_.multi_lines(false);
        RetSet_.fgcolor(static_cast<color_rgb>(0x333333));
        RetSet_.caption(conf.LowSim);


        //插入布局
        OtherSet_["ADinput"] << AdSet_;
        OtherSet_["Akyinput"] << AkySet_;
        OtherSet_["Filinput"] << FilSet_;
        OtherSet_["Lowsiminput"] << RetSet_;
    }


void MainWin::init_group_IS() 
    {
        InfoShow_.create(*this);
        InfoShow_.caption(u8"信息显示");
        InfoShow_.div(
            "< vert margin=[4,10,0,10] item arrange=[20%,20%,10%,50%] >"
        );

        limits_long_.create(InfoShow_);
        limits_long_.caption(u8"24小时余量: " + to_string(conf.long_limit));
        limits_long_.tooltip(u8"使用key可获得更多");
        limits_long_.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        limits_long_.text_align(align::left,align_v::center);

        limits_short_.create(InfoShow_);
        limits_short_.caption(u8"30秒余量: " + to_string(conf.short_limit) );
        limits_short_.tooltip(u8"使用key可获得更多");
        limits_short_.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        limits_long_.text_align(align::left,align_v::center);

        numbers_.create(InfoShow_);
        numbers_.caption(u8"累计搜索：" + to_string(conf.num) );
        numbers_.tooltip(u8"已消灭的bug");
        numbers_.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        numbers_.text_align(align::left,align_v::center);

        hitokoto.create(InfoShow_);
        hitokoto.caption(mycurl::get_hitokoto());
        hitokoto.tooltip(u8"一言");
        hitokoto.fgcolor(static_cast<color_rgb>(0x444444));
        hitokoto.bgcolor(static_cast<color_rgb>(0xC9C5BE));
        hitokoto.text_align(align::left,align_v::center);


        InfoShow_["item"] << limits_long_ << limits_short_ << numbers_ << hitokoto;
        InfoShow_.collocate();


    }

//按钮构造
void MainWin::init_savebto() 
    {
        savebto.create(*this);
        savebto.caption(u8"保存");
        savebto.enable_focus_color(false);

        savebto.events().click([this](const nana::arg_click& event){this->save_button_event(event);});
    }

void MainWin::init_moreappbto() 
    {
        moreappbto.create(*this);
        moreappbto.caption(u8"更多插件");
        moreappbto.enable_focus_color(false);

        moreappbto.events().click([this](const nana::arg_click& event) {this->moreapp_button_event(event);});
    }

void MainWin::init_feedbto() 
    {
        feedbto.create(*this);
        feedbto.caption(u8"反馈");
        moreappbto.enable_focus_color(false);

        feedbto.events().click([this](const nana::arg_click& event) {this->feed_button_event(event);});
    }


void MainWin::init_versionlab() 
    {
        versionlab.create(*this);
        versionlab.caption("2.0.0");
        versionlab.text_align(align::left,align_v::center);
        versionlab.tooltip(u8"点击反馈加入Q群852346462获取更新消息");
        versionlab.fgcolor(nana::colors::gray);
    }

void MainWin::init_all_widgets() 
    {
        //构造布局文件
        init_place_();
        //构造组件
        init_group_EG();
        init_group_AS();
        init_group_IS();
        init_savebto();
        init_moreappbto();
        init_feedbto();
        init_versionlab();
        // init_refreshbto();
    }

//事件
bool MainWin::save_button_event(const nana::arg_click& event) 
    {   

        //群控
        //清空原有
        conf.Enable_Group.clear();
        //加载勾选
        for( auto temp : GroupList_.checked() ) {
            string buf = GroupList_.at(temp).text(1);
            conf.Enable_Group.emplace_back(static_cast<int64_t>(stoll(buf)));
        }


        //ADset
        if(!std::regex_match(AdSet_.text(), regex("[0-9\\s]*")) )
            {
                msgbox msg{*this,u8"sauceNAO搜图"};
                msg << "管理员ID包含非法字符";
                msg.show();
                return false;
            }
        conf.Adminer_List.clear();
        string list = AdSet_.text();
        char *token = strtok(list.data(), " ");
        while (token  != NULL) {
            conf.Adminer_List.emplace_back(stoll(token));
            token  = strtok(NULL, " ");
        }

        //APIKEY set
        if(!std::regex_match(AkySet_.text(), regex("[a-z0-9]*")) )
            {
                msgbox msg{*this,u8"sauceNAO搜图"};
                msg << "APIkey包含非法字符";
                msg.show();
                return false;
            }
        conf.API_KEY.clear();
        conf.API_KEY = AkySet_.text();

        //Filter
        if(!std::regex_match(FilSet_.text(), regex("[0-9.]*") ))
            {   
                msgbox msg{*this,u8"sauceNAO搜图"};
                msg << "最低相似度包含非法字符";
                msg.show();
                return false;
            }
        if( FilSet_.to_double() <= 0 && FilSet_.to_double() >=100 )
            {
                msgbox msg{*this,u8"sauceNAO搜图"};
                msg << "最低相似度值域错误";
                msg.show();
                return false;
            }
        if( FilSet_.text().empty() )
            {
                msgbox msg{*this,u8"sauceNAO搜图"};
                msg << "最低相似度不能为空";
                msg.show();
                return false;
            }
        conf.Filter = FilSet_.to_double();


        //LowSim
        if( RetSet_.text().empty() )
            {
                msgbox msg{*this,u8"sauceNAO搜图"};
                msg << "失败返回不能为空";
                msg.show();
                return false;
            }
        conf.LowSim = RetSet_.text();

        //保存现有
        conf.all2write();
        conf.json2file();

        //重载现有
        conf.all2read();

        //刷新页面
        this->collocate();

        msgbox msg{*this,u8"sauceNAO搜图"};
        msg << "保存成功";
        msg.show();

        return true;

    }

void MainWin::moreapp_button_event(const nana::arg_click& event) 
    {
        ShellExecute(NULL,"open","https://cqp.cc/home.php?mod=space&uid=863187&do=thread&view=me&from=space",NULL,NULL,SW_SHOWNORMAL);
    }

void MainWin::feed_button_event(const nana::arg_click& event) 
    {
        ShellExecute(NULL,"open","https://shang.qq.com/wpa/qunwpa?idkey=8e83beb344b5dbf69ecdf5d21addec9b1aca8878dfb22fdcae871a20f5be7248",NULL,NULL,SW_SHOWNORMAL);
    }





MainWin::MainWin() : nana::form (API::make_center(620,430), appear::decorate<appear::minimize>())  
    {
        //载入配置
        conf.all2read();
        //窗口设置
        this->caption(u8"sauceNAO搜图");
        this->size(nana::size(620, 430));


        //构造布局和组件
        init_all_widgets();
        //读入布局文件
        place_.bind(*this);
        //插入组件
        place_["left_area"] << EnableGroup_;
        place_["right_top"] << OtherSet_;
        place_["right_bot"] << InfoShow_;

        place_["savebto"]   << savebto;
        place_["moreapp"]   << moreappbto;
        place_["feed"]      << feedbto;
        //place_["hitokoto"]  << hitokoto;
        place_["version"]   << versionlab;
        // place_["refresh"]   << refreshbto;



        //刷新页面
        place_.collocate();
        this->collocate();
    }

void MainWin::openWin()
    {
        this->show();
        exec();
    }





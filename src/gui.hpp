#ifndef _GUI_HPP
#define _GUI_HPP

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/spinbox.hpp>

using namespace nana;

class MainWin : public nana::form 
    {
        private:
            //窗口主布局
            place place_;
            //组件
            group EnableGroup_;
                listbox GroupList_;
            group OtherSet_;
                label lab_Adminer_;
                    textbox AdSet_;
                label lab_APIkey_;
                    textbox AkySet_;
                label lab_Filter_;
                    textbox FilSet_;
                label lab_Return_;
                    textbox RetSet_;
                    
            group InfoShow_;
                label limits_long_;
                label limits_short_;
                label numbers_;
                label hitokoto;

            button savebto;
            button moreappbto;
            button feedbto;
            // button refreshbto;
            label versionlab;
            //messagebox
            //msgbox savemsg_;

        private:
            //构造布局文件
            void init_place_();
            //构造组件
            void init_group_EG();
            void init_group_AS();
            void init_group_IS();
            void init_savebto();
            void init_moreappbto();
            void init_feedbto();
            void init_versionlab();
            // void init_refreshbto();
        private:
            //init all widgets
            void init_all_widgets();
        private:
            //按钮事件
            bool save_button_event(const nana::arg_click& event);
            void moreapp_button_event(const nana::arg_click& event);
            void feed_button_event(const nana::arg_click& event);
        public:
            //窗口设置与布局
            MainWin();
            //go
            void openWin();
            
    };

#endif
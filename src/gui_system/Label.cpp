#include "Label.hpp"
#include "../misc/Misc.hpp"
#include "../misc/Log.hpp"
#include "../misc/MediaManager.hpp"

using namespace gui;
using namespace std;

Label::Label(const ControlID id, const ControlPos& pos, const std::string& text, const std::string& font) :
	Control(id, pos, text),
	BStext_(GET_BITMAP_FONT(font.c_str()))
{
	BStext_.SetText(text_);
}

void Label::Update()
{
    if(ptr_ == NULL)return;

    switch(mode_){
        case LINKED_INT:
            text_ = str_sprintf("%d", *((int *)ptr_));
            break;
        case LINKED_CHAR:
            text_ = str_sprintf("%s", (char *)ptr_);
            break;
    }
    BStext_.SetText(text_);
}

void Label::SetText(const std::string s)
{
    text_ = s;
    BStext_.SetText(s);
}

void Label::Render(sf::RenderTarget& app) const
{
    app.Draw(BStext_);
}

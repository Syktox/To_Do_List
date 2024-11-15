module;

#include <wx/wx.h>

export module TodoListGUI;

import MainFrame;

export class TodoListGUI : public wxApp
{
public:
    virtual bool OnInit();
};

bool TodoListGUI::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show();
    return true;
};


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
    MainFrame* mainFrame = new MainFrame("Todo List GUI");
    mainFrame->Show();
    return true;
};


module;

#include <wx/wx.h>

export module TodoListGUI;

import MainFrame;

export class TodoListGUI : public wxApp
{
public:
    bool OnInit();
};

wxIMPLEMENT_APP(TodoListGUI);

bool TodoListGUI::OnInit()
{
    MainFrame* mainFrame = new MainFrame("Todo List GUI");
    mainFrame->Show();
    return true;
};


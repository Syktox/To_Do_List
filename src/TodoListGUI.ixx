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
    frame->SetMinSize(wxSize(300, 400));
    frame->SetMaxSize(wxSize(300, 400));
    frame->CenterOnScreen();
    frame->SetBackgroundColour(*wxWHITE);
    frame->Show();
    return true;
};


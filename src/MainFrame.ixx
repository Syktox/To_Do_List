module;

#include <wx/wx.h>

export module MainFrame;

export class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
};

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) { }

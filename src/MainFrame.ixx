module;

#include <wx/wx.h>
#include <wx/icon.h>



export module MainFrame;

export class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAddButtonClicked(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1,
    ID_ADD = 2
};

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Todo List")
{
    // Set Icon
    wxIcon appIcon("..\\resources\\appIcon.ico", wxBITMAP_TYPE_ICO);
    SetIcon(appIcon);
    
    // Menu Bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "Hello world");
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );


    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxButton* AddButton = new wxButton(panel, wxID_ANY, wxT("Add"),
                        wxPoint(150,50), wxSize(100, 35));
    AddButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
    
    
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
 
    Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    event.Skip();   
    Close(true);
}
 
void MainFrame::OnAbout(wxCommandEvent& event)
{
    event.Skip();   
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
 
void MainFrame::OnHello(wxCommandEvent& event)
{
    event.Skip();   
    wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& event)
{
    event.Skip();
    wxMessageBox("Bro i dont know what i am doing");
}
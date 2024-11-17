module;

#include <wx/wx.h>
#include <wx/icon.h>

export module MainFrame;

import TodoList;

export class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAddButtonClicked(wxCommandEvent& event);
    void OnDeleteButtonClicked(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1,
    ID_ADD = 2,
    ID_DELETE = 3,
    ID_VIEWTODOLISTS = 4
};

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Todo List")
{    
    wxIcon appIcon("..\\resources\\appIcon.ico", wxBITMAP_TYPE_ICO);
    SetIcon(appIcon);
    
    SetSize(wxSize(300, 400));
    SetMinSize(wxSize(300, 400));
    SetMaxSize(wxSize(300, 400));
    
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_ADD, "Add new Todo List");
    menuFile->Append(ID_DELETE, "Delete a TodoList");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *vieTodoListMenu = new wxMenu;
    
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(vieTodoListMenu, "&To Do Lists");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
    
    wxPanel *panel = new wxPanel(this, ID_ADD);
    wxButton* AddButton = new wxButton(panel, ID_ADD, wxT("Add"),
                        wxPoint(10,5), wxSize(100, 35));
    wxButton* DeleteButton = new wxButton(panel, ID_DELETE, wxT("Delete"),
                        wxPoint(170,5), wxSize(100, 35));
    
    DeleteButton->Bind(wxEVT_BUTTON, &MainFrame::OnDeleteButtonClicked, this);
    AddButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);

    
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
    wxMessageBox("Written by Markus Kammerstetter","", wxOK | wxICON_INFORMATION);
}
 
void MainFrame::OnHello(wxCommandEvent& event)
{
    event.Skip();   
    wxLogMessage("Hello world from wxWidgets!");
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& event)
{
    event.Skip();
    wxMessageBox("A new Todo List was created");
    TodoList tdl = TodoList();
}

void MainFrame::OnDeleteButtonClicked(wxCommandEvent& event)
{
    event.Skip();
    wxMessageBox("Deleted a todolist");
}
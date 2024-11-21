module;

#include <vector>
#include <wx/wx.h>
#include <wx/icon.h>

export module MainFrame;

import std;
import TodoList;
import Task;

export class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    wxPanel *mainPanel;
    wxCheckBox *checkbox;
    wxCheckListBox *checkboxList;
    wxTextCtrl *nameOfTask;
    std::vector<Task> tasks;
    
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAddButtonClicked(wxCommandEvent& event);
    void OnDeleteButtonClicked(wxCommandEvent& event);
    void CreateTaskButton(wxCommandEvent& evt);
    void DeleteTaskButton(wxCommandEvent& evt);
};

class CreateTaskWindow : public wxFrame
{
private:
    wxTextCtrl* taskName;
    wxTextCtrl* taskDescription;
    wxPanel* mainPanel;
    
public:
    CreateTaskWindow();
    wxTextCtrl* GetTaskName();
    wxTextCtrl* GetTaskDescription();
};

enum
{
    ADD_TODOLIST_ID = 2,
    DELETE_TODOLIST_ID = 3
};

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Todo List")
{    
    wxIcon appIcon("..\\resources\\appIcon.ico", wxBITMAP_TYPE_ICO);
    SetIcon(appIcon);
    SetSize(wxSize(300, 400));
    MainFrame::SetMinSize(wxSize(300, 400));
    MainFrame::SetMaxSize(wxSize(300, 400));
    MainFrame::SetBackgroundColour(*wxWHITE);
    CenterOnScreen();
    
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ADD_TODOLIST_ID, "Add new Todo List");
    menuFile->Append(DELETE_TODOLIST_ID, "Delete a TodoList");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *vieTodoListMenu = new wxMenu;
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    
    menuBar->Append(menuFile, "&File");
    menuBar->Append(vieTodoListMenu, "&To Do Lists");
    menuBar->Append(menuHelp, "&Help");
    MainFrame::SetMenuBar( menuBar );
    
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::OnAddButtonClicked, this, ADD_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::OnDeleteButtonClicked, this, DELETE_TODOLIST_ID);
    
    mainPanel = new wxPanel(this, wxID_ANY);
    wxButton* AddButton = new wxButton(mainPanel, wxID_ANY, wxT("Add"),
                        wxPoint(10,10), wxSize(120, 35));
    wxButton* DeleteButton = new wxButton(mainPanel, wxID_ANY, wxT("Delete"),
                        wxPoint(150,10), wxSize(120, 35));
    checkboxList = new wxCheckListBox(mainPanel, wxID_ANY,
                        wxPoint(10,55), wxSize(260,270));

    tasks.insert(tasks.begin(), Task("Task 1", "No"));
    tasks.insert(tasks.begin(), Task("Task 2", "Nod"));
    tasks.insert(tasks.begin(), Task("Task 4", "No"));
    tasks.insert(tasks.begin(), Task("Task 3", "No"));
    
    for (int i = 0; i < tasks.size(); i++)
    {
        checkboxList->Insert(tasks.at(i).getName(), checkboxList->GetCount());
    }
   
    
    AddButton->Bind(wxEVT_BUTTON, &MainFrame::CreateTaskButton, this);
    DeleteButton->Bind(wxEVT_BUTTON, &MainFrame::DeleteTaskButton, this);
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

void MainFrame::CreateTaskButton(wxCommandEvent& evt)
{
    evt.Skip();
    CreateTaskWindow* AddFrame = new CreateTaskWindow();
    AddFrame->CenterOnParent();
    AddFrame->Show(true);
}

void MainFrame::DeleteTaskButton(wxCommandEvent& evt)
{
    evt.Skip();
}


CreateTaskWindow::CreateTaskWindow() : wxFrame(nullptr, wxID_ANY, "Add new Task")
{
    CreateTaskWindow::SetBackgroundColour(*wxWHITE);
    SetSize(wxSize(400, 300));
    CreateTaskWindow::SetMinSize(wxSize(400, 300));
    CreateTaskWindow::SetMaxSize(wxSize(400, 300));
    
    mainPanel = new wxPanel(this, wxID_ANY);
    
    taskName = new wxTextCtrl(mainPanel, wxID_ANY, "", wxPoint(20, 20), wxDefaultSize);
    taskDescription = new wxTextCtrl(mainPanel, wxID_ANY, "", wxPoint(20, 70), wxDefaultSize);
    
}

wxTextCtrl* CreateTaskWindow::GetTaskName()
{
    return taskName;
}
wxTextCtrl* CreateTaskWindow::GetTaskDescription()
{
    return taskDescription;
}
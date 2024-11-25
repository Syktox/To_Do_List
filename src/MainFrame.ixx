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
    wxCheckListBox *checkboxList;
    wxTextCtrl *nameOfTask;
    std::vector<Task> tasks;

    
    void AddControls();
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnAddMenuButtonClicked(wxCommandEvent& event);
    void OnDeleteMenuButtonClicked(wxCommandEvent& event);
    void CreateTaskButton(wxCommandEvent& evt);
    void DeleteTaskButton(wxCommandEvent& evt);
    void UpdateTaskList();
    void LoadJSONFile();
    void ClearTaskList(wxCommandEvent& evt);
};

class CreateTaskWindow : public wxFrame
{
private:
    wxTextCtrl* taskName;
    wxTextCtrl* taskDescription;
    wxPanel* mainPanel;
    wxStaticText* name;
    wxStaticText* description;
    wxButton* createButton;
    wxButton* quitButton;
    wxString taskNameString;
    wxString taskDescriptionString;
    
    void QuitButtonClicked(wxCommandEvent& evt);
    void CreateButtonClicked(wxCommandEvent& evt);

public:
    CreateTaskWindow();
    wxString GetTaskName();
    wxString GetTaskDescription();
};

enum
{
    ADD_TODOLIST_ID = 2,
    DELETE_TODOLIST_ID = 3,
    CLEAR_TODOLIST_ID = 4
};

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Todo List")
{    
    AddControls();
    LoadJSONFile();

    tasks.insert(tasks.begin(), Task("Task 1", "No"));
    tasks.insert(tasks.begin(), Task("Task 2", "Nod"));
    tasks.insert(tasks.begin(), Task("Task 3", "No"));
    tasks.insert(tasks.begin(), Task("Task 4", "No"));
    
    // todo json file einbinden
    
    UpdateTaskList();
}

void MainFrame::AddControls()
{
    wxIcon appIcon("..\\resources\\appIcon.ico", wxBITMAP_TYPE_ICO);
    SetIcon(appIcon);
    SetSize(wxSize(300, 400));
    MainFrame::SetMinSize(wxSize(300, 400));
    MainFrame::SetMaxSize(wxSize(300, 400));
    MainFrame::SetBackgroundColour(*wxWHITE);
    CenterOnScreen();
    
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ADD_TODOLIST_ID, "Add a new Todo List");
    menuFile->Append(DELETE_TODOLIST_ID, "Delete this Todo List");
    menuFile->Append(CLEAR_TODOLIST_ID, "Clear this Todo List");
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
    Bind(wxEVT_MENU, &MainFrame::OnAddMenuButtonClicked, this, ADD_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::OnDeleteMenuButtonClicked, this, DELETE_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::ClearTaskList, this, CLEAR_TODOLIST_ID);
    
    mainPanel = new wxPanel(this, wxID_ANY);
    wxButton* AddButton = new wxButton(mainPanel, wxID_ANY, wxT("Add"),
                        wxPoint(10,10), wxSize(120, 35));
    wxButton* DeleteButton = new wxButton(mainPanel, wxID_ANY, wxT("Delete"),
                        wxPoint(150,10), wxSize(120, 35));
    checkboxList = new wxCheckListBox(mainPanel, wxID_ANY,
                        wxPoint(10,55), wxSize(260,270));
    
    AddButton->Bind(wxEVT_BUTTON, &MainFrame::CreateTaskButton, this);
    DeleteButton->Bind(wxEVT_BUTTON, &MainFrame::DeleteTaskButton, this);
}

void MainFrame::OnExit(wxCommandEvent&)
{
    Close(true);
}
 
void MainFrame::OnAbout(wxCommandEvent&)
{
    wxMessageBox("Written by Markus Kammerstetter\n"
                 "This should be a small to-do list designed to help me improve my C++ skills.",
                 "Author Message", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnAddMenuButtonClicked(wxCommandEvent& event)
{
    event.Skip();
    wxMessageBox("A new Todo List was created");
    TodoList tdl = TodoList();
}

void MainFrame::OnDeleteMenuButtonClicked(wxCommandEvent& event)
{
    event.Skip();
    wxMessageBox("Deleted a todolist");
}

void MainFrame::CreateTaskButton(wxCommandEvent&)
{
    CreateTaskWindow* AddFrame = new CreateTaskWindow();
    AddFrame->CenterOnParent();
    AddFrame->Show(true);   // fehler darunter
    AddFrame->Bind(wxEVT_CLOSE_WINDOW, [this, AddFrame](wxCloseEvent&)
    {
        wxString name = AddFrame->GetTaskName();
        wxString description = AddFrame->GetTaskDescription();
        tasks.insert(tasks.begin(),
        Task(name.ToStdString(), description.ToStdString()));
        UpdateTaskList();
        AddFrame->Destroy();
    });
}

void MainFrame::DeleteTaskButton(wxCommandEvent&)
{
    wxArrayInt indices;
    checkboxList->GetCheckedItems(indices);
    std::ranges::reverse(indices.begin(), indices.end());
    for (const auto& i : indices)
    {
        if (i < tasks.size())
        {
            tasks.erase(tasks.begin() + i);
        } else
        {
            wxLogWarning("Can't delete element", i);
        }
    }
    UpdateTaskList();
}

void MainFrame::UpdateTaskList()
{
    checkboxList->Clear();
    for (auto& task : tasks)
    {
        checkboxList->Insert(task.getName(), checkboxList->GetCount());
    }
}

void MainFrame::LoadJSONFile()
{
    
}

void MainFrame::ClearTaskList(wxCommandEvent&)
{
    tasks.clear();
    UpdateTaskList();
}

CreateTaskWindow::CreateTaskWindow() : wxFrame(nullptr, wxID_ANY, "Add new Task")
{
    wxIcon appIcon("..\\resources\\appIcon.ico", wxBITMAP_TYPE_ICO);
    SetIcon(appIcon);
    CreateTaskWindow::SetBackgroundColour(*wxWHITE);
    SetSize(wxSize(400, 400));
    CreateTaskWindow::SetMinSize(wxSize(400, 400));
    CreateTaskWindow::SetMaxSize(wxSize(400, 400));
    
    mainPanel = new wxPanel(this, wxID_ANY);
    name = new wxStaticText(mainPanel, wxID_ANY, wxT("Task Name"),
                                    wxPoint(20, 20), wxDefaultSize, 0);
    description = new wxStaticText(mainPanel, wxID_ANY, wxT("Task Description"),
                                    wxPoint(20, 120), wxDefaultSize, 0);
    taskName = new wxTextCtrl(mainPanel, wxID_ANY, "",
                                    wxPoint(20, 50), wxSize(345, 25));
    taskDescription = new wxTextCtrl(mainPanel, wxID_ANY, "",
                                    wxPoint(20, 150), wxSize(345, 90));
    createButton = new wxButton(mainPanel, wxID_ANY, wxT("Create Task"),
                                    wxPoint(225, 275), wxSize(100, 50));
    quitButton = new wxButton(mainPanel, wxID_ANY, wxT("Quit"),
                                    wxPoint(50, 275), wxSize(100, 50));
    quitButton->Bind(wxEVT_BUTTON, &CreateTaskWindow::QuitButtonClicked, this);
    createButton->Bind(wxEVT_BUTTON, &CreateTaskWindow::CreateButtonClicked, this);
}

void CreateTaskWindow::QuitButtonClicked(wxCommandEvent&)
{
    Close(true);
}

void CreateTaskWindow::CreateButtonClicked(wxCommandEvent&)
{
    taskNameString = taskName->GetValue();
    taskDescriptionString = taskDescription->GetValue();
    if (!taskNameString.empty())
    {
        Close(true);
        return;
    }
    wxMessageBox("No task name entered");
}

wxString CreateTaskWindow::GetTaskName()
{
    return taskNameString;
}

wxString CreateTaskWindow::GetTaskDescription()
{
    return taskDescriptionString;
}
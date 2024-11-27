module;

#include <iostream>
#include <ostream>
#include <vector>
#include <wx/wx.h>
#include <wx/icon.h>
#include <nlohmann/json.hpp>

export module MainFrame;

import std;
import TodoList;
import Task;

export class MainFrame : public wxFrame
{
public:
    MainFrame();
    std::vector<Task> GetTasks();

private:
    wxPanel *mainPanel;
    std::vector<Task> tasks;
    wxCheckListBox* checkboxList;
    wxButton* deleteButton;
    wxButton* addButton;
    wxButton* finishButton;
    
    
    void AddControls();
    void BindEventHandlers();
    void LoadJSONFile();
    void UpdateTaskList();

    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);

    void MenuAddTodoList(wxCommandEvent& evt);
    void MenuDeleteTodoList(wxCommandEvent& evt);
    void MenuClearTaskList(wxCommandEvent& evt);
    
    void CreateTaskButton(wxCommandEvent& evt);
    void DeleteTaskButton(wxCommandEvent& evt);
    void FinishTaskButton(wxCommandEvent& evt);
    
    void OnListKeyDown(wxKeyEvent& evt);
    void OnArrowUPKey();
    void OnDeleteKey();
    void OnArrowDOWNKey();
    void OnEnterKey();
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
    wxCommandEvent dummyCommandEvent;
    
    void QuitButtonClicked(wxCommandEvent& evt);
    void CreateButtonClicked(wxCommandEvent& evt);
    void OnListKeyDown(wxKeyEvent& evt);
    void OnEnterKey();
    void OnEscapeKey();

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
    BindEventHandlers();
    LoadJSONFile();

    tasks.insert(tasks.begin(), Task("Task 1", "No"));
    tasks.insert(tasks.begin(), Task("Task 2", "Nod"));
    tasks.insert(tasks.begin(), Task("Task 3", "No"));
    tasks.insert(tasks.begin(), Task("Task 4", "No"));
    
    // todo json file einbinden
    
    UpdateTaskList();
}

std::vector<Task> MainFrame::GetTasks()
{
    return tasks;
}

void MainFrame::AddControls()
{
    wxIcon appIcon("..\\resources\\appIcon.ico", wxBITMAP_TYPE_ICO);
    SetIcon(appIcon);
    SetSize(wxSize(300, 450));
    MainFrame::SetMinSize(wxSize(300, 450));
    MainFrame::SetMaxSize(wxSize(300, 450));
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
    
    mainPanel = new wxPanel(this, wxID_ANY);
    addButton = new wxButton(mainPanel, wxID_ANY, wxT("Add"),
                        wxPoint(10,10), wxSize(120, 35));
    deleteButton = new wxButton(mainPanel, wxID_ANY, wxT("Delete"),
                        wxPoint(150,10), wxSize(120, 35));
    finishButton = new wxButton(mainPanel, wxID_ANY, wxT("Finish"),
                            wxPoint(10, 340), wxSize(260, 35));
    checkboxList = new wxCheckListBox(mainPanel, wxID_ANY,
                        wxPoint(10,55), wxSize(260,270), 0, nullptr, wxWANTS_CHARS);
    
}

void MainFrame::BindEventHandlers()
{
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::MenuAddTodoList, this, ADD_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::MenuDeleteTodoList, this, DELETE_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::MenuClearTaskList, this, CLEAR_TODOLIST_ID);
    
    addButton->Bind(wxEVT_BUTTON, &MainFrame::CreateTaskButton, this);
    deleteButton->Bind(wxEVT_BUTTON, &MainFrame::DeleteTaskButton, this);
    finishButton->Bind(wxEVT_BUTTON, &MainFrame::FinishTaskButton, this);
    checkboxList->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
}

void MainFrame::LoadJSONFile()
{
    
}

void MainFrame::UpdateTaskList()
{
    int correction = 0;
    if (tasks.size() > checkboxList->GetCount()) {
        correction = 1;
    }
    std::vector<std::string> taskNames;
    wxArrayInt index;
    int checkedItemSize = checkboxList->GetCheckedItems(index);

    std::cout << checkboxList->GetCount() << std::endl;
    
    for (int i = 0; i < checkedItemSize && tasks.size() >= checkboxList->GetCount(); i++) {
        taskNames.emplace_back(tasks.at(index[i] + correction).getName());
    }
    checkboxList->Clear();
    for (auto& task : tasks)
    {
        checkboxList->Insert(task.getName(), checkboxList->GetCount());
        if (std::ranges::find(taskNames.begin(), taskNames.end(), task.getName()) != taskNames.end() && tasks.size() >= checkboxList->GetCount())
        {
            checkboxList->Check(checkboxList->GetCount() - 1, true);
        }
    }
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

void MainFrame::MenuAddTodoList(wxCommandEvent&)
{
    wxMessageBox("A new Todo List was created");
    TodoList tdl = TodoList();
}

void MainFrame::MenuDeleteTodoList(wxCommandEvent&)
{
    wxMessageBox("Deleted a todolist");
}

void MainFrame::MenuClearTaskList(wxCommandEvent&)
{
    tasks.clear();
    UpdateTaskList();
}

void MainFrame::CreateTaskButton(wxCommandEvent&)
{
    CreateTaskWindow* AddFrame = new CreateTaskWindow();
    AddFrame->CenterOnParent();
    AddFrame->Show(true);
    AddFrame->Bind(wxEVT_CLOSE_WINDOW, [this, AddFrame](wxCloseEvent&)
    {
        wxString name = AddFrame->GetTaskName();
        wxString description = AddFrame->GetTaskDescription();
        if (name.length() <= 0)
        {
            AddFrame->Destroy();
            return;
        }

        if (std::ranges::any_of(tasks, [&name, &description](auto& task){ return task.getName() == name; })) {
            wxMessageBox("Task already exists");
            AddFrame->Destroy();
            return;
        }
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

void MainFrame::FinishTaskButton(wxCommandEvent&)
{
    wxMessageBox("Finished Task");
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
    wxArrayInt indices;
    if (checkboxList->GetCheckedItems(indices) > 0)
    {
        switch (evt.GetKeyCode()) {
        case WXK_BACK:
            OnDeleteKey();
            break;
        case WXK_UP:
            OnArrowUPKey();
            break;
        case WXK_DOWN:
            OnArrowDOWNKey();
            break;
        case WXK_RETURN:
            OnEnterKey();
            break;
        default:
            std::cout << evt.GetKeyCode() << std::endl;
            break;
        }   
    }
}

void MainFrame::OnDeleteKey()
{
    wxCommandEvent dummyEvent(wxEVT_BUTTON, wxID_ANY);
    DeleteTaskButton(dummyEvent);
}

void MainFrame::OnArrowUPKey()
{
    wxMessageBox("UP Key pressed");
    int index = checkboxList->GetSelection();
    if (wxNOT_FOUND == index)
    {
        wxLogWarning("No element selected to move");
        return;
    }
    
    UpdateTaskList();
}

void MainFrame::OnArrowDOWNKey()
{
    wxMessageBox("DOWN Key pressed");
    UpdateTaskList();
}

void MainFrame::OnEnterKey()
{
    wxMessageBox("Enter Key pressed");
    UpdateTaskList();
}

//
// CREATETASK WINDOW
//


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
                                    wxPoint(20, 50), wxSize(345, 25), wxTE_PROCESS_ENTER);
    taskDescription = new wxTextCtrl(mainPanel, wxID_ANY, "",
                                    wxPoint(20, 150), wxSize(345, 90), wxTE_PROCESS_ENTER);
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
    wxMessageBox("No task name entered or task already exists");
}

wxString CreateTaskWindow::GetTaskName()
{
    return taskNameString;
}

wxString CreateTaskWindow::GetTaskDescription()
{
    return taskDescriptionString;
}

void CreateTaskWindow::OnListKeyDown(wxKeyEvent& evt)
{
    switch (evt.GetKeyCode())
    {
    case wxKEY_NONE:
        break;
    default:
        break;
    }    
}

void CreateTaskWindow::OnEnterKey()
{
    CreateButtonClicked(dummyCommandEvent);
}

void CreateTaskWindow::OnEscapeKey()
{
    QuitButtonClicked(dummyCommandEvent);
}



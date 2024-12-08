module;

#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <Windows.h>
#include <fstream>

export module MainFrame;

#define IDI_APP_ICON 101

import std;
import TodoList;
import Task;
import CreateTaskWindow;

export class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    wxPanel* mainPanel;
    std::vector<Task> tasks;
    wxCheckListBox* checkboxList;
    wxButton* deleteButton;
    wxButton* addButton;
    wxButton* finishButton;
    wxCommandEvent dummyEvent;
    
    void AddControls();
    void BindEventHandlers();
    std::filesystem::path GetDataPath();
    bool JSONFilesExists();
    nlohmann::json LoadJSONFile();
    void CreateJSONFile();
    void WriteTaskToJSON(Task task);
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
    void OnCKey();
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
    if (JSONFilesExists())
    {
        nlohmann::json jsonArray = LoadJSONFile();
        if (jsonArray.contains("tasks") && jsonArray["tasks"].is_array())
        {
            for (auto json : jsonArray["tasks"]) {
                Task task = Task(json["name"], json["description"]);
                task.setCreated(json["created"]);
                task.setCompleted(json["completed"]);
                task.setCompletedAt(json["completedAt"]);
                tasks.insert(tasks.begin(), task);
            }
        }
    } else
    {
        CreateJSONFile();
    }
    
    UpdateTaskList();
}

void MainFrame::AddControls()
{
    wxIcon appIcon;
    appIcon.LoadFile("IDI_APP_ICON", wxBITMAP_TYPE_ICO_RESOURCE);
    if (appIcon.IsOk()) { SetIcon(appIcon); }

    SetSize(wxSize(300, 450));
    MainFrame::SetMinSize(wxSize(300, 450));
    MainFrame::SetMaxSize(wxSize(300, 450));
    MainFrame::SetBackgroundColour(*wxWHITE);
    CenterOnScreen();

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ADD_TODOLIST_ID, "Add a new Todo List");
    menuFile->Append(DELETE_TODOLIST_ID, "Delete this Todo List");
    menuFile->Append(CLEAR_TODOLIST_ID, "Clear this Todo List");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu* vieTodoListMenu = new wxMenu;
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar* menuBar = new wxMenuBar;

    menuBar->Append(menuFile, "&File");
    menuBar->Append(vieTodoListMenu, "&To Do Lists");
    menuBar->Append(menuHelp, "&Help");
    MainFrame::SetMenuBar(menuBar);

    mainPanel = new wxPanel(this, wxID_ANY);
    addButton = new wxButton(mainPanel, wxID_ANY, wxT("Add"),
                             wxPoint(10, 10), wxSize(120, 35));
    deleteButton = new wxButton(mainPanel, wxID_ANY, wxT("Delete"),
                                wxPoint(150, 10), wxSize(120, 35));
    finishButton = new wxButton(mainPanel, wxID_ANY, wxT("Finish"),
                                wxPoint(10, 340), wxSize(260, 35));
    checkboxList = new wxCheckListBox(mainPanel, wxID_ANY,
                                      wxPoint(10, 55), wxSize(260, 270), 0, nullptr, wxWANTS_CHARS);
}

void MainFrame::BindEventHandlers()
{
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MainFrame::MenuAddTodoList, this, ADD_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::MenuDeleteTodoList, this, DELETE_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::MenuClearTaskList, this, CLEAR_TODOLIST_ID);
    Bind(wxEVT_CHAR_HOOK, &MainFrame::OnListKeyDown, this, wxID_ANY);
 
    addButton->Bind(wxEVT_BUTTON, &MainFrame::CreateTaskButton, this);
    deleteButton->Bind(wxEVT_BUTTON, &MainFrame::DeleteTaskButton, this);
    finishButton->Bind(wxEVT_BUTTON, &MainFrame::FinishTaskButton, this);
    checkboxList->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
}

std::filesystem::path MainFrame::GetDataPath()
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    static auto exe_parent_path = std::filesystem::path(exePath).parent_path();
    return exe_parent_path / "data";
}

bool MainFrame::JSONFilesExists()
{
    std::filesystem::path dataPath = GetDataPath();
    if (std::filesystem::exists(dataPath) &&
        std::filesystem::exists(dataPath / "tasks.json") &&
        std::filesystem::exists(dataPath / "finished.json"))
    {
        return true;
    }
    return false;
}

nlohmann::json MainFrame::LoadJSONFile() // no work
{
    std::filesystem::path dataPath = GetDataPath();
    std::ifstream inputFile{dataPath / "tasks.json"};
    std::ifstream jsonFinishedDataFile{dataPath / "finished.json"};

    if (inputFile.peek() == std::ifstream::traits_type::eof())
    {
        return nlohmann::json();
    }
    
    return nlohmann::json::parse(inputFile);
}

void MainFrame::CreateJSONFile()
{
    std::filesystem::path dataFolder = GetDataPath();
    try
    {
        if (!exists(dataFolder)) { create_directory(dataFolder); }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::ofstream jsonDataFile(dataFolder / "tasks.json");
    std::ofstream jsonFinishedDataFile(dataFolder / "finished.json");
}

void MainFrame::WriteTaskToJSON(Task task)
{
    std::filesystem::path outputFile = GetDataPath() / "tasks.json";
    std::cout << "Wrote task to JSON File: " << task.getName() << std::endl;
    
    nlohmann::json jsonData = LoadJSONFile();
    std::ofstream output(outputFile);    
    nlohmann::json jsonObj = { {"id", tasks.size() - 1}, {"name", task.getName()},
                        {"description", task.getDescription()}, {"created", task.getCreated()},
                        {"completed", false}, {"completedAt", task.getCompletedAt()}};
    jsonData["tasks"].push_back(jsonObj);
    
    if (output.is_open())
    {
        output << jsonData.dump(4);
        output.close();
    } else
    {
        std::cerr << "Error: " << outputFile.string() << std::endl;
    }
}

void MainFrame::UpdateTaskList()
{
    int correction = 0;
    if (tasks.size() > checkboxList->GetCount())
    {
        correction = 1;
    }
    std::vector<std::string> taskNames;
    wxArrayInt index;
    int checkedItemSize = checkboxList->GetCheckedItems(index);
    for (int i = 0; i < checkedItemSize && tasks.size() >= checkboxList->GetCount(); i++)
    {
        taskNames.emplace_back(tasks.at(index[i] + correction).getName());
    }
    checkboxList->Clear();
    for (auto& task : tasks)
    {
        checkboxList->Insert(task.getName(), checkboxList->GetCount());
        if (std::ranges::find(taskNames.begin(), taskNames.end(), task.getName()) != taskNames.end() && tasks.size() >=
            checkboxList->GetCount())
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
    nlohmann::json json = LoadJSONFile();
    json["tasks"].clear();
    std::filesystem::path outputFile = GetDataPath() / "tasks.json";
    std::ofstream output(outputFile);
    output << json.dump(4);
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

        if (std::ranges::any_of(tasks, [&name, &description](auto& task) { return task.getName() == name; }))
        {
            wxMessageBox("Task already exists");
            AddFrame->Destroy();
            return;
        }
        Task task = Task(name.ToStdString(), description.ToStdString());
        tasks.insert(tasks.begin(), task);
        WriteTaskToJSON(task);
        UpdateTaskList();
        AddFrame->Destroy();
    });
}

void MainFrame::DeleteTaskButton(wxCommandEvent&)   // needs to be updated
{
    wxArrayInt indices;
    checkboxList->GetCheckedItems(indices);
    std::ranges::reverse(indices.begin(), indices.end());
    for (const auto& i : indices)
    {
        if (i < tasks.size())
        {
            tasks.erase(tasks.begin() + i);
        }
        else
        {
            wxLogWarning("Can't delete element", i);
        }
    }
    UpdateTaskList();
}

void MainFrame::FinishTaskButton(wxCommandEvent&)   // needs to be updated
{
    wxArrayInt indices;
    checkboxList->GetCheckedItems(indices);

    for (const auto& i : indices)
    {
        if (i  < tasks.size())
        {
            
        }
    }
    
    wxMessageBox("Finished Task");
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
    wxArrayInt indices;
    if (checkboxList->GetCheckedItems(indices) > 0)
    {
        switch (evt.GetKeyCode())
        {
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
            evt.Skip(); // dont know the effects
            break;
        }
    }
    switch (evt.GetKeyCode())
    {
    case 67:
        OnCKey();
        break;
    default:
        break;
    }
}

void MainFrame::OnDeleteKey()
{
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
    FinishTaskButton(dummyEvent);
    UpdateTaskList();
}

void MainFrame::OnCKey()
{
    CreateTaskButton(dummyEvent);
}

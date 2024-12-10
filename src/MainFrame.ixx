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
    bool NeededFilesExist();
    nlohmann::json LoadJSONFile();
    void CreateJSONFile();
    void WriteTaskToJSON(Task task);
    void UpdateTaskList();
    void WriteJSONToFile(nlohmann::json json);
    void WriteToLogFile(std::string log);

    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    
    void MenuClearTaskList(wxCommandEvent& evt);
    void ShowLog(wxCommandEvent& evt);

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
    CLEAR_TODOLIST_ID = 2,
    SHOW_LOG_ID = 3
};

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Todo List")
{
    AddControls();
    BindEventHandlers();
    if (NeededFilesExist())
    {
        UpdateTaskList();
    } else
    {
        CreateJSONFile();
    }
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
    menuFile->Append(CLEAR_TODOLIST_ID, "Clear Todo List");
    menuFile->Append(SHOW_LOG_ID, "Show Log");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar* menuBar = new wxMenuBar;

    menuBar->Append(menuFile, "&File");
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
    Bind(wxEVT_MENU, &MainFrame::MenuClearTaskList, this, CLEAR_TODOLIST_ID);
    Bind(wxEVT_MENU, &MainFrame::ShowLog, this, SHOW_LOG_ID);
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

bool MainFrame::NeededFilesExist()
{
    std::filesystem::path dataPath = GetDataPath();
    if (std::filesystem::exists(dataPath) &&
        std::filesystem::exists(dataPath / "tasks.json") &&
        std::filesystem::exists(dataPath / "finished.json") &&
        std::filesystem::exists(dataPath / "log.txt"))
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
    std::ofstream jsonLogFile(dataFolder / "log.txt");
}

void MainFrame::WriteTaskToJSON(Task task)
{
    std::filesystem::path outputFile = GetDataPath() / "tasks.json";
    nlohmann::json jsonData = LoadJSONFile();
    std::ofstream output(outputFile);
    
    nlohmann::json jsonObj = { {"id", tasks.size() - 1}, {"name", task.getName()},
                        {"description", task.getDescription()}, {"created", task.getCreated()},
                        {"completed", false}, {"completedAt", task.getCompletedAt()}};
    jsonData["tasks"].push_back(jsonObj);
    
    WriteToLogFile("Wrote task to JSON File: " + task.getName());
    WriteJSONToFile(jsonData);
}

void MainFrame::UpdateTaskList()
{
    nlohmann::json jsonData = LoadJSONFile();
    if (jsonData.contains("tasks") && jsonData["tasks"].is_array())
    {
        checkboxList->Clear();
        tasks.clear();
        for (auto json : jsonData["tasks"]) {
            Task task = Task(json["name"], json["description"]);
            task.setCreated(json["created"]);
            task.setCompleted(json["completed"]);
            task.setCompletedAt(json["completedAt"]);
            tasks.insert(tasks.end(), task);
            checkboxList->Insert(task.getName(), checkboxList->GetCount());
        }
    }
}

void MainFrame::WriteJSONToFile(nlohmann::json json)
{
    std::filesystem::path outputFile = GetDataPath() / "tasks.json";
    std::ofstream output(outputFile);
    output << json.dump(4);
    output.close();
}

void MainFrame::WriteToLogFile(std::string log) 
{
    std::filesystem::path outputFile = GetDataPath() / "log.txt";
    std::ofstream output(outputFile, std::ios::app);
    output << log + "\n";
    output.close();
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

void MainFrame::MenuClearTaskList(wxCommandEvent&)
{
    tasks.clear();
    nlohmann::json json = LoadJSONFile();
    json["tasks"].clear();
    WriteJSONToFile(json);
    WriteToLogFile("Cleared Task List");
    UpdateTaskList();
}

void MainFrame::ShowLog(wxCommandEvent&)
{
    std::filesystem::path logFile = GetDataPath() / "log.txt";
    std::ifstream input(logFile);
    if (input.is_open())
    {
        while (!input.eof())
        {
            std::string line;
            std::getline(input, line);
            std::cout << line << std::endl;
        }
        input.close();
    }
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
            WriteToLogFile("Task already exists");
            AddFrame->Destroy();
            return;
        }
        Task task = Task(name.ToStdString(), description.ToStdString());
        tasks.insert(tasks.begin(), task);
        WriteToLogFile("Task created: " + task.getName());
        WriteTaskToJSON(task);
        UpdateTaskList();
        AddFrame->Destroy();
    });
}

void MainFrame::DeleteTaskButton(wxCommandEvent&)
{
    wxArrayInt indices;
    checkboxList->GetCheckedItems(indices);
    std::ranges::reverse(indices.begin(), indices.end());
    nlohmann::json json = LoadJSONFile();
    
    for (const auto& i : indices)
    {
        if (i < json["tasks"].size())
        {
            nlohmann::json j = json["tasks"][i];
            WriteToLogFile("Tasks deleted: " + j["name"].get<std::string>());
            json["tasks"].erase(i);
        }
        else
        {
            wxLogWarning("Can't delete element", i);
            WriteToLogFile("Can't delete element" + i);
        }
    }
    WriteJSONToFile(json);
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
            wxMessageBox("Finished task " + tasks[i].getName());
            WriteToLogFile("Finished task " + tasks[i].getName());
        }
    }
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
            evt.Skip();
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

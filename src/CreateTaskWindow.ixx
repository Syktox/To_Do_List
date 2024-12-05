module;

#include <wx/wx.h>

export module CreateTaskWindow;

#define IDI_APP_ICON 101

export class CreateTaskWindow : public wxFrame
{
public:
    CreateTaskWindow();
    wxString GetTaskName();
    wxString GetTaskDescription();
    
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
};



CreateTaskWindow::CreateTaskWindow() : wxFrame(nullptr, wxID_ANY, "Add new Task")
{
    wxIcon appIcon;
    appIcon.LoadFile("IDI_APP_ICON", wxBITMAP_TYPE_ICO_RESOURCE);
    if (appIcon.IsOk()) { SetIcon(appIcon); }
    
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
    taskName->Bind(wxEVT_KEY_DOWN, &CreateTaskWindow::OnListKeyDown, this);
    taskDescription->Bind(wxEVT_KEY_DOWN, &CreateTaskWindow::OnListKeyDown, this);
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
    case WXK_RETURN:
        OnEnterKey();
        break;
    case WXK_ESCAPE:
        OnEscapeKey();
    default:
        evt.Skip();
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


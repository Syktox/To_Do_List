module;

#include <wx/wx.h>

export module CreateLogWindow;

#define IDI_APP_ICON 101

import std;

export class CreateLogWindow : public wxFrame
{
    wxPanel *mainPanel;
    wxTextCtrl *logText;

    std::filesystem::path GetDataPath();
    
public:
    CreateLogWindow();
};

CreateLogWindow::CreateLogWindow() : wxFrame(nullptr, wxID_ANY, "Log File")
{
    wxIcon appIcon;
    appIcon.LoadFile("IDI_APP_ICON", wxBITMAP_TYPE_ICO_RESOURCE);
    if (appIcon.IsOk()) { SetIcon(appIcon); }

    {
        mainPanel = new wxPanel(this, wxID_ANY);
        
        logText = new wxTextCtrl(mainPanel, wxID_ANY, "", 
            wxPoint(0, 0), wxSize(460, 340), wxTE_MULTILINE | wxTE_READONLY);
        
        std::ifstream file(GetDataPath() / "log.txt");
        if (file.is_open())
        {
            std::ostringstream buffer;
            buffer << file.rdbuf();
            file.close();
            logText->SetValue(buffer.str());
        }
        else
        {
            logText->SetValue("Error: Unable to open file.");
        }
    }
}

std::filesystem::path CreateLogWindow::GetDataPath()
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    static auto exe_parent_path = std::filesystem::path(exePath).parent_path();
    return exe_parent_path / "data";
}
//
// Created by giacomo on 18/07/24.
//

#include "TaskPanel.h"

#include "ThemeManager.h"

TaskPanel::TaskPanel(wxWindow *parent, const wxString &title, const wxString &description, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size, wxBORDER_SUNKEN)
{
    this->taskTitle = title;
    this->taskDescription = description;

    this->checkBox = new wxCheckBox(this, TASK_CHECKMARK, "");
    this->checkBox->Bind(wxEVT_CHECKBOX, &TaskPanel::OnTaskEvent, this);

    wxString buttonLabel = title;
    if (buttonLabel.length() > 29) {
        buttonLabel.Truncate(29);
        buttonLabel.Append("...");
    }

    auto * taskButton = new wxButton(this, TASK_BUTTON, buttonLabel, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    taskButton->Bind(wxEVT_BUTTON, &TaskPanel::OnTaskEvent, this);
    
    auto * deleteButton = new wxButton(this, DELETE_BUTTON, wxString::FromUTF8("X"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE);
    deleteButton->SetBackgroundColour(ThemeManager::GetInstance().GetCurrentTheme().buttonDelete);
    deleteButton->SetForegroundColour(wxColour(255,255,255));
    deleteButton->Bind(wxEVT_BUTTON, &TaskPanel::OnTaskEvent, this);
    deleteButton->Fit();

    auto * sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(checkBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizer->Add(taskButton, 1, wxEXPAND | wxALL, 5);
    sizer->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    this->SetSizer(sizer);
}

bool TaskPanel::isChecked() {
    return this->checkBox->IsChecked();
}

void TaskPanel::OnTaskEvent(wxCommandEvent &event) {
    event.SetEventObject(this);
    wxPostEvent(GetParent(), event);
}
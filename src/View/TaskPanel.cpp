#include "TaskPanel.h"
#include "ThemeManager.h"

TaskPanel::TaskPanel(wxWindow *parent, const wxString &title, const wxString &description, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size, wxBORDER_SUNKEN)
{
    this->taskTitle = title;
    this->taskDescription = description;

    this->checkBox = new wxCheckBox(this, wxID_ANY, "");
    this->checkBox->Bind(wxEVT_CHECKBOX, &TaskPanel::OnTaskEvent, this);

    wxString buttonLabel = title;
    if (buttonLabel.length() > 29) {
        buttonLabel.Truncate(29);
        buttonLabel.Append("...");
    }

    auto * taskButton = new wxButton(this, TASK_BUTTON, buttonLabel, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    taskButton->Bind(wxEVT_BUTTON, &TaskPanel::OnTaskEvent, this);
    taskButton->SetBackgroundColour(ThemeManager::GetInstance().GetCurrentTheme().buttonBackground);
    
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

void TaskPanel::checkTask() {
    this->checkBox->SetValue(true);
}

wxString TaskPanel::getTaskTitle() const {
    return taskTitle;
}

wxString TaskPanel::getTaskDescription() const {
    return taskDescription;
}

void TaskPanel::setTaskTitle(const wxString &task_title) {
    auto sizer = this->GetSizer();
    wxSizerItem* item = sizer->GetItem(1);
    auto taskBtn = dynamic_cast<wxButton *>(item->GetWindow());
    if (taskBtn) {
        auto btnLabel = task_title;
        if (btnLabel.length() > 29) {
            btnLabel.Truncate(29);
            btnLabel.Append("...");
        }
        taskBtn->SetLabel(btnLabel);

    }
    this->taskTitle = task_title;
}

void TaskPanel::setTaskDescription(const wxString &task_description) {
    taskDescription = task_description;
}

void TaskPanel::setTaskColour(const wxColour &bgCl, const wxColour &fgCl) const {
    auto sizer = this->GetSizer();
    wxSizerItem* item = sizer->GetItem(1);
    if (item) {
        auto taskBtn = dynamic_cast<wxButton *>(item->GetWindow());
        if (taskBtn) {
            taskBtn->SetBackgroundColour(bgCl);
            taskBtn->SetForegroundColour(fgCl);
        }
    }
}
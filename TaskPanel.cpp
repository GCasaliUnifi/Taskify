//
// Created by giacomo on 18/07/24.
//

#include "TaskPanel.h"

TaskPanel::TaskPanel(wxWindow *parent, const wxString &title, const wxString &description, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size, wxBORDER_SUNKEN)
{
    this->taskTitle = title;
    this->taskDescription = description;

    this->checkBox = new wxCheckBox(this, wxID_ANY, "");
    this->checkBox->Bind(wxEVT_CHECKBOX, &TaskPanel::OnTaskEvent, this);

    auto * taskButton = new wxButton(this, TASK_BUTTON, title, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    taskButton->Bind(wxEVT_BUTTON, &TaskPanel::OnTaskEvent, this);

    auto * deleteButton = new wxButton(this, DELETE_BUTTON, wxString::FromUTF8("🗑"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE);
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
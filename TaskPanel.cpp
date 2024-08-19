//
// Created by giacomo on 18/07/24.
//

#include "TaskPanel.h"

TaskPanel::TaskPanel(wxWindow *parent, const wxString &title, const wxString &description, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size, wxBORDER_SUNKEN)
{
    this->checkBox = new wxCheckBox(this, wxID_ANY, "");
    this->checkBox->Bind(wxEVT_CHECKBOX, &TaskPanel::OnTaskCheck, this);
    auto * taskButton = new wxButton(this, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    auto * deleteButton = new wxButton(this, wxID_ANY, wxString::FromUTF8("🗑"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE);
    deleteButton->Fit();
    this->taskDescription = description;

    auto * sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(checkBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizer->Add(taskButton, 1, wxEXPAND | wxALL, 5);
    sizer->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    this->SetSizer(sizer);
}

bool TaskPanel::isChecked() {
    return this->checkBox->IsChecked();
}

void TaskPanel::OnTaskCheck(wxCommandEvent &event) {
    event.SetEventObject(this);
    wxPostEvent(GetParent(), event);
}

//
// Created by giacomo on 18/07/24.
//

#include "TaskPanel.h"

TaskPanel::TaskPanel(wxWindow *parent, const wxString &title, const wxString &description, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size)
{
    this->checkBox = new wxCheckBox(this, wxID_ANY, "");
    auto * taskButton = new wxButton(this, wxID_ANY, title);
    auto * deleteButton = new wxButton(this, wxID_ANY, wxT("D"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    deleteButton->Fit();
    this->taskDescription = description;

    this->SetBackgroundColour(wxColour(200, 100, 100));

    auto * sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(checkBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    sizer->Add(taskButton, 1, wxEXPAND | wxALL, 5);
    sizer->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    this->SetSizer(sizer);
}

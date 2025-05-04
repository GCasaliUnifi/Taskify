#include "TaskPanel.h"
#include "ThemeManager.h"

TaskPanel::TaskPanel(wxWindow *parent, const int taskIndex, bool isComplete, const wxString title)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN), taskIndex(taskIndex)
{
    auto * taskSizer = new wxBoxSizer(wxHORIZONTAL);

    checkBox = new wxCheckBox(this, wxID_ANY, "");
    checkBox->SetValue(isComplete);
    label = new wxStaticText(this, wxID_ANY, title);
    deleteButton = new wxButton(this, DELETE_BUTTON, wxString::FromUTF8("X"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT | wxBORDER_NONE);

    taskSizer->Add(checkBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    taskSizer->Add(label, 1, wxEXPAND | wxALL, 5);

    taskSizer->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    this->SetSizer(taskSizer);

    label->Bind(wxEVT_LEFT_UP, &TaskPanel::OnPanelClick, this);
    checkBox->Bind(wxEVT_CHECKBOX, &TaskPanel::OnCheckBox, this);
    deleteButton->Bind(wxEVT_BUTTON, &TaskPanel::OnCloseClicked, this);
}


int TaskPanel::getTaskIndex() const {
    return this->taskIndex;
}


bool TaskPanel::isChecked() {
    return checkBox->GetValue();
}


void TaskPanel::OnPanelClick(wxMouseEvent &event) {
    event.SetEventObject(this);
    wxPostEvent(GetGrandParent(), event);
}

void TaskPanel::OnCheckBox(wxCommandEvent &event) {
    // internalTask->SetCompleted(checkBox->GetValue());
    event.SetEventObject(this);
    wxPostEvent(GetGrandParent(), event);
}

void TaskPanel::OnCloseClicked(wxCommandEvent &event) {
    event.SetEventObject(this);
    wxPostEvent(GetGrandParent(), event);
}

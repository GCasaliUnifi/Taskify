//
// Created by giacomo on 18/07/24.
//

#ifndef TASKPANEL_H
#define TASKPANEL_H

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include "TaskPanel.h"

enum BUTTON_IDs {
    TASK_BUTTON = wxID_HIGHEST + 1,
    DELETE_BUTTON,
    ADD_TASK,
    MODIFY_TASK
};

class TaskPanel : public wxPanel {
public:
    TaskPanel(wxWindow *parent, const wxString &title, const wxString &description, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize);

    bool isChecked();

    void checkTask() {
        this->checkBox->SetValue(true);
    }

    wxString getTaskTitle() const {
        return taskTitle;
    }

    wxString getTaskDescription() const {
        return taskDescription;
    }

private:
    void OnTaskEvent(wxCommandEvent& event);

    wxString taskTitle;
    wxString taskDescription;
    wxCheckBox * checkBox;
};


#endif //TASKPANEL_H
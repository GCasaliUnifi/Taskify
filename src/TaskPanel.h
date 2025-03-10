#ifndef TASKPANEL_H
#define TASKPANEL_H

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include "TaskPanel.h"

enum BUTTON_IDs {
    TASK_BUTTON = wxID_HIGHEST + 1,
    TASK_CHECKMARK,
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

    void setTaskTitle(const wxString &task_title) {
        auto sizer = this->GetSizer();
        wxSizerItem* item = sizer->GetItem(1);
        auto taskBtn = dynamic_cast<wxButton *>(item->GetWindow());
        if (taskBtn) {
            taskBtn->SetLabel(task_title);
        }
        taskTitle = task_title;
    }

    void setTaskDescription(const wxString &task_description) {
        taskDescription = task_description;
    }

    void setTaskColour(const wxColour &bgCl, const wxColour &fgCl) const {
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

private:
    void OnTaskEvent(wxCommandEvent& event);

    wxString taskTitle;
    wxString taskDescription;
    wxCheckBox * checkBox;
};


#endif //TASKPANEL_H
#ifndef TASKPANEL_H
#define TASKPANEL_H

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "../Model/Task.h"

enum BUTTON_IDs {
    TASK_PANEL = wxID_HIGHEST + 1,
    DELETE_BUTTON,
    MODIFY_TASK
};

class TaskPanel : public wxPanel {
public:
    TaskPanel(wxWindow* parent, const int taskIndex, bool isComplete, const wxString title);

    int getTaskIndex() const;
    bool isChecked();

private:
    void OnPanelClick(wxMouseEvent& event);
    void OnCheckBox(wxCommandEvent& event);
    void OnCloseClicked(wxCommandEvent& event);

    int taskIndex;
    wxCheckBox * checkBox;
    wxStaticText* label;
    wxPanel * kek;
    wxButton* deleteButton;
};


#endif //TASKPANEL_H
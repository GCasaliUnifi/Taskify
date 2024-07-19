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

class TaskPanel : public wxPanel {
public:
    TaskPanel(wxWindow *parent, const wxString &title, const wxString &description, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize);

    bool isCheked();

private:
    wxString taskTitle;
    wxString taskDescription;
    wxCheckBox * checkBox;
};


#endif //TASKPANEL_H
//
// Created by giacomo on 18/07/24.
//

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/menu.h>

#include "TaskPanel.h"

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    std::vector<TaskPanel *> doneTasks;
    std::vector<TaskPanel *> unDoneTasks;
};


#endif //MAINFRAME_H

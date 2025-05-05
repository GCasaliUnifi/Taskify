#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include <wx/log.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/dialog.h>
#include <wx/statbox.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/datectrl.h>

#include "TaskPanel.h"
#include "../Model/XMLParser.h"
#include "ThemeManager.h"

enum ELEMENTS_IDs {
    NEW_FILE = wxID_HIGHEST + 1,
    ORDER_BY_COMPLETION,
    ORDER_BY_DATE,
    FILE_PICKER,
    ADD_TASK,
    TASK_COMPLETED
};

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    void DisplayTasks(const std::vector<std::unique_ptr<Task>> &taskList);
    void ShowSelectedDetails(int index, const std::string &title, const std::string &descr, const std::string &dueDate);
    void SetNewPath(const std::string &newPath);
    void ResetPanelColours();
    void ResetFrame();
private:
    wxScrolledWindow* scrolledWindow;
    wxBoxSizer * tasksSizer;
    wxFilePickerCtrl* filePicker;

    wxTextCtrl* titleBox;
    wxTextCtrl* descriptionBox;
    wxTextCtrl* dateBox;
    wxButton * modifyTaskButton;
};


#endif //MAINFRAME_H

//
// Created by giacomo on 18/07/24.
//

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

#include "TaskPanel.h"
#include "XMLParser.h"

enum MENU_IDs {
    OPEN_FILE_MENU = wxID_HIGHEST + 1,
    SAVE_AS_MENU,
    SAVE_MENU
};

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnTaskCheck(wxCommandEvent& event);
    void OnTaskButtonClick(wxCommandEvent& event);
    void OnMenuItemClick(wxCommandEvent& event);
    void OnFileChange(wxFileDirPickerEvent& event);
    void openFile(const wxString& fileName);

    wxScrolledWindow* scrolledWindow;
    wxBoxSizer * tasksSizer;
    wxTextCtrl* titleBox;
    wxTextCtrl* descriptionBox;
    wxFilePickerCtrl* filePicker;

    std::vector<TaskPanel *> doneTasks;
    std::vector<TaskPanel *> unDoneTasks;

    XMLParser xmlParser;
};


#endif //MAINFRAME_H

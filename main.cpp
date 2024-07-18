#include <wx/wx.h>

#include "MainFrame.h"

class Taskify : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(Taskify);

bool Taskify::OnInit() {
    auto *mainFrame = new MainFrame(wxT("Test"), wxDefaultPosition, wxDefaultSize);
    mainFrame->Show(true);
    return true;
}

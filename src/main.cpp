#include <wx/wx.h>

#include "Model/XMLParser.h"
#include "View/MainFrame.h"
#include "Controller/Controller.h"

class Taskify : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(Taskify);

bool Taskify::OnInit() {
    auto *viewFrame = new MainFrame(wxT("Taskify"), wxDefaultPosition, wxDefaultSize);
    viewFrame->Show(true);
    viewFrame->Centre(wxBOTH);

    auto * model = new XMLParser();

    auto * controller = new Controller(viewFrame, model);

    return true;
}

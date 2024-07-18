#include <wx/wx.h>

class Taskify : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(Taskify);

bool Taskify::OnInit() {
    auto *frame = new wxFrame(nullptr, wxID_ANY, wxT("Test"));
    frame->Show(true);
    return true;
}

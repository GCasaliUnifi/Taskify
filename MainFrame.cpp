//
// Created by giacomo on 18/07/24.
//

#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) :
    wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    auto * verticalSizer = new wxBoxSizer(wxVERTICAL);
    auto * horizontalSizer =  new wxBoxSizer(wxHORIZONTAL);

    auto * topMenuBar = new wxMenuBar();

    auto * fileMenu = new wxMenu();
    fileMenu->Append(OPEN_FILE_MENU, wxT("&Open file\tCtrl-O"));
    fileMenu->Append(wxID_EXIT, wxT("&Exit\tEsc"));
    topMenuBar->Append(fileMenu, wxT("File"));

    auto * helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, wxT("About"));
    topMenuBar->Append(helpMenu, wxT("Help"));

    this->SetMenuBar(topMenuBar);

    auto * leftSizer = new wxBoxSizer(wxVERTICAL);
    auto * tasksSizer = new wxBoxSizer(wxVERTICAL);

    scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxBORDER_SUNKEN);
    for(int i = 0; i < 10; ++i) {
        auto * testTask = new TaskPanel(scrolledWindow, wxString::Format("%s %d", wxT("Titolo"), i+1), wxString::Format("%s %d", wxT("Descrizione"), i+1));
        this->unDoneTasks.push_back(testTask);
    }

    for (const auto i: unDoneTasks) {
        tasksSizer->Add(i, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    }

    for (const auto i: doneTasks) {
        tasksSizer->Add(i, 0, wxEXPAND | wxALL, 5);
    }

    scrolledWindow->SetSizer(tasksSizer);
    tasksSizer->FitInside(scrolledWindow);
    scrolledWindow->SetScrollRate(2, 10);

    auto addTaskButton = new wxButton(this, wxID_ANY, wxString::FromUTF8("➕"));

    leftSizer->Add(scrolledWindow, 1, wxEXPAND, 0);
    leftSizer->Add(addTaskButton, 0, wxALL | wxALIGN_CENTER, 5);

    auto * rightSizer = new wxBoxSizer(wxVERTICAL);
    // TODO fai in modo che il testo "placeholder" sia significativo e magari in grigietto.
    auto * titleSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Titolo"));
    titleBox = new wxTextCtrl(this, wxID_ANY, wxT("Placeholder"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);

    auto * descriptionSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Descrizione"));
    descriptionBox = new wxTextCtrl(this, wxID_ANY, wxT("Placeholder"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);

    titleSizer->Add(titleBox, 0, wxEXPAND | wxALL, 8);
    descriptionSizer->Add(descriptionBox, 1, wxEXPAND | wxALL, 10);

    rightSizer->Add(titleSizer, 0, wxEXPAND | wxBOTTOM, 5);
    rightSizer->Add(descriptionSizer, 1, wxEXPAND, 5);

    horizontalSizer->Add(leftSizer, 2, wxEXPAND | wxRIGHT, 10);
    horizontalSizer->Add(rightSizer, 1, wxEXPAND, 0);

    verticalSizer->Add(horizontalSizer, 1, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(verticalSizer);
    this->SetMinSize(wxSize(600, 400));

    this->Bind(wxEVT_CHECKBOX, &MainFrame::OnTaskCheck, this);
    this->Bind(wxEVT_BUTTON, &MainFrame::OnTaskButtonClick, this);
    this->Bind(wxEVT_MENU, &MainFrame::OnMenuItemClick, this);

}

void MainFrame::OnTaskCheck(wxCommandEvent &event) {
    wxObject* obj = event.GetEventObject();

    if(auto* callerTask = dynamic_cast<TaskPanel *>(obj)) {
        if(callerTask->isChecked()) {
            auto it = std::find(unDoneTasks.begin(), unDoneTasks.end(), callerTask);
            if(it != unDoneTasks.end()) {
                auto sizer = scrolledWindow->GetSizer();
                sizer->Detach(callerTask);
                sizer->Add(callerTask, 0, wxEXPAND | wxALL, 5);
                scrolledWindow->Layout();

                unDoneTasks.erase(it);
                doneTasks.push_back(callerTask);
            }
        } else {
            auto it = std::find(doneTasks.begin(), doneTasks.end(), callerTask);
            if(it != doneTasks.end()) {
                auto sizer = scrolledWindow->GetSizer();
                sizer->Detach(callerTask);
                sizer->Insert(0, callerTask, 0, wxEXPAND | wxALL, 5);
                scrolledWindow->Layout();

                doneTasks.erase(it);
                unDoneTasks.insert(unDoneTasks.begin(), callerTask);
            }
        }
    }
}

void MainFrame::OnTaskButtonClick(wxCommandEvent &event) {
    wxObject* obj = event.GetEventObject();
    if(auto* callerTask = dynamic_cast<TaskPanel *>(obj)) {
        switch (event.GetId()) {
            case TASK_BUTTON: {
                this->titleBox->SetValue(callerTask->getTaskTitle());
                this->descriptionBox->SetValue(callerTask->getTaskDescription());
                break;
            }

            case DELETE_BUTTON: {
                wxLogMessage("Premuto delete");
                break;
            }

            default:
                break;
        }
    }
}

void MainFrame::OnMenuItemClick(wxCommandEvent &event) {
    switch (event.GetId()) {
        case wxID_EXIT: {
            std::cout << "Premuto exit" << std::endl;
            break;
        }

        case wxID_ABOUT: {
            std::cout << "Premuto About" << std::endl;
            break;
        }

        case OPEN_FILE_MENU: {
            std::cout << "Premuto Open" << std::endl;
            this->xmlParser.openFile("/home/giacomo/Scrivania/UNIFI/1_anno/test.xml");
            this->xmlParser.parseXML();

            break;
        }

        default:
            break;
    }
}

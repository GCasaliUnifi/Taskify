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
    fileMenu->Append(wxID_EXIT, wxT("Exit"));
    topMenuBar->Append(fileMenu, wxT("File"));

    auto * helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, wxT("About"));
    topMenuBar->Append(helpMenu, wxT("Help"));

    this->SetMenuBar(topMenuBar);

    auto * leftSizer = new wxBoxSizer(wxVERTICAL);

    auto * scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);

    for(int i = 0; i < 16; ++i) {
        auto * testTask = new TaskPanel(scrolledWindow, wxT("titolo"), wxT("Descrizione"));
        this->unDoneTasks.push_back(testTask);
    }

    for (const auto i: unDoneTasks) {
        leftSizer->Add(i, 0, wxEXPAND | wxALL, 5);
    }

    for (const auto i: doneTasks) {
        leftSizer->Add(i, 0, wxEXPAND | wxALL, 5);
    }

    scrolledWindow->SetSizer(leftSizer);
    leftSizer->FitInside(scrolledWindow);
    scrolledWindow->SetScrollRate(2, 10);

    auto * rightPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    rightPanel->SetBackgroundColour(wxColour(100, 100, 200));

    horizontalSizer->Add(scrolledWindow, 2, wxEXPAND | wxRIGHT, 10);
    horizontalSizer->Add(rightPanel, 1, wxEXPAND, 0);

    verticalSizer->Add(horizontalSizer, 1, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(verticalSizer);
}

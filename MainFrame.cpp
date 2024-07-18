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
    auto * leftPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    leftPanel->SetBackgroundColour(wxColour(100, 200, 100));
    auto * testTask = new TaskPanel(this, wxT("titolo"), wxT("Descrizione"));

    leftSizer->Add(leftPanel, 0, wxEXPAND);
    leftSizer->Add(testTask, 0, wxEXPAND);

    auto * rightPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    rightPanel->SetBackgroundColour(wxColour(100, 100, 200));


    horizontalSizer->Add(leftSizer, 2, wxEXPAND | wxRIGHT, 10);
    horizontalSizer->Add(rightPanel, 1, wxEXPAND, 0);

    verticalSizer->Add(horizontalSizer, 1, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(verticalSizer);

}

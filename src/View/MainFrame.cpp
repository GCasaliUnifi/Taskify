#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(
    nullptr, wxID_ANY, title, pos, size) {

    wxLocale locale;
    locale.Init(wxLANGUAGE_ITALIAN);

    auto isSysDark = wxSystemSettings::GetAppearance().IsSystemDark();
    ThemeManager::GetInstance().SetDarkTheme(isSysDark);

    auto *verticalSizer = new wxBoxSizer(wxVERTICAL);
    auto *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    auto *topMenuBar = new wxMenuBar();

    auto *fileMenu = new wxMenu();
    fileMenu->Append(NEW_FILE, wxT("&New file\tCtrl-N"));
    fileMenu->Append(wxID_OPEN, wxT("&Open file\tCtrl-O"));
    fileMenu->Append(wxID_SAVE, wxT("&Save file\tCtrl-S"));
    fileMenu->Append(wxID_SAVEAS, wxT("&Save file as..\tCtrl-Shift-S"));
    fileMenu->Append(wxID_EXIT, wxT("&Exit\tEsc"));
    topMenuBar->Append(fileMenu, wxT("File"));

    auto *viewMenu = new wxMenu();
    auto* subViewMenu = new wxMenu;
    subViewMenu->AppendRadioItem(ORDER_BY_DATE, "data scadenza");
    subViewMenu->AppendRadioItem(ORDER_BY_COMPLETION, "completamento");
    viewMenu->AppendSubMenu(subViewMenu, "Ordina per ..");

    viewMenu->AppendSeparator();

    topMenuBar->Append(viewMenu, "View");

    auto *helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, wxT("About"));
    topMenuBar->Append(helpMenu, wxT("Help"));

    this->wxFrameBase::SetMenuBar(topMenuBar);

    auto *leftSizer = new wxBoxSizer(wxVERTICAL);
    tasksSizer = new wxBoxSizer(wxVERTICAL);

    scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                          wxHSCROLL | wxVSCROLL | wxBORDER_SUNKEN);

    if (tasksSizer->IsEmpty()) {
        auto txtNoTask = new wxStaticText(scrolledWindow, wxID_ANY, wxT("Nessun file selezionato..."));
        tasksSizer->Add(txtNoTask, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    }

    scrolledWindow->SetSizer(tasksSizer);
    tasksSizer->FitInside(scrolledWindow);
    scrolledWindow->SetScrollRate(2, 10);

    this->filePicker = new wxFilePickerCtrl(this, FILE_PICKER, "", "Selezionare un file XML", "XML files (*.xml)|*.xml");
    auto addTaskButton = new wxButton(this, ADD_TASK, wxString::FromUTF8("✚"));

    auto tmpFont = addTaskButton->GetFont();
    tmpFont.SetPointSize(15);
    addTaskButton->SetFont(tmpFont);
    addTaskButton->SetBackgroundColour(ThemeManager::GetInstance().GetCurrentTheme().buttonAdd);
    addTaskButton->SetForegroundColour(ThemeManager::GetInstance().GetCurrentTheme().buttonForeground);

    leftSizer->Add(filePicker, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
    leftSizer->Add(scrolledWindow, 1, wxEXPAND, 0);
    leftSizer->Add(addTaskButton, 0, wxALL | wxALIGN_CENTER, 5);

    auto *rightSizer = new wxBoxSizer(wxVERTICAL);
    auto *titleSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Titolo"));
    titleBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    titleBox->SetHint(wxT("Selezionare task..."));

    auto *dateSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Scadenza (YYYY-MM-DD)"));
    dateBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);

    auto *descriptionSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Descrizione"));
    descriptionBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                    wxTE_READONLY | wxTE_MULTILINE);
    descriptionBox->SetHint(wxT("Selezionare task..."));

    modifyTaskButton = new wxButton(this, MODIFY_TASK, wxT("Modifica task"));
    modifyTaskButton->SetBackgroundColour(ThemeManager::GetInstance().GetCurrentTheme().buttonBackground);
    modifyTaskButton->Disable();

    titleSizer->Add(titleBox, 0, wxEXPAND | wxALL, 8);
    dateSizer->Add(dateBox, 0, wxEXPAND | wxALL, 8);
    descriptionSizer->Add(descriptionBox, 1, wxEXPAND | wxALL, 10);

    rightSizer->Add(titleSizer, 0, wxEXPAND | wxBOTTOM, 5);
    rightSizer->Add(dateSizer, 0, wxEXPAND | wxBOTTOM, 5);
    rightSizer->Add(descriptionSizer, 1, wxEXPAND, 5);
    rightSizer->Add(modifyTaskButton, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

    horizontalSizer->Add(leftSizer, 2, wxEXPAND | wxRIGHT, 10);
    horizontalSizer->Add(rightSizer, 1, wxEXPAND, 0);

    verticalSizer->Add(horizontalSizer, 1, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(verticalSizer);
    this->SetMinSize(wxSize(600, 400));
}

void MainFrame::DisplayTasks(const std::vector<std::unique_ptr<Task>> &taskList) {
    this->tasksSizer->Clear(true);
    this->titleBox->Clear();
    this->descriptionBox->Clear();
    this->dateBox->Clear();
    this->modifyTaskButton->Disable();

    for (int i = 0; i < taskList.size(); ++i) {
        std::string cutTitle = taskList[i]->GetTitle();
        if (cutTitle.length() > 40)
            cutTitle = cutTitle.substr(0, 40) + "...";

        const auto tp = new TaskPanel(scrolledWindow, i, taskList[i]->IsCompleted(), cutTitle);
        tasksSizer->Add(tp, 0, wxEXPAND | wxALL, 5);
    }

    ResetPanelColours();

    scrolledWindow->FitInside();
    scrolledWindow->Layout();

}

void MainFrame::ShowSelectedDetails(int index, const std::string &title, const std::string &descr, const std::string& dueDate) {
    this->titleBox->SetValue(title);
    this->dateBox->SetValue(dueDate);
    this->descriptionBox->SetValue(descr);
    this->tasksSizer->GetItem(index)->GetWindow()->SetBackgroundColour(ThemeManager::GetInstance().GetCurrentTheme().buttonSelected);

    this->modifyTaskButton->Enable();

}

void MainFrame::SetNewPath(const std::string &newPath) {
    this->filePicker->SetPath(newPath);
}

void MainFrame::ResetPanelColours() {
    for (int i = 0; i < tasksSizer->GetItemCount(); ++i) {
        tasksSizer->GetItem(i)->GetWindow()->SetBackgroundColour(ThemeManager::GetInstance().GetCurrentTheme().buttonBackground);
    }
}

void MainFrame::ResetFrame() {
    this->tasksSizer->Clear(true);
    this->filePicker->SetPath("");
    this->titleBox->Clear();
    this->dateBox->Clear();
    this->descriptionBox->Clear();
    this->modifyTaskButton->Disable();

    this->SetTitle("Taskify");

    this->Layout();
}

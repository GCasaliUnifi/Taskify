#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(
    nullptr, wxID_ANY, title, pos, size) {
    auto *verticalSizer = new wxBoxSizer(wxVERTICAL);
    auto *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    auto *topMenuBar = new wxMenuBar();

    auto *fileMenu = new wxMenu();
    fileMenu->Append(OPEN_FILE_MENU, wxT("&Open file\tCtrl-O"));
    fileMenu->Append(SAVE_MENU, wxT("&Save file\tCtrl-S"));
    fileMenu->Append(SAVE_AS_MENU, wxT("&Save file as..\tCtrl-Shift-S"));
    fileMenu->Append(wxID_EXIT, wxT("&Exit\tEsc"));
    topMenuBar->Append(fileMenu, wxT("File"));

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

    this->filePicker = new wxFilePickerCtrl(this, wxID_ANY, "", "Selezionare un file XML", "XML files (*.xml)|*.xml");
    // auto addTaskButton = new wxButton(this, ADD_TASK, wxString::FromUTF8("➕"));
    // auto addTaskButton = new wxButton(this, ADD_TASK, wxString::FromUTF8("🞥"));
    auto addTaskButton = new wxButton(this, ADD_TASK, wxString::FromUTF8("✚"));

    auto tmpFont = addTaskButton->GetFont();
    tmpFont.SetPointSize(15);
    addTaskButton->SetFont(tmpFont);
    addTaskButton->SetBackgroundColour(wxColour(77, 113, 0));
    addTaskButton->SetForegroundColour(wxColour(255, 255, 255));

    leftSizer->Add(filePicker, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
    leftSizer->Add(scrolledWindow, 1, wxEXPAND, 0);
    leftSizer->Add(addTaskButton, 0, wxALL | wxALIGN_CENTER, 5);

    auto *rightSizer = new wxBoxSizer(wxVERTICAL);
    auto *titleSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Titolo"));
    titleBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    titleBox->SetHint(wxT("Selezionare task..."));

    auto *descriptionSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Descrizione"));
    descriptionBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                    wxTE_READONLY | wxTE_MULTILINE);
    descriptionBox->SetHint(wxT("Selezionare task..."));

    modifyTaskButton = new wxButton(this, MODIFY_TASK, wxT("Modifica task"));
    modifyTaskButton->Disable();

    titleSizer->Add(titleBox, 0, wxEXPAND | wxALL, 8);
    descriptionSizer->Add(descriptionBox, 1, wxEXPAND | wxALL, 10);

    rightSizer->Add(titleSizer, 0, wxEXPAND | wxBOTTOM, 5);
    rightSizer->Add(descriptionSizer, 1, wxEXPAND, 5);
    rightSizer->Add(modifyTaskButton, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

    horizontalSizer->Add(leftSizer, 2, wxEXPAND | wxRIGHT, 10);
    horizontalSizer->Add(rightSizer, 1, wxEXPAND, 0);

    verticalSizer->Add(horizontalSizer, 1, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(verticalSizer);
    this->SetMinSize(wxSize(600, 400));

    this->Bind(wxEVT_CHECKBOX, &MainFrame::OnTaskCheck, this);
    this->Bind(wxEVT_BUTTON, &MainFrame::OnTaskButtonClick, this);
    this->Bind(wxEVT_MENU, &MainFrame::OnMenuItemClick, this);
    this->Bind(wxEVT_FILEPICKER_CHANGED, &MainFrame::OnFileChange, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::OnTaskCheck(wxCommandEvent &event) {
    wxObject *obj = event.GetEventObject();

    if (auto *callerTask = dynamic_cast<TaskPanel *>(obj)) {
        if (callerTask->isChecked()) {
            auto it = std::find(unDoneTasks.begin(), unDoneTasks.end(), callerTask);
            if (it != unDoneTasks.end()) {
                auto sizer = scrolledWindow->GetSizer();
                sizer->Detach(callerTask);
                sizer->Add(callerTask, 0, wxEXPAND | wxALL, 5);
                scrolledWindow->Layout();

                unDoneTasks.erase(it);
                doneTasks.push_back(callerTask);
            }
        } else {
            auto it = std::find(doneTasks.begin(), doneTasks.end(), callerTask);
            if (it != doneTasks.end()) {
                auto sizer = scrolledWindow->GetSizer();
                sizer->Detach(callerTask);
                sizer->Insert(0, callerTask, 0, wxEXPAND | wxALL, 5);
                scrolledWindow->Layout();

                doneTasks.erase(it);
                unDoneTasks.insert(unDoneTasks.begin(), callerTask);
            }
        }

        if (!this->hasFileBeenModified) {
            this->hasFileBeenModified = true;
            SetTitle("* " + GetTitle());
        }
    }
}

void MainFrame::OnTaskButtonClick(wxCommandEvent &event) {
    wxObject *obj = event.GetEventObject();

    switch (event.GetId()) {
        case ADD_TASK: {
            // Apri un dialog con "titolo" e "descrizione".
            auto addTaskDialog = wxDialog(this, wxID_ANY, wxT("Aggiungi Task"));
            auto dialogMainSizer = new wxBoxSizer(wxVERTICAL);

            auto dialogTitleSizer = new wxStaticBoxSizer(wxVERTICAL, &addTaskDialog, wxT("Titolo Task"));
            auto dialogDescrSizer = new wxStaticBoxSizer(wxVERTICAL, &addTaskDialog, wxT("Descrizione Task"));

            auto titleCtrl = new wxTextCtrl(&addTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                            wxSize(400, -1));
            titleCtrl->SetHint(wxT("Titolo..."));
            dialogTitleSizer->Add(titleCtrl, 0, wxEXPAND | wxALL, 5);

            auto descrCtrl = new wxTextCtrl(&addTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1),
                                            wxTE_MULTILINE);
            descrCtrl->SetHint(wxT("Descrizione..."));
            dialogDescrSizer->Add(descrCtrl, 0, wxEXPAND | wxALL, 5);

            auto buttonSizer = addTaskDialog.CreateSeparatedButtonSizer(wxYES | wxCANCEL);

            dialogMainSizer->Add(dialogTitleSizer, 0, wxEXPAND | wxALL, 5);
            dialogMainSizer->Add(dialogDescrSizer, 1, wxEXPAND | wxALL, 5);
            dialogMainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

            addTaskDialog.SetSizerAndFit(dialogMainSizer);
            addTaskDialog.Layout();

            if (addTaskDialog.ShowModal() == wxID_YES) {
                if (tasksSizer->GetItemCount() > 0) {
                    const size_t index = 0;
                    const auto firstItem = tasksSizer->GetItem(index);
                    auto win = firstItem->GetWindow();

                    // Se nessun file è selezionato, il primo (e unico) elemento nel sizer è un wxStaticText.
                    // Con questo wxDynamicCast controllo se sono in questa situazione e in tal caso svuoto il Sizer.
                    if (auto text = wxDynamicCast(win, wxStaticText)) {
                        tasksSizer->Clear(true);
                    }
                }

                if (!titleCtrl->IsEmpty()) {
                    auto newTask = new TaskPanel(scrolledWindow, titleCtrl->GetValue(), descrCtrl->GetValue());
                    this->unDoneTasks.push_back(newTask);
                    tasksSizer->Add(newTask, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
                    scrolledWindow->FitInside();
                    scrolledWindow->Layout();

                    if (!this->hasFileBeenModified) {
                        this->hasFileBeenModified = true;
                        SetTitle("* " + GetTitle());
                    }

                    if (selectedTask) {
                        selectedTask->setTaskColour(wxColour(53, 53, 53));
                        selectedTask = nullptr;
                    }
                } else {
                    wxLogMessage("Impossibile aggiungere task senza titolo!");
                }
            }

            break;
        }

        case TASK_BUTTON: {
            if (auto *callerTask = dynamic_cast<TaskPanel *>(obj)) {
                this->titleBox->SetValue(callerTask->getTaskTitle());
                this->descriptionBox->SetValue(callerTask->getTaskDescription());
                modifyTaskButton->Enable();

                if (selectedTask) {
                    selectedTask->setTaskColour(wxColour(53, 53, 53));
                }
                selectedTask = callerTask;
                callerTask->setTaskColour(wxColour(0,83, 53));
            }
            break;
        }

        case DELETE_BUTTON: {
            if (auto *callerTask = dynamic_cast<TaskPanel *>(obj)) {
                int answer = wxMessageBox("Delete task?", "Confirm", wxYES_NO | wxICON_ERROR, this);
                if (answer == wxYES) {
                    if (selectedTask) {
                        selectedTask->setTaskColour(wxColour(53, 53, 53));
                        selectedTask = nullptr;
                    }

                    if (!callerTask->isChecked()) {
                        auto it = std::find(unDoneTasks.begin(), unDoneTasks.end(), callerTask);
                        if (it != unDoneTasks.end()) {
                            auto sizer = scrolledWindow->GetSizer();
                            sizer->Detach(callerTask);
                            callerTask->Destroy();
                            unDoneTasks.erase(it);

                            scrolledWindow->Layout();
                        }
                    } else {
                        auto it = std::find(doneTasks.begin(), doneTasks.end(), callerTask);
                        if (it != doneTasks.end()) {
                            auto sizer = scrolledWindow->GetSizer();
                            sizer->Detach(callerTask);
                            callerTask->Destroy();
                            doneTasks.erase(it);

                            scrolledWindow->Layout();
                        }
                    }

                    if (!isFileOpen && this->tasksSizer->GetItemCount() == 0) {
                        this->hasFileBeenModified = false;
                        SetTitle("Taskify");
                    } else {
                        if (!this->hasFileBeenModified) {
                            this->hasFileBeenModified = true;
                            SetTitle("* " + GetTitle());
                        }
                    }

                    this->titleBox->Clear();
                    this->descriptionBox->Clear();
                    this->descriptionBox->SetHint(wxT("Selezionare task..."));
                    modifyTaskButton->Disable();
                }
            }

            break;
        }

        case MODIFY_TASK: {
            const wxString oldTitle = selectedTask->getTaskTitle();
            const wxString oldDescr = selectedTask->getTaskDescription();

            auto modTaskDialog = wxDialog(this, wxID_ANY, wxT("Modifica Task"));
            auto dialogMainSizer = new wxBoxSizer(wxVERTICAL);

            auto dialogTitleSizer = new wxStaticBoxSizer(wxVERTICAL, &modTaskDialog, wxT("Nuovo titolo"));
            auto dialogDescrSizer = new wxStaticBoxSizer(wxVERTICAL, &modTaskDialog, wxT("Nuova descrizione"));

            auto titleCtrl = new wxTextCtrl(&modTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                            wxSize(400, -1));
            titleCtrl->SetHint(oldTitle);
            dialogTitleSizer->Add(titleCtrl, 0, wxEXPAND | wxALL, 5);

            auto descrCtrl = new wxTextCtrl(&modTaskDialog, wxID_ANY, oldDescr, wxDefaultPosition, wxSize(400, -1),
                                            wxTE_MULTILINE);
            dialogDescrSizer->Add(descrCtrl, 0, wxEXPAND | wxALL, 5);

            auto buttonSizer = modTaskDialog.CreateSeparatedButtonSizer(wxYES | wxCANCEL);

            dialogMainSizer->Add(dialogTitleSizer, 0, wxEXPAND | wxALL, 5);
            dialogMainSizer->Add(dialogDescrSizer, 1, wxEXPAND | wxALL, 5);
            dialogMainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

            modTaskDialog.SetSizerAndFit(dialogMainSizer);
            modTaskDialog.Layout();

            if (modTaskDialog.ShowModal() == wxID_YES) {
                if (!titleCtrl->IsEmpty()) {
                    selectedTask->setTaskTitle(titleCtrl->GetValue());
                    this->titleBox->SetValue(titleCtrl->GetValue());
                }

                selectedTask->setTaskDescription(descrCtrl->GetValue());
                this->descriptionBox->SetValue(descrCtrl->GetValue());

                if (!this->hasFileBeenModified) {
                    this->hasFileBeenModified = true;
                    SetTitle("* " + GetTitle());
                }
            }

            break;
        }

        default:
            break;
    }
}

void MainFrame::OnMenuItemClick(wxCommandEvent &event) {
    switch (event.GetId()) {
        case wxID_EXIT: {
            this->Close();
            break;
        }

        case wxID_ABOUT: {
            wxMessageBox(
                "Sviluppatore: giacomo.casali@edu.unifi.it\n\nCodice Sorgente:\nhttps://github.com/GCasaliUnifi/Taskify",
                "About Taskify", wxOK | wxICON_INFORMATION);
            break;
        }

        case OPEN_FILE_MENU: {
            wxFileDialog openFileDialog(this, wxT("Selzionare file XML"), "", "", "XML files (*.xml)|*.xml",
                                        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

            if (openFileDialog.ShowModal() == wxID_OK) {
                wxString newPath = openFileDialog.GetPath();
                this->filePicker->SetPath(newPath);

                // Crea un evento wxEVT_FILEPICKER_CHANGED poiché "SetPath()" non lo genera automaticamente.
                wxCommandEvent fileChangeEvent(wxEVT_FILEPICKER_CHANGED, filePicker->GetId());
                fileChangeEvent.SetEventObject(filePicker);
                fileChangeEvent.SetString(newPath);

                filePicker->GetEventHandler()->ProcessEvent(fileChangeEvent);
            }

            break;
        }

        case SAVE_MENU: {
            if (this->unDoneTasks.empty() && this->doneTasks.empty()) {
                int answer = wxMessageBox("Nessuna task, salvare comunque?", "Conferma", wxYES_NO | wxICON_ERROR, this);
                if (answer != wxYES)
                    break;
            }

            if (this->isFileOpen) {
                auto savePath = this->filePicker->GetPath();
                this->saveFile(savePath);
                this->hasFileBeenModified = false;
                SetTitle("Taskify");
                break;
            }
        }

        case SAVE_AS_MENU: {
            if (this->unDoneTasks.empty() && this->doneTasks.empty()) {
                int answer = wxMessageBox("Nessuna task, salvare comunque?", "Conferma", wxYES_NO | wxICON_ERROR, this);
                if (answer != wxYES)
                    break;
            }

            wxFileDialog saveFileDialog(this, _("Save XML file"), "", "",
                                            "XML files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

            if (saveFileDialog.ShowModal() == wxID_OK) {
                wxString newPath = saveFileDialog.GetPath();
                this->filePicker->SetPath(newPath);
                this->saveFile(newPath);
                this->hasFileBeenModified = false;
                SetTitle("Taskify");
                this->isFileOpen = true;
            }

            break;
        }

        default:
            break;
    }
}

void MainFrame::OnFileChange(wxFileDirPickerEvent &event) {
    if (this->hasFileBeenModified) {
        if (wxMessageBox(_("Ci sono delle modifiche non salvate! Procedere?"), _("Confermare"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO) {
            wxYield();
            return;
        }
    }
    this->openFile(this->filePicker->GetPath());
}

void MainFrame::OnClose(wxCloseEvent &event) {
    if (hasFileBeenModified) {
        int response = wxMessageBox(
                "Vuoi salvare le modifiche prima di uscire?",
                "Conferma chiusura",
                wxYES_NO | wxCANCEL | wxICON_QUESTION,
                this);

        if (response == wxYES) {
            auto savePath = this->filePicker->GetPath();
            this->saveFile(savePath);
            event.Skip();
        } else if (response == wxNO) {
            event.Skip();
        } else if (response == wxCANCEL) {
            event.Veto();
        }
    } else {
        event.Skip();
    }
}

void MainFrame::openFile(const wxString &fileName) {
    if (this->xmlParser.openFile(fileName)) {
        this->xmlParser.parseXML();
        auto taskList = xmlParser.getTaskList();

        this->unDoneTasks.clear();
        this->doneTasks.clear();
        this->tasksSizer->Clear(true);

        for (const auto &task: taskList) {
            auto *readTask = new TaskPanel(scrolledWindow, wxString::FromUTF8(std::get<0>(task).c_str()),
                                           wxString::FromUTF8(std::get<1>(task).c_str()));

            if (std::get<2>(task)) {
                this->doneTasks.push_back(readTask);
            } else {
                this->unDoneTasks.push_back(readTask);
            }
        }

        for (const auto i: unDoneTasks) {
            tasksSizer->Add(i, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
        }

        for (const auto i: doneTasks) {
            tasksSizer->Add(i, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
            i->checkTask();
        }

        this->isFileOpen = true;
        this->hasFileBeenModified = false;
        SetTitle("Taskify");
        this->titleBox->Clear();
        this->descriptionBox->Clear();
        this->descriptionBox->SetHint(wxT("Selezionare task..."));

        tasksSizer->Layout();
        scrolledWindow->FitInside();
        scrolledWindow->Layout();
        this->Refresh();
    } else {
        this->filePicker->SetPath("");
    }
}

void MainFrame::saveFile(const wxString &fileName) {
    // Genero tasklist dalla lista delle task;
    std::vector<std::tuple<std::string, std::string, bool> > currentTasklist;
    for (auto ut: unDoneTasks) {
        std::tuple<std::string, std::string, bool> tmpTask;
        std::get<0>(tmpTask) = ut->getTaskTitle().ToUTF8().data();
        std::get<1>(tmpTask) = ut->getTaskDescription().ToUTF8().data();
        std::get<2>(tmpTask) = false;
        currentTasklist.push_back(tmpTask);
    }

    for (auto ut: doneTasks) {
        std::tuple<std::string, std::string, bool> tmpTask;
        std::get<0>(tmpTask) = ut->getTaskTitle().ToUTF8().data();
        std::get<1>(tmpTask) = ut->getTaskDescription().ToUTF8().data();
        std::get<2>(tmpTask) = true;
        currentTasklist.push_back(tmpTask);
    }

    this->xmlParser.setTaskList(currentTasklist);
    this->xmlParser.serializeXML();
    if (this->xmlParser.saveToFile(fileName)) {
        wxLogMessage("File salvato con successo!");
        wxLog::FlushActive();
    }
    this->filePicker->SetPath(filePicker->GetPath());
}

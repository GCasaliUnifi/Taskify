//
// Created by giacomo on 18/07/24.
//

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

    this->SetMenuBar(topMenuBar);

    auto *leftSizer = new wxBoxSizer(wxVERTICAL);
    tasksSizer = new wxBoxSizer(wxVERTICAL);

    scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                          wxHSCROLL | wxVSCROLL | wxBORDER_SUNKEN);
    // TODO fai in modo da caricare dall'ultimo file aperto
    // for (const auto i: unDoneTasks) {
    //     tasksSizer->Add(i, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    // }
    //
    // for (const auto i: doneTasks) {
    //     tasksSizer->Add(i, 0, wxEXPAND | wxALL, 5);
    // }

    if (tasksSizer->IsEmpty()) {
        auto txtNoTask = new wxStaticText(scrolledWindow, wxID_ANY, wxT("Nessun file selezionato..."));
        tasksSizer->Add(txtNoTask, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    }

    scrolledWindow->SetSizer(tasksSizer);
    tasksSizer->FitInside(scrolledWindow);
    scrolledWindow->SetScrollRate(2, 10);

    this->filePicker = new wxFilePickerCtrl(this, wxID_ANY);
    auto addTaskButton = new wxButton(this, ADD_TASK, wxString::FromUTF8("➕"));

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
    this->Bind(wxEVT_FILEPICKER_CHANGED, &MainFrame::OnFileChange, this);
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
    }
}

void MainFrame::OnTaskButtonClick(wxCommandEvent &event) {
    wxObject *obj = event.GetEventObject();

    switch(event.GetId()) {
        case ADD_TASK: {
            // Apri un dialog con "titolo" e "descrizione".
            auto addTaskDialog = wxDialog(this, wxID_ANY, wxT("Aggiungi Task"));
            auto dialogMainSizer = new wxBoxSizer(wxVERTICAL);

            // FIXME la documentazione consiglia l'uso di
            //      wxStaticBox *box = new wxStaticBox(panel, wxID_ANY, "StaticBox");
            // al posto dello staticboxsizer con dentro i figli direttamente.
            auto dialogTitleSizer = new wxStaticBoxSizer(wxVERTICAL, &addTaskDialog, wxT("Titolo Task"));
            auto dialogDescrSizer = new wxStaticBoxSizer(wxVERTICAL, &addTaskDialog, wxT("Descrizione Task"));

            auto titleCtrl = new wxTextCtrl(&addTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1));
            titleCtrl->SetHint(wxT("Titolo..."));
            dialogTitleSizer->Add(titleCtrl, 0, wxEXPAND | wxALL, 5);

            auto descrCtrl = new wxTextCtrl(&addTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), wxTE_MULTILINE);
            descrCtrl->SetHint(wxT("Descrizione..."));
            dialogDescrSizer->Add(descrCtrl, 0, wxEXPAND | wxALL, 5);

            // TODO valuta se colorare diversamente i buttons.
            auto buttonSizer = addTaskDialog.CreateSeparatedButtonSizer(wxYES | wxCANCEL);

            dialogMainSizer->Add(dialogTitleSizer, 0, wxEXPAND | wxALL, 5);
            dialogMainSizer->Add(dialogDescrSizer, 1, wxEXPAND | wxALL, 5);
            dialogMainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

            addTaskDialog.SetSizerAndFit(dialogMainSizer);
            addTaskDialog.Layout();

            if(addTaskDialog.ShowModal() == wxID_YES) {
                if(tasksSizer->GetItemCount() > 0) {
                    // Devo dichiararlo fuori poiché dentro dà problemi.
                    const size_t index = 0;
                    const auto firstItem = tasksSizer->GetItem(index);
                    auto win = firstItem->GetWindow();

                    // Se nessun file è selezionato, il primo (e unico) elemento nel sizer è un wxStaticText.
                    // Con questo wxDynamicCast controllo se sono in questa situazione e in tal caso svuoto il Sizer.
                    if(auto text = wxDynamicCast(win, wxStaticText)) {
                        tasksSizer->Clear(true);
                    }
                }

                if(!titleCtrl->IsEmpty()) {
                    auto newTask = new TaskPanel(scrolledWindow, titleCtrl->GetValue(), descrCtrl->GetValue());
                    this->unDoneTasks.push_back(newTask);
                    tasksSizer->Add(newTask, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
                    scrolledWindow->Layout();
                    if(isFileOpen) {
                        this->hasFileBeenModified = true;
                    }
                } else {
                    wxLogMessage("Impossibile aggiungere task senza titolo!");
                }
            }

            break;
        }

        case TASK_BUTTON: {
            if(const auto* callerTask = dynamic_cast<TaskPanel *>(obj)) {
                this->titleBox->SetValue(callerTask->getTaskTitle());
                this->descriptionBox->SetValue(callerTask->getTaskDescription());
            }
            break;
        }

        case DELETE_BUTTON: {
            if(auto* callerTask = dynamic_cast<TaskPanel *>(obj)) {
                // TODO usa un Dialog Customizzato maybe?
                int answer = wxMessageBox("Delete task?", "Confirm", wxYES_NO | wxICON_ERROR, this);
                if(answer == wxYES) {
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
                }
            }
            //TODO quando non ci sono più task fare in modo che SE un file è selezionato appenda un testo tipo "Nessuna task nel file!".
            this->hasFileBeenModified = true;
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
            std::cout << "Premuto About" << std::endl;
            break;
        }

        case OPEN_FILE_MENU: {
            wxFileDialog openFileDialog(this, wxT("Open XML file"), "", "", "XML files (*.xml)|*.xml",
                                        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

            if (openFileDialog.ShowModal() == wxID_OK) {
                wxString newPath = openFileDialog.GetPath();
                this->filePicker->SetPath(newPath);

                // Crea un evento wxEVT_FILEPICKER_CHANGED poiché "SetPath()" non lo genera da solo.
                wxCommandEvent fileChangeEvent(wxEVT_FILEPICKER_CHANGED, filePicker->GetId());
                fileChangeEvent.SetEventObject(filePicker);
                fileChangeEvent.SetString(newPath);

                filePicker->GetEventHandler()->ProcessEvent(fileChangeEvent);
            }

            break;
        }

        case SAVE_MENU: {
            // TODO implementa salvataggio
            std::cout << "Premuto Save" << std::endl;
            this->hasFileBeenModified = false;
            break;
        }

        case SAVE_AS_MENU: {
            // TODO implementa salvataggio con nome.
            wxFileDialog saveFileDialog(this, _("Save XML file"), "", "",
                   "XML files (*.xml)|*.xml", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
            break;
        }

        default:
            break;
    }
}

void MainFrame::OnFileChange(wxFileDirPickerEvent &event) {
    // TODO controlli sul salvataggio
    if (this->hasFileBeenModified) {
        if (wxMessageBox(_("Current content has not been saved! Proceed?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
            return;
    }
    this->openFile(this->filePicker->GetPath());
    this->isFileOpen = true;
    this->hasFileBeenModified = false;
}

void MainFrame::openFile(const wxString &fileName) {
    this->xmlParser.openFile(fileName);
    this->xmlParser.parseXML();
    auto taskList = xmlParser.getTaskList();

    for (auto t: unDoneTasks) {
        delete t;
    }
    this->unDoneTasks.clear();

    this->tasksSizer->Clear(true);

    for (const auto &task: taskList) {
        auto *testTask = new TaskPanel(scrolledWindow, wxString::Format("%s", task.first),
                                       wxString::Format("%s", task.second));
        this->unDoneTasks.push_back(testTask);
    }

    for (const auto i: unDoneTasks) {
        tasksSizer->Add(i, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    }

    for (const auto i: doneTasks) {
        tasksSizer->Add(i, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    }

    tasksSizer->Layout();
    scrolledWindow->Layout();
    this->Refresh();
}

void MainFrame::saveFile(const wxString &fileName) {
    // TODO Genera tasklist dalla lista delle task;
    //this->xmlParser.setTaskList(...);
    this->xmlParser.serializeXML();
    this->xmlParser.saveToFile(fileName);
    // Setta nuovo path del file picker
}

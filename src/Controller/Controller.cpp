#include "Controller.h"

Controller::Controller(MainFrame *view, XMLParser *model)
    : view(view), model(model) {
    BindEvents();
}

void Controller::BindEvents() {
    view->Bind(wxEVT_MENU, &Controller::OnMenuOpen, this, wxID_OPEN);
    view->Bind(wxEVT_MENU, &Controller::OnMenuSave, this, wxID_SAVE);
    view->Bind(wxEVT_MENU, &Controller::OnMenuSaveAs, this, wxID_SAVEAS);
    view->Bind(wxEVT_MENU, &Controller::OnMenuNew, this, NEW_FILE);
    view->Bind(wxEVT_MENU, &Controller::OnMenuShowCompleted, this, SHOW_COMPLETED_MENU);
    view->Bind(wxEVT_MENU, &Controller::OnMenuAbout, this, wxID_ABOUT);

    view->Bind(wxEVT_FILEPICKER_CHANGED, &Controller::OnFilePicked, this, FILE_PICKER);

    view->Bind(wxEVT_LEFT_UP, &Controller::OnTaskClick, this);
    view->Bind(wxEVT_CHECKBOX, &Controller::OnMarkTaskCompleted, this);
    view->Bind(wxEVT_BUTTON, &Controller::OnAddTask, this, ADD_TASK);
    view->Bind(wxEVT_BUTTON, &Controller::OnDeleteTask, this, DELETE_BUTTON);
    view->Bind(wxEVT_BUTTON, &Controller::OnModifySelectedTask, this, MODIFY_TASK);
}

void Controller::OnMenuNew(wxCommandEvent &event) {
    if (!PromptSaveIfNeeded())
        return;


    model->clearTasks();
    view->ResetFrame();

}

void Controller::OnMenuSaveAs(wxCommandEvent &event) {
    // case SAVE_AS_MENU: {
    //     if (this->unDoneTasks.empty() && this->doneTasks.empty()) {
    //         int answer = wxMessageBox("Nessuna task, salvare comunque?", "Conferma", wxYES_NO | wxICON_ERROR, this);
    //         if (answer != wxYES)
    //             break;
    //     }
    //
    //     wxFileDialog saveFileDialog(this, _("Save XML file"), "", "",
    //                                 "XML files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    //
    //     if (saveFileDialog.ShowModal() == wxID_OK) {
    //         wxString newPath = saveFileDialog.GetPath();
    //         this->filePicker->SetPath(newPath);
    //         this->saveFile(newPath);
    //         this->hasFileBeenModified = false;
    //         SetTitle("Taskify");
    //         this->isFileOpen = true;
    //     }
    //
    //     break;
    // }
}


void Controller::OnMenuSave(wxCommandEvent &event) {
    if (model->saveToFile(currentFilePath)) {
        hasFileBeenModified = false;
        UpdateWindowTitle();
    }
}

bool Controller::PromptSaveIfNeeded() {
    if (!hasFileBeenModified)
        return true;

    int result = wxMessageBox(
        "Il file è stato modificato. Vuoi salvare prima di continuare?",
        "Salva modifiche",
        wxYES_NO | wxCANCEL | wxICON_QUESTION,
        view
    );

    if (result == wxYES) {
        if (currentFilePath.empty()) {
            wxFileDialog saveDlg(view, "Salva come", "", "",
                                 "File XML (*.xml)|*.xml",
                                 wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

            if (saveDlg.ShowModal() == wxID_CANCEL)
                return false;

            currentFilePath = saveDlg.GetPath().ToStdString();
        }

        if (!model->saveToFile(currentFilePath)) {
            wxMessageBox("Errore nel salvataggio del file.", "Errore", wxICON_ERROR);
            return false;
        }

        hasFileBeenModified = false;
        // SALVATAGGIO RIUSCITO
        UpdateWindowTitle();
        return true;
    } else if (result == wxNO) {
        return true;
    }

    return false; // Qui solo wxCancel
}

void Controller::OnMenuOpen(wxCommandEvent &event) {
    if (!PromptSaveIfNeeded())
        return;

    wxFileDialog openDlg(view, "Apri file XML", "", "",
                         "File XML (*.xml)|*.xml", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openDlg.ShowModal() == wxID_CANCEL)
        return;

    std::string path = openDlg.GetPath().ToStdString();
    if (model->loadFromFile(path)) {
        currentFilePath = path;
        isFileOpen = true;
        hasFileBeenModified = false;
        UpdateWindowTitle();
        view->DisplayTasks(model->GetTasks());
    } else {
        wxMessageBox("Errore nel caricamento dei file.", "Errore", wxICON_ERROR);
    }
}

void Controller::OnMenuClose(wxCloseEvent &close) {
    if (!PromptSaveIfNeeded())
        return;

    close.Skip();
}

void Controller::UpdateWindowTitle() const {
    std::string title = "Taskify";
    if (isFileOpen) {
        std::string fileName = currentFilePath.substr(currentFilePath.find_last_of("/\\") + 1);
        if (hasFileBeenModified)
            title += " - *" + fileName;
        else
            title += " - " + fileName;
    } else {
        if (hasFileBeenModified) {
            title = "* " + title;
        }
    }

    view->SetTitle(title);
}


void Controller::OnAddTask(wxCommandEvent &event) {
    auto addTaskDialog = wxDialog(view, wxID_ANY, wxT("Aggiungi Task"));
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
        if (!titleCtrl->IsEmpty()) {
            model->addTask(titleCtrl->GetValue().ToStdString(), descrCtrl->GetValue().ToStdString());

            if (!this->hasFileBeenModified) {
                std::cout << "modified: " << this->hasFileBeenModified << std::endl;
                this->hasFileBeenModified = true;
                UpdateWindowTitle();
                view->DisplayTasks(model->GetTasks());
            }
        } else {
            wxLogMessage("Impossibile aggiungere task senza titolo!");
        }
    }
}


void Controller::OnModifySelectedTask(wxCommandEvent &event) {
    // case MODIFY_TASK: {
    //         const wxString oldTitle = selectedTask->getTaskTitle();
    //         const wxString oldDescr = selectedTask->getTaskDescription();
    //
    //         auto modTaskDialog = wxDialog(this, wxID_ANY, wxT("Modifica Task"));
    //         auto dialogMainSizer = new wxBoxSizer(wxVERTICAL);
    //
    //         auto dialogTitleSizer = new wxStaticBoxSizer(wxVERTICAL, &modTaskDialog, wxT("Nuovo titolo"));
    //         auto dialogDescrSizer = new wxStaticBoxSizer(wxVERTICAL, &modTaskDialog, wxT("Nuova descrizione"));
    //
    //         auto titleCtrl = new wxTextCtrl(&modTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition,
    //                                         wxSize(400, -1));
    //         titleCtrl->SetHint(oldTitle);
    //         dialogTitleSizer->Add(titleCtrl, 0, wxEXPAND | wxALL, 5);
    //
    //         auto descrCtrl = new wxTextCtrl(&modTaskDialog, wxID_ANY, oldDescr, wxDefaultPosition, wxSize(400, -1),
    //                                         wxTE_MULTILINE);
    //         dialogDescrSizer->Add(descrCtrl, 0, wxEXPAND | wxALL, 5);
    //
    //         auto buttonSizer = modTaskDialog.CreateSeparatedButtonSizer(wxYES | wxCANCEL);
    //
    //         dialogMainSizer->Add(dialogTitleSizer, 0, wxEXPAND | wxALL, 5);
    //         dialogMainSizer->Add(dialogDescrSizer, 1, wxEXPAND | wxALL, 5);
    //         dialogMainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);
    //
    //         modTaskDialog.SetSizerAndFit(dialogMainSizer);
    //         modTaskDialog.Layout();
    //
    //         if (modTaskDialog.ShowModal() == wxID_YES) {
    //             if (!titleCtrl->IsEmpty()) {
    //                 selectedTask->setTaskTitle(titleCtrl->GetValue());
    //                 this->titleBox->SetValue(titleCtrl->GetValue());
    //             }
    //
    //             selectedTask->setTaskDescription(descrCtrl->GetValue());
    //             this->descriptionBox->SetValue(descrCtrl->GetValue());
    //
    //             if (!this->hasFileBeenModified) {
    //                 this->hasFileBeenModified = true;
    //                 SetTitle("* " + GetTitle());
    //             }
    //         }
    //
    //         break;
    //     }
}


void Controller::OnTaskClick(wxMouseEvent &event) {
    auto *panel = dynamic_cast<TaskPanel *>(event.GetEventObject());
    if (panel) {
        wxLogMessage("Clicked on: %s", model->getTaskByIndex(panel->getTaskIndex())->GetTitle());
    }

    // TODO seleziona la task cliccata

    event.Skip();
}

void Controller::OnMarkTaskCompleted(wxCommandEvent &event) {
    auto *panel = dynamic_cast<TaskPanel *>(event.GetEventObject());
    if (panel) {
        wxLogMessage("Marked: %s -> %d", model->getTaskByIndex(panel->getTaskIndex())->GetTitle(), panel->isChecked());
        hasFileBeenModified = true;
        UpdateWindowTitle();
    } else {
        std::cout << "no cast checkbox" <<std::endl;
    }
}


void Controller::OnDeleteTask(wxCommandEvent &event) {
    auto *panel = dynamic_cast<TaskPanel *>(event.GetEventObject());
    if (panel) {
        wxLogMessage("Closed Task: %s", model->getTaskByIndex(panel->getTaskIndex())->GetTitle());
    }
}

void Controller::OnFilePicked(wxFileDirPickerEvent &event) {
    if (!PromptSaveIfNeeded())
        return;

    if (model->loadFromFile(event.GetPath().ToStdString())) {
        currentFilePath = event.GetPath().ToStdString();
        isFileOpen = true;
        hasFileBeenModified = false;
        UpdateWindowTitle();
        view->DisplayTasks(model->GetTasks());
    } else {
        wxMessageBox("Errore nel caricamento dei file.", "Errore", wxICON_ERROR);
    }
}


void Controller::OnMenuAbout(wxCommandEvent &event) {
    wxMessageBox(
        "Sviluppatore: giacomo.casali@edu.unifi.it\n\nCodice Sorgente:\nhttps://github.com/GCasaliUnifi/Taskify",
        "About Taskify",
        wxOK | wxICON_INFORMATION);
}

void Controller::OnMenuShowCompleted(wxCommandEvent &event) {
    // Aggiungi un bool alla funzione già esistente chiaamta DisplayTasks in MainFrame
}

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
    isFileOpen = false;
}

void Controller::OnMenuSaveAs(wxCommandEvent &event) {
    if (model->GetTasks().empty()) {
        int answer = wxMessageBox("Nessuna task, salvare comunque?", "Conferma", wxYES_NO | wxICON_ERROR, view);
        if (answer != wxYES)
            return;
    }

    wxFileDialog saveFileDialog(view, _("Salva XML file"), "", "",
                                "XML files (*.xml)|*.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        std::string newPath = saveFileDialog.GetPath().ToStdString();

        currentFilePath = newPath;
        model->saveToFile(currentFilePath);
        view->SetNewPath(newPath);

        this->hasFileBeenModified = false;
        this->isFileOpen = true;
        UpdateWindowTitle();
    }
}


void Controller::OnMenuSave(wxCommandEvent &event) {
    if (isFileOpen) {
        if (model->saveToFile(currentFilePath)) {
            hasFileBeenModified = false;
            UpdateWindowTitle();
        }
    } else {
        wxCommandEvent saveAsEvt(wxEVT_MENU);
        saveAsEvt.SetId(wxID_SAVEAS);
        wxPostEvent(view, saveAsEvt);
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
    auto dialogDateSizer = new wxStaticBoxSizer(wxVERTICAL, &addTaskDialog, wxT("Scadenza Task"));
    auto dialogDescrSizer = new wxStaticBoxSizer(wxVERTICAL, &addTaskDialog, wxT("Descrizione Task"));

    auto titleCtrl = new wxTextCtrl(&addTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxSize(400, -1));
    titleCtrl->SetHint(wxT("Titolo..."));
    dialogTitleSizer->Add(titleCtrl, 0, wxEXPAND | wxALL, 5);

    auto dateCtrl = new wxDatePickerCtrl(&addTaskDialog, wxID_ANY, wxDateTime::Now());
    dialogDateSizer->Add(dateCtrl, 0, wxEXPAND | wxALL, 5);

    auto descrCtrl = new wxTextCtrl(&addTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1),
                                    wxTE_MULTILINE);
    descrCtrl->SetHint(wxT("Descrizione..."));
    dialogDescrSizer->Add(descrCtrl, 0, wxEXPAND | wxALL, 5);

    auto buttonSizer = addTaskDialog.CreateSeparatedButtonSizer(wxYES | wxCANCEL);

    dialogMainSizer->Add(dialogTitleSizer, 0, wxEXPAND | wxALL, 5);
    dialogMainSizer->Add(dialogDateSizer, 0, wxEXPAND | wxALL, 5);
    dialogMainSizer->Add(dialogDescrSizer, 1, wxEXPAND | wxALL, 5);
    dialogMainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

    addTaskDialog.SetSizerAndFit(dialogMainSizer);
    addTaskDialog.Layout();

    if (addTaskDialog.ShowModal() == wxID_YES) {
        if (!titleCtrl->IsEmpty()) {
            std::string date = dateCtrl->GetValue().FormatISODate().ToStdString();
            model->addTask(titleCtrl->GetValue().ToStdString(), descrCtrl->GetValue().ToStdString(), date);
            hasFileBeenModified = true;
            UpdateWindowTitle();
            view->DisplayTasks(model->GetTasks());
        } else {
            wxLogMessage("Impossibile aggiungere task senza titolo!");
        }
    }
}


void Controller::OnModifySelectedTask(wxCommandEvent &event) {
    const std::string oldTitle = model->getTaskByIndex(selectedTaskIndex)->GetTitle();
    const std::string oldDate = model->getTaskByIndex(selectedTaskIndex)->GetDueDate();
    const std::string oldDescr = model->getTaskByIndex(selectedTaskIndex)->GetDescription();

    auto modTaskDialog = wxDialog(view, wxID_ANY, wxT("Modifica Task"));
    auto dialogMainSizer = new wxBoxSizer(wxVERTICAL);

    auto dialogTitleSizer = new wxStaticBoxSizer(wxVERTICAL, &modTaskDialog, wxT("Nuovo titolo"));
    auto dialogDateSizer = new wxStaticBoxSizer(wxVERTICAL, &modTaskDialog, wxT("Nuova Scadenza"));
    auto dialogDescrSizer = new wxStaticBoxSizer(wxVERTICAL, &modTaskDialog, wxT("Nuova descrizione"));

    auto titleCtrl = new wxTextCtrl(&modTaskDialog, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                    wxSize(400, -1));
    titleCtrl->SetHint(oldTitle);
    dialogTitleSizer->Add(titleCtrl, 0, wxEXPAND | wxALL, 5);

    wxDateTime dt;
    dt.ParseISODate(oldDate);
    auto dateCtrl = new wxDatePickerCtrl(&modTaskDialog, wxID_ANY, dt);
    dialogDateSizer->Add(dateCtrl, 0, wxEXPAND | wxALL, 5);

    auto descrCtrl = new wxTextCtrl(&modTaskDialog, wxID_ANY, oldDescr, wxDefaultPosition, wxSize(400, -1),
                                    wxTE_MULTILINE);
    dialogDescrSizer->Add(descrCtrl, 0, wxEXPAND | wxALL, 5);

    auto buttonSizer = modTaskDialog.CreateSeparatedButtonSizer(wxYES | wxCANCEL);

    dialogMainSizer->Add(dialogTitleSizer, 0, wxEXPAND | wxALL, 5);
    dialogMainSizer->Add(dialogDateSizer, 0, wxEXPAND | wxALL, 5);
    dialogMainSizer->Add(dialogDescrSizer, 1, wxEXPAND | wxALL, 5);
    dialogMainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);

    modTaskDialog.SetSizerAndFit(dialogMainSizer);
    modTaskDialog.Layout();

    if (modTaskDialog.ShowModal() == wxID_YES) {
        auto tsk = model->getTaskByIndex(selectedTaskIndex);
        if (!titleCtrl->IsEmpty()) {
            tsk->SetTitle(titleCtrl->GetValue().ToStdString());
        }

        tsk->SetDescription(descrCtrl->GetValue().ToStdString());
        tsk->SetDueDate(dateCtrl->GetValue().FormatISODate().ToStdString());

        view->DisplayTasks(model->GetTasks());
        view->ShowSelectedDetails(selectedTaskIndex, tsk->GetTitle(), tsk->GetDescription(), tsk->GetDueDate());
        hasFileBeenModified = true;
        UpdateWindowTitle();
    }
}


void Controller::OnTaskClick(wxMouseEvent &event) {
    auto *panel = dynamic_cast<TaskPanel *>(event.GetEventObject());
    if (panel) {
        this->selectedTaskIndex = panel->getTaskIndex();
        auto tsk = model->getTaskByIndex(selectedTaskIndex);
        view->ResetPanelColours();
        view->ShowSelectedDetails(selectedTaskIndex, tsk->GetTitle(), tsk->GetDescription(), tsk->GetDueDate());
    }

    event.Skip();
}

void Controller::OnMarkTaskCompleted(wxCommandEvent &event) {
    auto *panel = dynamic_cast<TaskPanel *>(event.GetEventObject());
    if (panel) {
        model->setTaskStatus(panel->isChecked(), panel->getTaskIndex());
        hasFileBeenModified = true;
        UpdateWindowTitle();
    }
}


void Controller::OnDeleteTask(wxCommandEvent &event) {
    auto *panel = dynamic_cast<TaskPanel *>(event.GetEventObject());
    if (panel) {
        // wxLogMessage("Closed Task: %s", model->getTaskByIndex(panel->getTaskIndex())->GetTitle());
        int answer = wxMessageBox("Eliminare task?", "Conferma", wxYES_NO | wxICON_ERROR, view);
        if (answer == wxYES) {
            model->removeTask(panel->getTaskIndex());
            view->DisplayTasks(model->GetTasks());
            hasFileBeenModified = true;
            UpdateWindowTitle();
        }
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

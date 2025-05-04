#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/XMLParser.h"
#include "../Model/Task.h"
#include "../View/MainFrame.h"

class Controller {
public:
    Controller(MainFrame* view, XMLParser* model);
    void BindEvents();

private:
    void OnMenuNew(wxCommandEvent& event);
    void OnMenuOpen(wxCommandEvent& event);
    void OnMenuSave(wxCommandEvent& event);
    void OnMenuSaveAs(wxCommandEvent& event);
    void OnMenuClose(wxCloseEvent& close);
    void OnMenuAbout(wxCommandEvent& event);
    void OnMenuShowCompleted(wxCommandEvent& event);

    void OnFilePicked(wxFileDirPickerEvent& event);

    void OnAddTask(wxCommandEvent& event);

    void OnTaskClick(wxMouseEvent& event);
    void OnMarkTaskCompleted(wxCommandEvent& event);
    void OnDeleteTask(wxCommandEvent& event);
    void OnModifySelectedTask(wxCommandEvent& event);

    bool PromptSaveIfNeeded();
    void UpdateWindowTitle() const;

    MainFrame* view;
    XMLParser* model;

    bool isFileOpen = false;
    bool hasFileBeenModified = false;
    std::string currentFilePath;

};



#endif //CONTROLLER_H

//
// Created by giacomo on 26/09/24.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <iostream>
#include <vector>
#include <utility>
#include <wx/xml/xml.h>

class XMLParser {
public:
    XMLParser() = default;
    explicit XMLParser(const wxString& fileName) {
        tasksFile.Load(fileName);
    }

    bool openFile(const wxString& fileName) {
        if(tasksFile.Load(fileName)) {
            if(tasksFile.GetRoot()->GetName() != "tasklist") {
                std::cout << "Not a tasks file!" << std::endl;
                return false;
            }
            return true;
        }

        return false;
    }

    // TODO modifica in modo tale da leggere anche se i task sono "completati o meno"
    void parseXML() {
        this->taskList.clear();
        auto child = tasksFile.GetRoot()->GetChildren();
        while(child) {
            if(child->GetName() == "task") {
                std::pair<std::string, std::string> tmpTask;
                auto inside = child->GetChildren();
                while(inside) {
                    if(inside->GetName() == "title") {
                        tmpTask.first = inside->GetNodeContent().mb_str();
                    } else if(inside->GetName() == "desc") {
                        tmpTask.second = inside->GetNodeContent().mb_str();
                    }
                    inside = inside->GetNext();
                }
                this->taskList.push_back(tmpTask);
                delete inside;
            }
            child = child->GetNext();
        }

        delete child;
    }

    void serializeXML() {
        auto root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "tasklist");
        this->tasksFile.SetRoot(root);

        // Structured binding disponibile da C++17, figo
        for (const auto&[fst, snd]: taskList) {

            // Radice di ogni task "<task></task>"
            auto taskNode = new wxXmlNode(wxXML_ELEMENT_NODE, "task");

            // <title></title>
            auto taskTitleNode = new wxXmlNode(wxXML_ELEMENT_NODE, "title");
            taskNode->AddChild(taskTitleNode);

            // Testo dentro bracket title
            auto taskTitleText = new wxXmlNode(wxXML_TEXT_NODE, "", wxString(fst));
            taskTitleNode->AddChild(taskTitleText);

            // Analogo per la desc
            auto taskDescNode = new wxXmlNode(wxXML_ELEMENT_NODE, "desc");
            taskNode->AddChild(taskDescNode);

            auto taskDescText = new wxXmlNode(wxXML_TEXT_NODE, "", wxString(snd));
            taskDescNode->AddChild(taskDescText);

            root->AddChild(taskNode);
        }
    }

    bool saveToFile(const wxString& fileName) {
        if(this->tasksFile.Save(fileName)) {
            return true;
        }

        return false;
    }

    void addToTaskList(const std::pair<std::string, std::string>& newTask) {
        this->taskList.push_back(newTask);
    }

    std::vector<std::pair<std::string, std::string>> getTaskList() const {
        return taskList;
    }

    void setTaskList(const std::vector<std::pair<std::string, std::string>> &task_list) {
        taskList = task_list;
    }

private:
    wxXmlDocument tasksFile;
    std::vector<std::pair<std::string, std::string>> taskList;
};

#endif //XMLPARSER_H

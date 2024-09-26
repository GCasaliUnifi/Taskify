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

    void parseXML() {
        auto child = tasksFile.GetRoot()->GetChildren();
        while(child) {
            if(child->GetName() == "task") {
                std::pair<std::string, std::string> tmpTask;
                auto inside = child->GetChildren();
                while(inside) {
                    if(inside->GetName() == "title") {
                        std::cout << inside->GetNodeContent() << std::endl;
                        tmpTask.first = inside->GetNodeContent().mb_str();
                    } else if(inside->GetName() == "desc") {
                        std::cout << inside->GetNodeContent() << std::endl;
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

    bool saveToFile(const wxString& fileName) {
        return true;
    }

    std::vector<std::pair<std::string, std::string>> getTaskList() const {
        return taskList;
    }

private:
    wxXmlDocument tasksFile;
    std::vector<std::pair<std::string, std::string>> taskList;
};

#endif //XMLPARSER_H

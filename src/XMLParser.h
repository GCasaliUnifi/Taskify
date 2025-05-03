//
// Created by giacomo on 26/09/24.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <vector>
#include <utility>
#include <wx/xml/xml.h>


class XMLParser {
public:
    XMLParser() = default;
    explicit XMLParser(const wxString& fileName);
    bool openFile(const wxString& fileName);
    void parseXML();
    void serializeXML();
    bool saveToFile(const wxString& fileName) const;
    void addToTaskList(const std::tuple<std::string, std::string, bool>& newTask);
    std::vector<std::tuple<std::string, std::string, bool>> getTaskList() const;
    void setTaskList(const std::vector<std::tuple<std::string, std::string, bool>> &task_list);

private:
    wxXmlDocument tasksFile;
    std::vector<std::tuple<std::string, std::string, bool>> taskList;
};

#endif //XMLPARSER_H

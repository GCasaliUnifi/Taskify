#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <vector>
#include <utility>
#include <wx/xml/xml.h>

#include "Task.h"

class XMLParser {
public:
    XMLParser() = default;
    explicit XMLParser(const wxString& fileName);

    void addTask(const std::string& title, const std::string& descr);
    void removeTask(int index);
    void clearTasks();
    Task* getTaskByIndex(const int index) const;

    void setTaskStatus(bool isCompleted, int index);

    bool loadFromFile(const std::string& filePath);
    bool saveToFile(const std::string& filePath);

    void parseXML();
    void serializeXML();

    const std::vector<std::unique_ptr<Task>>& GetTasks() const;

private:
    wxXmlDocument tasksFile;
    std::vector<std::unique_ptr<Task>> taskList;
};

#endif //XMLPARSER_H

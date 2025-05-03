#include "XMLParser.h"

XMLParser::XMLParser(const wxString &fileName) {
    tasksFile.Load(fileName);
}

bool XMLParser::openFile(const wxString &fileName) {
    if (tasksFile.Load(fileName)) {
        if (tasksFile.GetRoot()->GetName() != "tasklist") {
            return false;
        }
        return true;
    }

    return false;
}

void XMLParser::parseXML() {
    this->taskList.clear();
    // Prendi il primo figlio
    auto child = tasksFile.GetRoot()->GetChildren();
    while (child) {
        // loop per tutti i figli della radice
        if (child->GetName() == "task") {
            std::tuple<std::string, std::string, bool> tmpTask;
            bool taskCompleted = false;

            auto attr = child->GetAttributes(); // Ritorna un puntatore al primo attributo del nodo
            if (attr && attr->GetName() == "completed") {
                taskCompleted = (attr->GetValue() == "true");
            }

            std::get<2>(tmpTask) = taskCompleted;

            auto inside = child->GetChildren();
            while (inside) {
                if (inside->GetName() == "title") {
                    std::get<0>(tmpTask) = std::string(inside->GetNodeContent().ToUTF8().data());
                } else if (inside->GetName() == "desc") {
                    std::get<1>(tmpTask) = std::string(inside->GetNodeContent().ToUTF8().data());
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

void XMLParser::serializeXML() {
    auto root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "tasklist");
    this->tasksFile.SetRoot(root);

    // Structured binding disponibile da C++17
    for (const auto &[fst, snd, completed]: taskList) {
        // Radice di ogni task "<task completed="true/false"></task>"
        auto taskNode = new wxXmlNode(wxXML_ELEMENT_NODE, "task");
        taskNode->AddAttribute("completed", completed ? "true" : "false");

        // <title></title>
        auto taskTitleNode = new wxXmlNode(wxXML_ELEMENT_NODE, "title");
        taskNode->AddChild(taskTitleNode);

        // Testo dentro bracket title
        auto taskTitleText = new wxXmlNode(wxXML_TEXT_NODE, "", wxString(fst.c_str(), wxConvUTF8));
        taskTitleNode->AddChild(taskTitleText);

        // Analogo per la desc
        auto taskDescNode = new wxXmlNode(wxXML_ELEMENT_NODE, "desc");
        taskNode->AddChild(taskDescNode);

        auto taskDescText = new wxXmlNode(wxXML_TEXT_NODE, "", wxString(snd.c_str(), wxConvUTF8));
        taskDescNode->AddChild(taskDescText);

        root->AddChild(taskNode);
    }
}

bool XMLParser::saveToFile(const wxString &fileName) const {
    if (this->tasksFile.Save(fileName)) {
        return true;
    }

    return false;
}

void XMLParser::addToTaskList(const std::tuple<std::string, std::string, bool> &newTask) {
    this->taskList.push_back(newTask);
}

std::vector<std::tuple<std::string, std::string, bool> > XMLParser::getTaskList() const {
    return taskList;
}

void XMLParser::setTaskList(const std::vector<std::tuple<std::string, std::string, bool> > &task_list) {
    taskList = task_list;
}

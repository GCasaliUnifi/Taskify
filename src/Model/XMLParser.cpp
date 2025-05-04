#include "XMLParser.h"

XMLParser::XMLParser(const wxString &fileName) {
    tasksFile.Load(fileName);
}

void XMLParser::addTask(const std::string& title, const std::string& descr) {
    taskList.emplace_back(std::make_unique<Task>(title,descr));
}

void XMLParser::clearTasks() {
    this->taskList.clear();
}

Task* XMLParser::getTaskByIndex(const int index) const {
    return this->taskList[index].get();
}


bool XMLParser::loadFromFile(const std::string &filePath) {
    if (tasksFile.Load(filePath)) {
        if (tasksFile.GetRoot()->GetName() != "tasklist") {
            // Messaggio qui xml non valido
            return false;
        }

        parseXML();
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
            std::string title;
            std::string description;
            bool taskCompleted = false;

            auto attr = child->GetAttributes(); // Ritorna un puntatore al primo attributo del nodo
            if (attr && attr->GetName() == "completed") {
                taskCompleted = (attr->GetValue() == "true");
            }

            auto inside = child->GetChildren();
            while (inside) {
                if (inside->GetName() == "title") {
                    title = std::string(inside->GetNodeContent().ToUTF8().data());
                } else if (inside->GetName() == "desc") {
                    description = std::string(inside->GetNodeContent().ToUTF8().data());
                }
                inside = inside->GetNext();
            }
            auto tmp = new Task(title, description, taskCompleted);
            this->taskList.push_back(std::make_unique<Task>(*tmp));
            delete inside;
        }
        child = child->GetNext();
    }

    delete child;
}

/* void XMLParser::serializeXML() {
    auto root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "tasklist");
    this->tasksFile.SetRoot(root);

    // Structured binding disponibile da C++17
    for (const auto &[fst, snd, completed]: oldTaskList) {
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
} */

bool XMLParser::saveToFile(const std::string &filePath) {

    return false;
}

// bool XMLParser::saveToFile(const wxString &fileName) const {
//     if (this->tasksFile.Save(fileName)) {
//         return true;
//     }
//
//     return false;
// }

const std::vector<std::unique_ptr<Task>>& XMLParser::GetTasks() const {
    return this->taskList;
}

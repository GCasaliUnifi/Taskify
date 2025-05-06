#include "XMLParser.h"

XMLParser::XMLParser(const wxString &fileName) {
    tasksFile.Load(fileName);
}

void XMLParser::addTask(const std::string& title, const std::string& descr, const std::string &dueDate) {
    taskList.emplace_back(std::make_unique<Task>(title,descr, dueDate));
}

void XMLParser::removeTask(int index) {
    this->taskList.erase(taskList.begin() + index);
}

void XMLParser::clearTasks() {
    this->taskList.clear();
}

Task* XMLParser::getTaskByIndex(int index) const {
    return this->taskList[index].get();
}

std::vector<Task *> XMLParser::getCompletedTasks() {
    std::vector<Task*> completedTasks;

    for (const auto& task : taskList) {
        if (task->IsCompleted()) {
            completedTasks.push_back(task.get());
        }
    }

    return completedTasks;
}

std::vector<Task *> XMLParser::findTasksByRegex(const std::regex &pattern) {
    std::vector<Task*> matchingTasks;

    for (const auto& task : taskList) {
        if (std::regex_search(task->GetTitle(), pattern)) {
            matchingTasks.push_back(task.get());
        }
    }

    return matchingTasks;
}

void XMLParser::setTaskStatus(bool isCompleted, int index) {
    this->taskList[index].get()->SetCompleted(isCompleted);
}

void XMLParser::setTaskDate(const wxDateTime &newDate, int index) {
    this->taskList[index].get()->SetDueDate(newDate.FormatISODate().ToStdString());
}

void XMLParser::orderTasksByStatus() {
    std::sort(taskList.begin(), taskList.end(), [](const auto& a, const auto& b) {
    return a->IsCompleted() < b->IsCompleted();
    });
}

void XMLParser::orderTasksByDate() {
    std::sort(taskList.begin(), taskList.end(), [](const auto& a, const auto& b) {
        return a->GetDueDate() < b->GetDueDate();
    });
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
            std::string dueDateStr;
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
                } else if (inside->GetName() == "datedue") {
                    dueDateStr = std::string(inside->GetNodeContent().ToUTF8().data());
                }
                inside = inside->GetNext();
            }

            auto tmp = new Task(title, description, dueDateStr, taskCompleted);
            this->taskList.push_back(std::make_unique<Task>(*tmp));
            delete inside;
        }
        child = child->GetNext();
    }

    delete child;
}

void XMLParser::serializeXML() {
    auto root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, "tasklist");
    this->tasksFile.SetRoot(root);

    for (int i = 0; i < taskList.size(); ++i) {
        // Radice di ogni task "<task completed="true/false"></task>"
        auto taskNode = new wxXmlNode(wxXML_ELEMENT_NODE, "task");
        taskNode->AddAttribute("completed", taskList[i].get()->IsCompleted() ? "true" : "false");

        // <title></title>
        auto taskTitleNode = new wxXmlNode(wxXML_ELEMENT_NODE, "title");
        taskNode->AddChild(taskTitleNode);

        // Testo dentro bracket title
        auto taskTitleText = new wxXmlNode(wxXML_TEXT_NODE, "", wxString(taskList[i].get()->GetTitle()));
        taskTitleNode->AddChild(taskTitleText);

        // Analogo per la desc
        auto taskDescNode = new wxXmlNode(wxXML_ELEMENT_NODE, "desc");
        taskNode->AddChild(taskDescNode);

        auto taskDescText = new wxXmlNode(wxXML_TEXT_NODE, "", wxString(taskList[i].get()->GetDescription()));
        taskDescNode->AddChild(taskDescText);

        // data scadenza
        auto taskDateNode = new wxXmlNode(wxXML_ELEMENT_NODE, "datedue");
        taskNode->AddChild(taskDateNode);

        auto taskDateText = new wxXmlNode(wxXML_TEXT_NODE, "", taskList[i].get()->GetDueDate()); // YYYY-MM-DD
        taskDateNode->AddChild(taskDateText);

        root->AddChild(taskNode);
    }
}

bool XMLParser::saveToFile(const std::string &filePath) {

    serializeXML();
    if (tasksFile.Save(filePath)) {
        return true;
    }

    return false;
}

const std::vector<std::unique_ptr<Task>>& XMLParser::GetTasks() const {
    return this->taskList;
}

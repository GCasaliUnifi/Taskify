#include "../Task.h"

Task::Task(const std::string &title, const std::string &description,
           const std::chrono::system_clock::time_point &dueDate, bool completed) {

    this->title = title;
    this->description = description;
    this->dueDate = dueDate;
    this->isCompleted = completed;
}

const std::string & Task::GetTitle() const {
    return this->title;
}

const std::string & Task::GetDescription() const {
    return this->description;
}

std::chrono::system_clock::time_point Task::GetDueDate() const {
    return this->dueDate;
}

bool Task::IsCompleted() const {
    return this->isCompleted;
}

void Task::SetTitle(const std::string &title) {
    this->title = title;
}

void Task::SetDescription(const std::string &description) {
    this->description = description;
}

void Task::SetCompleted(bool completed) {
    this->isCompleted = completed;
}

void Task::SetDueDate(const std::chrono::system_clock::time_point &date) {
    this->dueDate = date;
}

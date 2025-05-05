#ifndef TASK_H
#define TASK_H

#include <iostream>

class Task {
public:
    Task() = default;
    Task(const std::string& title, const std::string& description, const std::string& dueDate, bool completed = false );

    const std::string& GetTitle() const;
    const std::string& GetDescription() const;
    const std::string& GetDueDate() const;
    bool IsCompleted() const;

    void SetTitle(const std::string& title);
    void SetDescription(const std::string& description);
    void SetCompleted(bool completed);
    void SetDueDate(const std::string& date);

private:
    std::string title;
    std::string description;
    bool isCompleted = false;

    std::string dueDate; // "YYYY-MM-DD"

};

#endif //TASK_H
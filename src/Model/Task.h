#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <chrono>

class Task {
public:
    Task();
    Task(const std::string& title, const std::string& description, bool completed = false );

    const std::string& GetTitle() const;
    const std::string& GetDescription() const;
    std::chrono::system_clock::time_point GetDueDate() const;
    bool IsCompleted() const;

    void SetTitle(const std::string& title);
    void SetDescription(const std::string& description);
    void SetCompleted(bool completed);
    void SetDueDate(const std::chrono::system_clock::time_point& date);

private:
    std::string title;
    std::string description;
    bool isCompleted = false;

    std::chrono::system_clock::time_point dueDate;

};

#endif //TASK_H
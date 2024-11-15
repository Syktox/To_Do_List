module;

#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>

export module Task;

import std;


export class Task {
private: 
    std::string name;
    std::string description;
    bool completed;
    std::time_t created;   
    std::time_t completedAt;

public:
    Task(std::string name, std::string description);
    std::string getName();
    std::string getDescription();
    bool isCompleted();
    void setCompleted(bool completed);
    std::time_t getCreated();
    std::time_t getCompletedAt();
    void setCompletedAt(std::time_t completedAt);
    void print();
};

Task::Task(std::string nameArg, std::string descriptionArg) {
    this->name = nameArg;
    this->description = descriptionArg;
    this->completed = false;
    this->created = std::time(0);
    this->completedAt = 0;
}

std::string Task::getName() {
    return this->name;
}

std::string Task::getDescription() {
    return this->description;
}

bool Task::isCompleted() {
    return this->completed;
}

void Task::setCompleted(bool completedArg) {
    this->completed = completedArg;
}

std::time_t Task::getCreated() {
    return this->created;
}

std::time_t  Task::getCompletedAt() {
    return this->completedAt;
}

void Task::setCompletedAt(std::time_t completedAtArg) {
    this->completedAt = completedAtArg;
}

void Task::print() {
    char str[26];
    std::cout << "Task: " << this->name << std::endl;
    std::cout << "Description: " << this->description << std::endl;
    std::cout << "Created: " << ctime_s(str, sizeof(str), &this->created);
    if (this->completed) {
        std::cout << "Completed: " << ctime_s(str, sizeof(str), &this->completedAt);
    }
}




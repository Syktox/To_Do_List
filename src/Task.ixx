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

Task::Task(std::string name, std::string description) {
    this->name = name;
    this->description = description;
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

void Task::setCompleted(bool completed) {
    this->completed = completed;
}

std::time_t Task::getCreated() {
    return this->created;
}

std::time_t  Task::getCompletedAt() {
    return this->completedAt;
}

void Task::setCompletedAt(std::time_t completedAt) {
    this->completedAt = completedAt;
}

void Task::print() {
    std::cout << "Task: " << this->name << std::endl;
    std::cout << "Description: " << this->description << std::endl;
    std::cout << "Created: " << std::ctime(&this->created);
    if (this->completed) {
        std::cout << "Completed: " << std::ctime(&this->completedAt);
    }
}




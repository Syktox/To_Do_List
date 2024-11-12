#include "Task.h"
#include <iostream>
#include <ctime>

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




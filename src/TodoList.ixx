//
// Created by Markus Kammerstetter on 12.11.24.
//

export module TodoList;

import Task;
import std;

export class TodoList {
private:
    std::vector<Task> tasks;

public:
    void addTask(Task task);
    void printAllTasks();
    void deleteTask(std::string name_of_task);
};

void TodoList::addTask(Task task)
{
    tasks.push_back(task);
}

void TodoList::printAllTasks()
{    
    for (auto task : tasks) {
        task.print();
    }
}

void TodoList::deleteTask(std::string name_of_task)
{
    std::erase_if(tasks, [&name_of_task](Task& task)
    {
        return task.getName() == name_of_task;
    });
}


#include <iostream>
#include <nlohmann/json.hpp>
#include <wx/wx.h>

import Task;
import TodoList;

int main(int argc, char *argv[]) {
    TodoList tl;
    
    Task task("Task 1", "Description of Task 1");
    Task task2("Task 2", "Description of Task 2");
    Task task3("Task 3", "Description of Task 3");
    tl.addTask(task);
    tl.addTask(task2);
    tl.addTask(task3);
    
    Task currTask = tl.finishTask(task2);
    tl.printAllTasks();
    std::cout << "////////////\n";
    time_t time = currTask.getCompletedAt();
    
    std::cout << "Completed: " << std::ctime(&time);
    
    
    return 0;
}

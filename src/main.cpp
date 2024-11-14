#include <iostream>
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

    tl.printAllTasks();

    tl.deleteTask("Task 1");

    std::cout << "__---___----__-----------" << std::endl;
    tl.printAllTasks();
    
    
    return 0;
}

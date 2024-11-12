#include <iostream>
#include "../lib/mylib.h"
#include "../lib/Task.h"

int main(int argc, char *argv[]) {
    
    Task task("Task 1", "Description of Task 1");
    task.print();

    return 0;
}

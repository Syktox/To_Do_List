#ifndef TASK_H
#define TASK_H

#include <string>
#include <ctime> 

class Task {
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

#endif // TASK_H
// mlfq scheduler - process control block (pcb)

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <iostream>
// building an skmple process block so that eveything can be tracked
class Process {
private:
    std::string pid;         // unique id
    int arrivalTime;         // when arrived
    int burstTime;           // total cpu needed
    int remTime;             // cpu time left
    int currPriority;        // queue level
    bool hasStarted;   
    bool hasIO;              // does it do I/O?
    int firstRunTime;        // first cpu time
    int waitTime;            // time spent waiting
    int turnaroundTime;      // arrival to completion
    int resTime;             // response time

public:
    Process(std::string pid, int arrivalTime, int burstTime, bool io = false){
        this->pid = pid;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime; 
        this->remTime = burstTime;
        this->currPriority = 0;
        this->hasStarted = false;
        this->hasIO = io;
        this->firstRunTime = 0;
        this->waitTime = 0;
        this->turnaroundTime = 0;
        this->resTime = 0;
    }

    // check if done
    bool isComplete() const {
        return remTime == 0;
    }    

    // for debugging 
    void display() const {
        std::cout << "Process " << pid << " | Arrival: " << arrivalTime << " | Burst: " << burstTime << " | Remaining: " << remTime << " | Queue: Q" << currPriority << std::endl;
    }

    // getters
    std::string getPid() const { return pid; }
    int getArrivalTime() const { return arrivalTime; }
    int getBurstTime() const { return burstTime; }
    int getRemainingTime() const { return remTime; }
    int getPriority() const { return currPriority; }
    bool getHasStarted() const { return hasStarted; }
    bool doesIO() const { return hasIO; }
    int getWaitTime() const { return waitTime; }
    int getTurnaroundTime() const { return turnaroundTime; }
    int getResponseTime() const { return resTime; }

    // reduce remaining time
    void executeFor(int time) {
        if (time > 0 && time <= remTime) {
            remTime -= time;
        }
    }

    // mark first run, calc response time
    void markStarted(int currentTime) {
        if (!hasStarted) {
            hasStarted = true;
            firstRunTime = currentTime;
            resTime = currentTime - arrivalTime;
        }
    }

    // move to lower queue
    void demote() {
        if (currPriority < 7) {
            currPriority++;
        }
    }
    void boostPriority() {
        currPriority = 0;
    }

    // set queue level
    void setPriority(int priority) {
        if (priority >= 0 && priority <= 7) {
            currPriority = priority;
        }
    }
    void addWaitTime(int time) {
        if (time > 0) {
            waitTime += time;
        }
    }
    void calculateTurnaroundTime(int completionTime) {
        turnaroundTime = completionTime - arrivalTime;
    }

};

#endif

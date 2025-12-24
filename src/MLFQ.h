// mlfq scheduler - main class

#ifndef MLFQ_H
#define MLFQ_H

#include <iostream>
#include <deque>
#include <vector>
#include "Process.h"

class MLFQ {
private:
    std::deque<Process*> queues[8];
    //storing the quantoms
    int quantum[8];
    int currentTime;

public:
    MLFQ() {
        currentTime = 0;
        int q = 4;
        for(int i=0; i<8; i++){
            quantum[i] = q;
            q *= 2; 
        }
    }
    // add process to first queue
    void addProcess(Process* p) {
        if (p->getRemainingTime() > 0) {
            queues[0].push_back(p);
        }
    }
    // DEBUG
    void debugShowQueues() const {
        std::cout << "--- queue status ---" << std::endl;
        for(int i=0; i<8; i++) {
            if(!queues[i].empty()) {
                std::cout << "Q" << i << " (q=" << quantum[i] << "ms): ";
                for(Process* p : queues[i]) {
                    std::cout << "[" << p->getPid() << "] ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << "--------------------" << std::endl;
    }
    // retrive highest priority 
    Process* getNextProcess() {
        for(int i=0; i<8; i++) {
            if(!queues[i].empty()) {
                Process* p = queues[i].front();
                return p;
            }
        }
        return nullptr;
    }
    void runSimulation() {
        std::cout << "start simulation..." << std::endl;
        
        while(true) {
            Process* p = getNextProcess();
            
            if (p == nullptr) {
                std::cout << "all processes complete!" << std::endl;
                break;
            }
            
            // get quantum for this queue
            int currentQ = p->getPriority();
            int timeSlice = quantum[currentQ];
            
            // logic: does process finish in this quantum?
            int timeNeeded = p->getRemainingTime();
            
            if(timeNeeded <= timeSlice) {
                currentTime += timeNeeded;
                p->executeFor(timeNeeded);
                p->calculateTurnaroundTime(currentTime);
                // DEBUG
                std::cout << "[time " << currentTime << "] process " << p->getPid() << " finishes" << std::endl;
                queues[currentQ].pop_front();
            } 
            else {
                // if Process Uses Full Quantum
                currentTime += timeSlice;
                p->executeFor(timeSlice);
                std::cout << "[time " << currentTime << "] process " << p->getPid() << " runs for " << timeSlice << "ms (rem: " << p->getRemainingTime() << ")" << std::endl;
                
                // DEMOTE
                queues[currentQ].pop_front();
                
                // demote and push to end of next queue
                p->demote();
                int nextQ = p->getPriority();
                queues[nextQ].push_back(p);
                
                std::cout << "demoted to Q" << nextQ << std::endl;
            }
        }
    }
};

#endif 

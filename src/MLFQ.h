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
    std::vector<Process*> processes;
    std::vector<Process*> futureProcesses; // waiting to arrive
    // storing the quantoms
    int quantum[8];
    int currentTime;
    int boostInterval;
    int lastBoostTime;

public:
    MLFQ() {
        currentTime = 0;
        boostInterval = 200; //every 200ms
        lastBoostTime = 0;
        int q = 4;
        for(int i=0; i<8; i++){
            quantum[i] = q;
            q *= 2; 
        }
    }
    // add process handling arrival time
    void addProcess(Process* p) {
        processes.push_back(p); // track globally
        if (p->getArrivalTime() <= currentTime) {
            queues[0].push_back(p);
        } else {
            futureProcesses.push_back(p);
        }
    }
    
    // check for new arrivals
    void checkArrivals() {
        for(auto it = futureProcesses.begin(); it != futureProcesses.end(); ) {
            Process* p = *it;
            if(p->getArrivalTime() <= currentTime) {
                queues[0].push_back(p);
                // std::cout << "[time " << currentTime << "] process " << p->getPid() << " arrived" << std::endl;
                it = futureProcesses.erase(it);
            } else {
                ++it;
            }
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
    // find next process (Rule 1)
    Process* getNextProcess() {
        for(int i=0; i<8; i++) {
            if(!queues[i].empty()) {
                Process* p = queues[i].front();
                return p;
            }
        }
        return nullptr;
    }

    // move everyone to q0 (Rule 6)
    void boostAllProcesses() {
        std::cout << "[time " << currentTime << "] BOOST! moving all processes to Q0" << std::endl;
        for(int i=1; i<8; i++) {
            while(!queues[i].empty()) {
                Process* p = queues[i].front();
                queues[i].pop_front();
                
                // reset process priority
                p->boostPriority();
                queues[0].push_back(p);
            }
        }
    }

    // run simulation
    void runSimulation() {
        std::cout << "start simulation..." << std::endl;
        
        while(true) {
            checkArrivals();

            if (currentTime >= lastBoostTime + boostInterval) {
                boostAllProcesses();
                lastBoostTime = currentTime;
            }

            Process* p = getNextProcess();
            
            if (p == nullptr) {
                if (futureProcesses.empty()) {
                    std::cout << "all processes complete!" << std::endl;
                    break;
                } else {
                    currentTime++;
                    continue; 
                }
            }
            
            p->markStarted(currentTime);

            int currentQ = p->getPriority();
            int timeSlice = quantum[currentQ];
            int timeNeeded = p->getRemainingTime();
            
            if(timeNeeded <= timeSlice) {
                // CASE 1: Process Finishes
                currentTime += timeNeeded;
                p->executeFor(timeNeeded);
                p->calculateTurnaroundTime(currentTime);
                std::cout << "[time " << currentTime << "] process " << p->getPid() << " finishes" << std::endl;
                queues[currentQ].pop_front();
            } 
            else if (p->doesIO()) {
                // CASE 2: I/O (Rule 5)
                int ioRunTime = 2;
                currentTime += ioRunTime;
                p->executeFor(ioRunTime);
                std::cout << "[time " << currentTime << "] process " << p->getPid() << " does I/O (rem: " << p->getRemainingTime() << ")" << std::endl;
                
                // keep priority
                queues[currentQ].pop_front();
                queues[currentQ].push_back(p);
                std::cout << "keeps priority Q" << currentQ << std::endl;
            }
            else {
                // CASE 3: Full Quantum (Rule 4)
                currentTime += timeSlice;
                p->executeFor(timeSlice);
                std::cout << "[time " << currentTime << "] process " << p->getPid() << " runs for " << timeSlice << "ms (rem: " << p->getRemainingTime() << ")" << std::endl;
                
                // demote
                queues[currentQ].pop_front();
                p->demote();
                int nextQ = p->getPriority();
                queues[nextQ].push_back(p);
                std::cout << "demoted to Q" << nextQ << std::endl;
            }
        }
        
        printMetrics(); // automatic report
    }

    // print stats
    void printMetrics() {
        std::cout << "\n=== Final Metrics ===" << std::endl;
        std::cout << "PID\tTurnaround\tWaiting\t\tResponse" << std::endl;
        
        double totalTurnaround = 0;
        double totalWait = 0;
        double totalResponse = 0;
        
        for(Process* p : processes) {
            int ta = p->getTurnaroundTime();
            // wait time = turnaround - burst
            int wt = ta - p->getBurstTime(); 
            // set it (since we didn't track it tick-by-tick)
            // p->setWaitTime(wt); // assumes we had a setter, but we can just calc here
            
            // wait we have waitTime in process but didn't update it tick-by-tick
            // calculation is easier:
            
            int resp = p->getResponseTime();
            
            std::cout << p->getPid() << "\t" << ta << "ms\t\t" << wt << "ms\t\t" << resp << "ms" << std::endl;
            
            totalTurnaround += ta;
            totalWait += wt;
            totalResponse += resp;
        }
        
        int n = processes.size();
        std::cout << "---------------------------------------------" << std::endl;
        if(n > 0) {
            std::cout << "Avg Turnaround: " << totalTurnaround / n << " ms" << std::endl;
            std::cout << "Avg Waiting:    " << totalWait / n << " ms" << std::endl;
            std::cout << "Avg Response:   " << totalResponse / n << " ms" << std::endl;
        }
        std::cout << "=====================" << std::endl;
    }
};

#endif 


#include <iostream>
#include "Process.h"
#include "MLFQ.h"

int main() {
    std::cout << "=== MLFQ Scheduler ===" << std::endl;
    
    // Test MLFQ creation
    std::cout << "Creating Scheduler..." << std::endl;
    MLFQ scheduler; // Constructor is public now!
    
    std::cout << "Scheduler created successfully!" << std::endl;
    std::cout << std::endl;

    // Create processes
    Process p1("P1", 0, 100);
    Process p2("P2", 5, 50);
    Process p3("P3", 10, 200);
    Process p4("Interactive", 15, 50, true);

    // Add to scheduler
    std::cout << "Adding processes to scheduler..." << std::endl;
    scheduler.addProcess(&p1);
    scheduler.addProcess(&p2);
    scheduler.addProcess(&p3);
    scheduler.addProcess(&p4);

    // Show queues
    scheduler.debugShowQueues();

    // Test scheduler logic
    scheduler.runSimulation();
    
    return 0;
}

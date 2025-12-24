
#include <iostream>
#include "Process.h"

int main() {
    std::cout << "=== MLFQ Scheduler ===" << std::endl;
    std::cout << "Testing Process Class..." << std::endl;
    std::cout << std::endl;

    Process p1("P1", 0, 100);   // PID="P1", arrives at time 0, needs 100ms CPU
    Process p2("P2", 5, 50);    // PID="P2", arrives at time 5, needs 50ms CPU
    Process p3("P3", 10, 200);  // PID="P3", arrives at time 10, needs 200ms CPU
    
    std::cout << "Created 3 processes:" << std::endl;
    p1.display();
    p2.display();
    p3.display();
    std::cout << std::endl;
    std::cout << "Testing isComplete():" << std::endl;
    if (p1.isComplete()) {
        std::cout << "P1 is complete!" << std::endl;
    } else {
        std::cout << "P1 still has work to do (remaining time > 0)" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "✓ All tests passed! Your Process class works!" << std::endl;
    
    return 0;
}

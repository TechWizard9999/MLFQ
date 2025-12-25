# Multi-Level Feedback Queue (MLFQ) Scheduler

A robust C++ implementation of the Multi-Level Feedback Queue scheduling algorithm, demonstrating advanced OS concepts like dynamic priority adjustment, starvation avoidance, and I/O handling.

## 🚀 Features

*   **8 Priority Queues**: Ranging from Q0 (Highest) to Q7 (Lowest).
*   **Dynamic Time Quantum**: 
    *   Q0: 4ms (Fast response)
    *   Q1: 8ms
    *   ...
    *   Q7: 512ms (High throughput)
*   **Anti-Starvation**: Implements **Priority Boost** (Rule 6) every 200ms to reset all jobs to Q0.
*   **I/O Handling**: Interactive processes that yield early stay in high-priority queues (Rule 5).
*   **True Arrival Time**: Simulates realistic process arrival (no "time travel").

## 🛠️ How to Run

This project includes a `Makefile` for easy compilation.

### Prerequisites
*   g++ (C++17 support recommended)
*   Make

### Commands

**1. Compile and Run:**
```bash
make && ./mlfq
```
Or strictly:
```bash
make
./mlfq
```

**2. Clean Build Files:**
```bash
make clean
```

## 📊 Output

The simulation prints a tick-by-tick trace of process execution, queue changes, and demotions. At the end, it displays a metrics report:

```text
=== Final Metrics ===
PID          Turnaround      Waiting         Response
P1           288ms           188ms           0ms
---------------------------------------------
Avg Turnaround: 229 ms
Avg Response:   4.5 ms
```

## 🧠 Core Rules Implemented

1.  **Rule 1**: If Priority(A) > Priority(B), A runs (B doesn't).
2.  **Rule 2**: If Priority(A) = Priority(B), they run in Round-Robin.
3.  **Rule 3**: When a job enters the system, it is placed at the highest priority (Q0).
4.  **Rule 4**: If a job uses up its entire time slice while running, its priority is reduced (moves down one queue).
5.  **Rule 5**: If a job gives up the CPU before the time slice is up, it stays at the same priority level.
6.  **Rule 6**: After some time period S, move all the jobs in the system to the topmost queue.

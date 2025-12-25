# 🚧 Challenges & Solutions

Developing this MLFQ Scheduler involved adhering to strict OS principles. Here are the major challenges we encounted and how we solved them.

## 1. The "Time Travel" Paradox (Negative Response Time) 🕰️

**Problem:** 
Initially, we observed negative `ResponseTime` in our metrics (e.g., `-50ms`).
This happened because processes created with `ArrivalTime = 50` were being added to the ready queue at `Time = 0`. The scheduler ran them immediately, effectively simulating execution *before* arrival.

**Solution:**
We implemented a **Waiting Room** (`futureProcesses` vector).
*   **Logic:** `if (process.arrival > currentTime) -> addToWaitingRoom()`
*   **Mechanism:** A `checkArrivals()` function runs at the start of every simulation tick. It only moves processes from the Waiting Room to the Ready Queue when `currentTime >= arrivalTime`.

## 2. Low-Priority Starvation 💀

**Problem:** 
With many CPU-intensive jobs (P1, P2, P3) filling up the lower queues, newly arrived processes or interactive jobs could potentially get stuck or delayed significantly.

**Solution:**
We implemented **Priority Boost (Rule 6)**.
*   **Mechanism:** Every `200ms`, a global reset triggers (`boostAllProcesses()`).
*   **Effect:** All processes, regardless of their current queue, are moved back to **Queue 0**. This gives CPU-bound jobs a fresh start and ensures no job is forgotten in the basement queues forever.

## 3. The "Gaming the System" Loop 🎮

**Problem:** 
How do we distinguish between a job that *needs* the CPU (Batch) and a job that is *interactive* (waiting for User)?
If we demote everyone who runs, interactive jobs (which run frequently but briefly) would unfairly sink to low priority.

**Solution:**
We implemented **Rule 5 (I/O Handling)**.
*   **Logic:** If `process.doesIO()` is true, it yields the CPU *before* its time quantum expires (e.g., runs for 2ms out of 4ms).
*   **Result:** The scheduler does **NOT** demote it. It stays in Q0, ensuring it remains responsive for the next input event.

## 4. Accurate Time Tracking ⏱️

**Problem:** 
Calculating `Waiting Time` tick-by-tick is computationally expensive and error-prone.

**Solution:**
We used derived math at the end.
*   We tracked: `Arrival Time`, `Burst Time`, and `Turnaround Time (Completion - Arrival)`.
*   Formula: `Waiting Time = Turnaround Time - Burst Time`.
*   This simplified the simulation logic while guaranteeing mathematical accuracy.

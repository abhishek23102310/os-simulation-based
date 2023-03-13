#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Process {
    int arrival_time;
    int cpu_burst_time;
    int remaining_time;
    int wait_time;
};

void run_round_robin(vector<Process>& processes, int time_slice) {
    queue<Process> ready_queue;
    int current_time = 0;
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    int num_completed_processes = 0;

    while (num_completed_processes < processes.size()) {
        // Add arriving processes to the ready queue
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                ready_queue.push(processes[i]);
            }
        }

        // Process the next process in the ready queue
        if (!ready_queue.empty()) {
            Process current_process = ready_queue.front();
            ready_queue.pop();

            // Determine how much time the process will run for
            int time_remaining = current_process.remaining_time;
            if (time_remaining > time_slice) {
                time_remaining = time_slice;
            }

            // Update the wait time for all processes in the ready queue
            for (int i = 0; i < ready_queue.size(); i++) {
                Process& p = ready_queue.front();
                p.wait_time += time_remaining;
                ready_queue.pop();
                ready_queue.push(p);
            }

            // Update the current process
            current_process.remaining_time -= time_remaining;
            current_time += time_remaining;

            // Check if the process is complete
            if (current_process.remaining_time == 0) {
                num_completed_processes++;
                total_wait_time += current_process.wait_time;
                total_turnaround_time += current_time - current_process.arrival_time;
            } else {
                ready_queue.push(current_process);
            }
        } else {
            current_time++;
        }
    }

    // Calculate and output the average wait and turnaround times
    double avg_wait_time = (double) total_wait_time / processes.size();
    double avg_turnaround_time = (double) total_turnaround_time / processes.size();
    cout << "Average wait time: " << avg_wait_time << endl;
    cout << "Average turnaround time: " << avg_turnaround_time << endl;
}

int main() {
    // Generate a set of random processes
    vector<Process> processes;
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        Process p;
        p.arrival_time = rand() % 100;
        p.cpu_burst_time = rand() % 10 + 1;
        p.remaining_time = p.cpu_burst_time;
        p.wait_time = 0;
        processes.push_back(p);
    }

    // Run the Round Robin scheduling algorithm with a time slice of 2
    run_round_robin(processes, 2);

    return 0;
}

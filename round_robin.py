from collections import deque
from process import Process
from tabulate import tabulate

class RoundRobin:
    """
    The round-robin (RR) scheduling algorithm is similar to FCFS scheduling, but
    preemption is added to enable the system to switch between processes. 
    
    The ready queue is treated as a circular queue. The CPU scheduler goes around the ready queue, allocating
    the CPU to each process for a time interval of up to 1 time quantum.
    """

    def __init__(self, processes, time_quantum):
        self.processes = processes
        self.number = len(processes)
        self.tq = time_quantum
        self.arrival_times = [process.arrival_time for process in processes]
        self.burst_times = [process.burst_time for process in processes]
        self.completion_times = [0] * self.number  
        self.turnaround_times = [0] * self.number
        self.waiting_times = [0] * self.number

        self.average_turnaround_time = None
        self.average_waiting_time = None

        self.readyQueue = deque()

    def run(self) -> None:
        t = 0
        completed_count = 0
        rt = self.burst_times
        index = 0
        while completed_count != self.number:
            count = 0
            while self.processes[index].arrival_time <= t and count != self.number:
                if 0 < rt[index] and not self.processes[index].in_queue:
                    self.readyQueue.append((index, self.processes[index]))
                    self.processes[index].in_queue = True
                index += 1
                count += 1
                if index == self.number:
                    index = 0
           
            if self.readyQueue:
                curr, curr_process = self.readyQueue.popleft()
                curr_process.in_queue = False
                
                if self.tq < rt[curr]:
                    t += self.tq
                    rt[curr] -= self.tq   
                           
                else:
                    t += rt[curr]
                    rt[curr] = 0
                    completed_count += 1
                    curr_process.completion_time = t             
            else:
                t += 1
              
    def calculate(self):
        for process in self.processes:
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time 

        self.completion_times = [process.completion_time for process in self.processes]
        self.turnaround_times = [process.completion_time - process.arrival_time for process in self.processes]
        self.waiting_times = [process.turnaround_time - process.burst_time for process in self.processes]

        self.average_turnaround_time = sum(self.turnaround_times) // self.number
        self.average_waiting_time = sum(self.waiting_times) // self.number

    def display(self):
        table = [[process.id, process.arrival_time, process.burst_time, process.completion_time,
                    process.turnaround_time, process.waiting_time] for process in self.processes]
        headers = ["Process", "Arrival Times", "Burst Times", "Completion Times", "Turnaround Times", "Waiting Times"]
        print(tabulate(table, headers = headers, tablefmt="grid"))

        result = [["Average Turnaround Time", self.average_turnaround_time], ["Average Waiting Time", self.average_waiting_time]]
        print(tabulate(result, tablefmt="grid"))
        

p1 = Process('p1', 0, 7)
p2 = Process('p2', 0, 2)
p3 = Process('p3', 1, 10)
processes = [p1, p2, p3]

rr = RoundRobin(processes, 2)
rr.run()
rr.calculate()
rr.display()

    
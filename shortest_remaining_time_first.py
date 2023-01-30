import heapq as hq
from process import Process
from tabulate import tabulate

class SRTF:
    """
    At any moment, out of all the processes that have arrived, the one with the shortest burst time is
    executed.
    If two processes have the same burst time, FCFS (First Come First Served is followed)
    The SJF algorithm can be either preemptive or nonpreemptive. The choice
    arises when a new process arrives at the ready queue while a previous process is still executing. The next CPU burst of the newly arrived process may
    be shorter than what is left of the currently executing process. A preemptive
    SJF algorithm will preempt the currently executing process.
    """

    def __init__(self, processes):
        self.processes = processes
        self.number = len(processes)
        self.arrival_times = [process.arrival_time for process in processes]
        self.burst_times = [process.burst_time for process in processes]
        self.completion_times = [0] * self.number  
        self.turnaround_times = [0] * self.number
        self.waiting_times = [0] * self.number

        self.average_turnaround_time = None
        self.average_waiting_time = None

        self.readyQueue = []
        
    def run(self) -> None:
        t = 0
        rt = self.burst_times
        completed_count = 0
        min_bt = float('inf')
        current_process = None

        while completed_count != self.number:
            for index, process in enumerate(self.processes):
                if process.arrival_time <= t and 0 < rt[index] < min_bt:
                    min_bt = rt[index]
                    current_process_index = index
                    current_process = process
            
            rt[current_process_index] -= 1
            if rt[current_process_index] == 0:
                completed_count += 1
                current_process.completion_time = t + 1
            
            min_bt = rt[current_process_index]
            if min_bt == 0:
                min_bt = float('inf')
            
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
p3 = Process('p3', 1, 3)
processes = [p1, p2, p3]

srtf = SRTF(processes)
srtf.run()
srtf.calculate()
srtf.display()

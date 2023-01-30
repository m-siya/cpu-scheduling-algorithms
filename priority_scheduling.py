import heapq as hq
from process import Process
from tabulate import tabulate

class PriorityScheduling:
    """
    A priority is associated with each process and the CPU is allocated to the process with the highest priority first.

    In case of equal priority, FCFS order is followed
    """

    def __init__(self, processes, pre_emptive = False):
        self.processes = processes
        self.number = len(processes)
       
        self.arrival_times = [process.arrival_time for process in processes]
        self.burst_times = [process.burst_time for process in processes]
        self.priorities = [process.priority for process in processes]

        self.completion_times = [0] * self.number  
        self.turnaround_times = [0] * self.number
        self.waiting_times = [0] * self.number

        self.average_turnaround_time = None
        self.average_waiting_time = None

        self.readyQueue = []

    def run(self) -> None:
        """at any given moment, out of all the processes in the ready queue at that moment, 
            the process with the highest (lowest numerically) priority is run first.
            
            if a new high priority process arrives, it is put at the head of the ready queue.
        """

        t = 0
        completed_count = 0
        index = 0

        while completed_count != self.number:
            while index < self.number and self.processes[index].arrival_time <= t:
                hq.heappush(self.readyQueue, (self.processes[index].priority, index))
                index += 1
            
            if self.readyQueue:
                priority, curr = hq.heappop(self.readyQueue)
                curr_process = self.processes[curr]

                t += curr_process.burst_time
                curr_process.completion_time = t
                completed_count += 1
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
        table = [[process.id, process.priority, process.arrival_time, process.burst_time, process.completion_time,
                    process.turnaround_time, process.waiting_time] for process in self.processes]
        headers = ["Process", "Priorities", "Arrival Times", "Burst Times", "Completion Times", "Turnaround Times", "Waiting Times"]
        print(tabulate(table, headers = headers, tablefmt="grid"))

        result = [["Average Turnaround Time", self.average_turnaround_time], ["Average Waiting Time", self.average_waiting_time]]
        print(tabulate(result, tablefmt="grid"))
        

p1 = Process('p1', 0, 7, 2)
p2 = Process('p2', 0, 2, 8)
p3 = Process('p3', 1, 10, 1)
processes = [p1, p2, p3]

priority = PriorityScheduling(processes, pre_emptive=False)
priority.run()
priority.calculate()
priority.display()

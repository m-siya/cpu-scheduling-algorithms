from collections import deque
from process import Process
from tabulate import tabulate

class FCFS:
    """
    The process which arrives first is executed first.
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

        self.readyQueue = deque()

    def run(self):
        for process in self.processes:
            self.readyQueue.append(process)
    
    def calculate(self):
        t = 0
        for process in self.readyQueue:
            t += process.burst_time
            process.completion_time = t
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

fcfs= FCFS(processes)
fcfs.run()
fcfs.calculate()
fcfs.display()
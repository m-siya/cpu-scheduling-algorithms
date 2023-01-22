import heapq as hq
from pgit rocess import Process
from tabulate import tabulate


class SJF:
    """
    At any moment, out of all the processes that have arrived, the one with the shortest burst time is
    executed.
    If two processes have the same burst time, FCFS (First Come First Served is followed
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
        for count, process in enumerate(self.processes):
            hq.heappush(self.readyQueue, (process.burst_time, count, process))
    
    def calculate(self):
        t = 0
        for process in self.readyQueue:
            t += process[2].burst_time
            process[2].completion_time = t
            process[2].turnaround_time = process[2].completion_time - process[2].arrival_time
            process[2].waiting_time = process[2].turnaround_time - process[2].burst_time 

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

sjf = SJF(processes)
sjf.run()
sjf.calculate()
sjf.display()

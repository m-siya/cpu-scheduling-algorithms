from collections import deque
from process import Process
from tabulate import tabulate

class FCFS:
    """
    The process which arrives first is executed first.
    """

    def __init__(self, processes):
        self.overhead = False
        self.overhead_value = 0
        self.efficiency = 0

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

    def is_overhead(self, value, overhead = True):
        self.overhead = overhead
        self.overhead_value = value

    def run(self):
        for process in self.processes:
            self.readyQueue.append(process)
    
    def calculate_no_overhead(self):
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

    def calculate_with_overhead(self):
        t = self.readyQueue[0].arrival_time
        overhead_count = 0
        for process in self.readyQueue:
            t += process.burst_time
            process.completion_time = t
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time 
            t += self.overhead_value
            overhead_count += 1
        
        t -= self.overhead_value #to get rid of extra overhead at end
        overhead_count -= 1


        self.completion_times = [process.completion_time for process in self.processes]
        self.turnaround_times = [process.completion_time - process.arrival_time for process in self.processes]
        self.waiting_times = [process.turnaround_time - process.burst_time for process in self.processes]

        self.average_turnaround_time = sum(self.turnaround_times) // self.number
        self.average_waiting_time = sum(self.waiting_times) // self.number
        self.total_overhead_time = overhead_count * self.overhead_value
        self.efficiency = (1 - self.total_overhead_time/t) * 100

        
    def calculate(self):
        if self.overhead:
            self.calculate_with_overhead()
        else:
            self.calculate_no_overhead()
    
    def display(self):
        table = [[process.id, process.arrival_time, process.burst_time, process.completion_time,
                    process.turnaround_time, process.waiting_time] for process in self.processes]
        headers = ["Process", "Arrival Times", "Burst Times", "Completion Times", "Turnaround Times", "Waiting Times"]
        print(tabulate(table, headers = headers, tablefmt="grid"))

        result = [["Average Turnaround Time", self.average_turnaround_time], ["Average Waiting Time", self.average_waiting_time], ["Efficiency", self.efficiency]]
        print(tabulate(result, tablefmt="grid"))

p1 = Process('p1', 0, 3)
p2 = Process('p2', 1, 2)
p3 = Process('p3', 2, 1)
p4 = Process('p4', 3, 4)
p5 = Process('p5', 4, 5)
p6 = Process('p6', 5, 2)

processes = [p1, p2, p3, p4, p5, p6]

fcfs= FCFS(processes)
fcfs.is_overhead(1, overhead = True)
fcfs.run()
fcfs.calculate()
fcfs.display()
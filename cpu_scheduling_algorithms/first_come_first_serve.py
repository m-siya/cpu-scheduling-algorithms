import heapq as hq
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

        self.readyQueue = []

    def run(self):
        for index, process in enumerate(self.processes):
            hq.heappush(self.readyQueue, (process.arrival_time, index))
        
        print(self.readyQueue)
        trial = []
        for index, process in enumerate(self.processes):
            trial.append(process.arrival_time)

        hq.heapify(trial)
        print(trial)


        

    
    def calculate(self):
        t = 0
        completed_count = 0

        while self.readyQueue:
            if t <= self.readyQueue[0][0]:
                at, index = hq.heappop(self.readyQueue)
                print(at, index)
                t += processes[index].burst_time
                processes[index].completion_time = t
                processes[index].turnaround_time = processes[index].completion_time - processes[index].arrival_time
                processes[index].waiting_time = processes[index].turnaround_time - processes[index].burst_time 
                
                #completed_count += 1
            else:
                t += 1

        self.completion_times = [process.completion_time for process in self.processes]
        self.turnaround_times = [process.completion_time - process.arrival_time for process in self.processes]
        self.waiting_times = [process.turnaround_time - process.burst_time for process in self.processes]

        self.average_turnaround_time = sum(self.turnaround_times) / self.number
        self.average_waiting_time = sum(self.waiting_times) / self.number
    
    def display(self):
        table = [[process.id, process.arrival_time, process.burst_time, process.completion_time,
                    process.turnaround_time, process.waiting_time] for process in self.processes]
        headers = ["Process", "Arrival Times", "Burst Times", "Completion Times", "Turnaround Times", "Waiting Times"]
        print(tabulate(table, headers = headers, tablefmt="grid"))

        result = [["Average Turnaround Time", self.average_turnaround_time], ["Average Waiting Time", self.average_waiting_time]]
        print(tabulate(result, tablefmt="grid"))

p1 = Process('p1', 3, 4)
p2 = Process('p2', 5, 3)
p3 = Process('p3', 0, 2)
p4 = Process('p4', 5, 1)
p5 = Process('p5', 4, 3)
processes = [p1, p2, p3, p4, p5]

fcfs= FCFS(processes)
fcfs.run()
fcfs.calculate()
fcfs.display()
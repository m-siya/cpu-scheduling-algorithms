class FCFS:
    def __init__(self, arrivalTimes, burstTimes):
        #self.completionTimes = [self.completionTimes[i - 1] + burstTimes[i] if i > 0 else burstTimes[0] for i in range(len(burstTimes))]

        self.completionTimes = [burstTimes[0]]
        for i in range(1, len(burstTimes)):
            self.completionTimes.append(self.completionTimes[i - 1] + burstTimes[i])
        self.turnAroundTimes = [CT - AT for CT, AT in zip(self.completionTimes, arrivalTimes)]
        self.waitingTimes = [TAT - BT for TAT, BT in zip(self.turnAroundTimes, burstTimes)]


fcfs = FCFS([0, 0, 0], [3, 4, 5])
print(fcfs.completionTimes,fcfs.turnAroundTimes, fcfs.waitingTimes)


    

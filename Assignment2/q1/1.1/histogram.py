import matplotlib.pyplot as plt

# the first array represents - SCHED_OTHER, second array represents - SCHED_RR and the third - SCHED_FIFO
other = [6.566136632, 6.894021511, 6.967029350, 6.830017032, 6.810664897]
rr = [6.943372511, 7.253149304, 7.383558768, 7.194101827, 7.164438897]
fifo = [6.816199680, 7.116233320, 7.258619069, 7.14959327, 7.586069750]

priority_other = [0, 0, 0, 0, 0]
priority_rr = [47, 23, 45, 99, 2]
priority_fifo = [47, 68, 16, 2, 99]

for i in range(5):
    plt.xlabel("SET PRIORITIES")
    plt.ylabel("RECORDED TIME(sec)")
    plt.title(f'SET - {i+1}')

    plt.bar(priority_other[i],other[i],10,label = "OTHER")
    plt.bar(priority_rr[i],rr[i],10,label = "RR")
    plt.bar(priority_fifo[i],fifo[i],10,label = "FIFO")
    plt.show()

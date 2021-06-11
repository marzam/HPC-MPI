#!/home/mzamith/Apps/anaconda3/bin/python
import datetime
import numpy as np
import matplotlib.pyplot as plt

def graphic_speedup_data(X, Y1, Y2, Y3):
    S1 = []
    S2 = []
    myMax = 0
    for i in range(0, len(Y1)):
        s = Y1[i] / Y2[i]
        myMax = max(myMax, s)
        S1.append(s)
        s = Y1[i] / Y3[i]
        myMax = max(myMax, s)
        S2.append(s)

    fig = plt.figure()
    ax = fig.add_subplot(111)


    ax.plot(X, S1, color='red', linewidth=1)
    ax.plot(X, S2, color='blue', linewidth=1)
    ax.legend(['Aceleração c/struct', 'Aceleração s/struct'])

    plt.yticks(np.arange(0, myMax+1, 1))#
    plt.xlabel('bytes trafegados')
    plt.ylabel('acerelaração')
    plt.title('n-corpos')

    plt.show()


def graphic_time_data(X, Y1, Y2, Y3):

    fig = plt.figure()
    ax = fig.add_subplot(111)


    ax.plot(X, Y1, color='red', linewidth=1)
    ax.plot(X, Y2, color='green', linewidth=1)
    ax.plot(X, Y3, color='blue', linewidth=1)

    ax.legend(['1 thread c/struct', '6 thread c/struct', '6 thread s/struct'])


    plt.xlabel('bytes trafegados')
    plt.ylabel('tempo em segundos')
    plt.title('n-corpos')

    plt.show()

def getSeconds(mytime):
    p = mytime.find(':')
    hour = int(mytime[:p]) * 3600
    aux = mytime[p+1:]
    p = aux.find('.')
    minutes = int(aux[:2]) * 60
    seconds = int(aux[p+1:])
    return hour + minutes + seconds

def loadData(yFileName, xFileName):
    infile = open(yFileName, 'r')
    lines = infile.readlines()
    Y = []
    for line in lines:
        p = line.find('system')
        q = line.find('elapsed')
        if p > 0:
            mytime = line[p+6:q].strip()
            seconds = getSeconds(mytime)
            Y.append(seconds)
#12
    infile.close()

    infile = open(xFileName, 'r')
    lines = infile.readlines()
    X = []
    for line in lines:
        p = line.find('bytes')
        if p > 0:
            bytes = int(line[12:p])
            X.append(bytes)
    infile.close()
    return X, Y

X1, Y1 = loadData('n-bodies.txt', 'n-bodies-mem.txt')
X2, Y2 = loadData('n-bodies-openmp.txt', 'n-bodies-openmp-mem.txt')
X3, Y3 = loadData('n-bodies-openmp-st.txt', 'n-bodies-openmp-mem-st.txt')
graphic_speedup_data(X1, Y1, Y2, Y3)
graphic_time_data(X1, Y1, Y2, Y3)

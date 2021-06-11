#!/home/mzamith/Apps/anaconda3/bin/python
import pandas as pd
import matplotlib.pyplot as plt
import sys
if __name__ == "__main__":
    nemesis_sync = pd.read_csv('nemesis.sync.log', sep=';', encoding='utf8',  low_memory=False)
    nemesis_async = pd.read_csv('nemesis.async.log', sep=';', encoding='utf8',  low_memory=False)
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(nemesis_sync['bytes'], nemesis_sync['tempo'], 'go-', color='red', linewidth=3)
    ax.plot(nemesis_sync['bytes'], nemesis_async['tempo'], 'go-', color='blue', linewidth=3)
    #ax.set_yscale('log')
    #ax.plot(days, R, color='blue', linewidth=3)
    ax.legend(['Blocante', 'Não blocante'])
    plt.xlabel('bytes transmitido')
    plt.ylabel('custo da chamada da função')
    #plt.title(title)
    plt.grid(True)
    plt.savefig('grafico.pdf')
    plt.show()

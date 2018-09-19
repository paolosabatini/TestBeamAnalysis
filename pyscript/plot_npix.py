#!/usr/bin/env python

#import numpy as np
#import seaborn as sns
import matplotlib.pyplot as plt



def npix_vs_run(dict_output):
    d_mu = {}
    d_sigma = {}
    for name, value in dict_output.iteritems():
        run = int(name[4:9])
        if "npix" in name and "mu" in name:
            d_mu[int(run)] = value
        if "npix" in name and "sigma" in name:
            d_sigma[int(run)] = value
    l = sorted(d_mu.items())
    x,y = zip(*l)
    plt.xlabel("Run Number")
    plt.ylabel("Average number of pixel in cluster")
    plt.plot(x,y)
    plt.show()
    

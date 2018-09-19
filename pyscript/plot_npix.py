#!/usr/bin/env python

import matplotlib.pyplot as plt


def npix_vs_run(dict_output):
    d_mu = {}
    d_sigma = {}
    for name, value in dict_output.iteritems():
        run = int(name[4:9])
        print run
        if "npix" in name and "mu" in name:
            d_mu[int(run)] = value
        if "npix" in name and "sigma" in name:
            d_sigma[int(run)] = value
    l = sorted(d_mu.items())
    x,y = zip(*l)
    print x,y
    plt.plot(x,y)
    plt.show()

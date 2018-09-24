#!/usr/bin/env python

#import numpy as np
#import seaborn as sns
import matplotlib.pyplot as plt
import math


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
    
def npix_vs_hv(dict_output):
    d_mu = {}
    d_sigma = {}
    for name, value in dict_output.iteritems():
        run = int(name[4:9])
        if "tot" in name and "mu" in name:
            d_mu[int(run)] = value
        if "tot" in name and "sigma" in name:
            d_sigma[int(run)] = value

    dict_hv = {
        '50_min': 5580, '50_max': 5620,
        '80_min': 5630, '80_max': 5670,
        '100_min': 5680, '100_max': 5720,
        '150_min': 5780, '150_max': 5820,
        '200_min': 5830, '200_max': 5870,
        '300_min': 5880, '300_max': 5920
    }

    d_mu_avg = {}
    d_mu_sigavg = {}
    d_mu_num = {}
    for label in dict_hv:
        hv = label.split("_")[0]
        d_mu_num [hv] = 0
        d_mu_avg [hv] = 0
        d_mu_sigavg [hv] = 0
        
    for run, value in d_mu.iteritems():
        for hv_label in dict_hv.keys():
            hv = hv_label.split("_")[0]
            if (int(run) <= int(dict_hv[hv+"_max"])) and (int(run) >= int(dict_hv[hv+"_min"])):
                d_mu_num [hv] += 1
                d_mu_avg [hv] += float(value)

    for hv,v in d_mu_avg.iteritems():
        if d_mu_num[hv] == 0:
            continue
        d_mu_avg [hv] = float(v) / float(d_mu_num[hv])


                
    for run, value in d_mu.iteritems():
        for hv_label in dict_hv.keys():
            hv = hv_label.split("_")[0]
            if (int(run) <= int(dict_hv[hv+"_max"])) and (int(run) >= int(dict_hv[hv+"_min"])):
                d_mu_sigavg [hv] += (value-d_mu_avg[hv])*(value-d_mu_avg[hv])/(float(d_mu_num[hv])-1)

    for hv,v in d_mu_sigavg.iteritems():
        d_mu_sigavg [hv] = math.sqrt(float(v))
 
                
    l = sorted(d_mu_avg.items())
    x,y = zip(*l)
    err_l = sorted(d_mu_sigavg.items())
    err_x,err_y = zip(*err_l)
    plt.xlabel("HV [V]")
    plt.ylabel("Average ToT of cluster on track")
    plt.errorbar(x,y,err_y,marker='.', mfc='black',ms=10,color="black",ls="")
    plt.xlim((0,350))
    plt.ylim((0,8))
    plt.show()

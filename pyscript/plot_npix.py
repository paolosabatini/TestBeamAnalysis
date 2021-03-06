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
    
def npix_vs_hv(dict_output, tune):
    d_mu = {}
    d_sigma = {}
    for name, value in dict_output.iteritems():
        run = int(name[4:9])
        if "npix" in name and "mu" in name:
            d_mu[int(run)] = value
        if "npix" in name and "sigma" in name:
            d_sigma[int(run)] = value

    dict_hv = {}
    if tune == "8at16t2k":
        dict_hv = {
            '50_min': 5376, '50_max': 5425,
            '80_min': 5426, '80_max': 5475,
            '100_min': 5476, '100_max': 5525,
            '150_min': 5526, '150_max': 5575,
            '200_min': 5576, '200_max': 5625,
            '250_min': 5626, '250_max': 5675,
            '300_min': 5676, '300_max': 5725,
            '350_min': 5726, '350_max': 5775,
            '400_min': 5776, '400_max': 5825,
            '500_min': 5826, '500_max': 5875,
            '600_min': 5876, '600_max': 5925,
        }
    elif tune == "8at16t2p5k":
        dict_hv = {
            '50_min': 5932, '50_max': 5933,
            '80_min': 5934, '80_max': 5938,
            '100_min': 5939, '100_max': 5943,
            '150_min': 5944, '150_max': 5948,
            '200_min': 5949, '200_max': 5953,
            '250_min': 5954, '250_max': 5958,
            '300_min': 5959, '300_max': 5963,
            '350_min': 5964, '350_max': 5968,
            '400_min': 5969, '400_max': 5973,
            '500_min': 5974, '500_max': 5978,
            '600_min': 5979, '600_max': 5983,
        }

    else:
        print ">>>  [plot_npix] Tuning point %s not found as schedule"

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
    plt.ylabel("Average number of pixels in cluster")
    plt.errorbar(x,y,err_y,marker='.', mfc='black',ms=10,color="black",ls="")
    plt.xlim((0,650))
    plt.ylim((0,3))
    plt.show()

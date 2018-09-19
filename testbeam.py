#!/usr/bin/env python

import os, glob
import argparse
import sys,subprocess
sys.path.append('pyscript/')
import plot_npix 


sample_dir = "sample"

parser = argparse.ArgumentParser(description='Launch analysis on testbeam.')
parser.add_argument('--sample', dest='sample', choices = ["track-data","match-trees"], default="track-data", help='which type of files to use')
parser.add_argument('--npix', dest='npix', action = "store_true", default=False, help='Execute the npix analysis')
args = parser.parse_args()

wildcard = sample_dir+"/*"+args.sample+".root"
list_samples = glob.glob(wildcard)

dict_output = {}

for f in list_samples:
    run = f.split("/")[1][0:9]
    if args.npix:
        cmd = "root -l -q 'root/npix.C("+'"'+str(f)+'"'+","+'"mean"'+")'"
        dict_output [run+"_npix_mu"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])
        cmd = "root -l -q 'root/npix.C("+'"'+str(f)+'"'+","+'"err"'+")'"
        dict_output [run+"_npix_sigma"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])



if args.npix:
    plot_npix.npix_vs_run(dict_output)


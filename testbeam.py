#!/usr/bin/env python

import os, glob
import argparse
import sys,subprocess
sys.path.append('pyscript/')
import plot_npix
import plot_tot
import plot_eff


sample_dir = "sample"

parser = argparse.ArgumentParser(description='Launch analysis on testbeam.')
parser.add_argument('--npix', dest='npix', action = "store_true", default=False, help='Execute the npix analysis: npix')
parser.add_argument('--tot', dest='tot', action = "store_true", default=False, help='Execute the npix analysis: tot')
parser.add_argument('--eff',dest='eff', action = "store_true", default=False, help='Execute the npix analysis: eff')
args = parser.parse_args()

sample_type = ""
if args.npix or args.tot:
    sample_type = 'match-trees'
else:
    sample_type = 'tracks-data'

wildcard = sample_dir+"/*"+sample_type+".root"
list_samples = glob.glob(wildcard)

dict_output = {}

for f in list_samples:
    run = f.split("/")[1][0:9]
    if args.npix:
        cmd = "root -l -q 'root/npix.C("+'"'+str(f)+'"'+","+'"mean"'+")'"
        dict_output [run+"_npix_mu"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])
        cmd = "root -l -q 'root/npix.C("+'"'+str(f)+'"'+","+'"err"'+")'"
        dict_output [run+"_npix_sigma"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])
    if args.tot:
        cmd = "root -l -q 'root/tot.C("+'"'+str(f)+'"'+","+'"mean"'+")'"
        dict_output [run+"_tot_mu"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])
        cmd = "root -l -q 'root/tot.C("+'"'+str(f)+'"'+","+'"err"'+")'"
        dict_output [run+"_tot_sigma"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])
    if args.eff:
        cmd = "root -l -q 'root/eff.C("+'"'+str(f)+'"'+","+'"mean"'+")'"
        os.system(cmd)
        # dict_output [run+"_eff_mu"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])
        # cmd = "root -l -q 'root/tot.C("+'"'+str(f)+'"'+","+'"err"'+")'"
        # dict_output [run+"_eff_sigma"] = float(os.popen(cmd).read().replace('(double)','').split("\n")[2])



if args.npix:
    plot_npix.npix_vs_run(dict_output)

if args.tot:
    plot_tot.tot_vs_run(dict_output)

# if args.eff:
#     plot_eff.eff_vs_run(dict_output)

#!/usr/bin/env python

import os, glob
import argparse

sample_dir = "sample"

parser = argparse.ArgumentParser(description='Launch analysis on testbeam.')
parser.add_argument('--sample', dest='sample', choices = ["track-data","match-trees"], default="track-data", help='which type of files to use')
parser.add_argument('--npix', dest='npix', action = "store_true", default=False, help='Execute the npix analysis')
args = parser.parse_args()

wildcard = sample_dir+"/*"+args.sample+".root"
list_samples = glob.glob(wildcard)

for f in list_samples:
    
    if args.npix:
        cmd = "root -l -q 'root/npix.C("+'"'+str(f)+'"'+")'"
        print cmd
        os.system(cmd)

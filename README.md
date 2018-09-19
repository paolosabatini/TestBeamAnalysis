# TestBeam analysis code

The code is structured with a python main `testbeam.py` that manages all the possible analysis one wants to do with the root files in `sample/` directory.

To get more info on possible arguments to pass, just type `./testbeam.py --h`, here an example:

```python
./textbeam.py --sample track-data --tot
```

This takes as input the files in samples that matches the wildcard `*track-data.root` and executes `root/tot.C` and `pyscript/plot_tot.py` to get the plot of average ToT of clusters on tracks vs run number. Another available analysis is `npix` that gives the average number of pixel in clusters vs run number.

Other analysis of this type can be easily done just by:
- Adding another argument in the parser
- Copy the block in the file loop referring to the analysis in `testbeam.py`
- Copy the file in `root/` and adapt it to your analysis (structure of loop over tree should be the same)
- Copy the block after loop in `testbeam.py`
- Copy the `pyscript/plot*` file and include it in `testbeam.py`

### Contacts

In case of any question, contact me: `paolo.sabatini@cern.ch`
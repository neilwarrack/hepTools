#!/usr/bin/env python3
## this is designed to be run from a virtual env with uproot and mplhep installed
#  e.g. use:
#            conda activate myuproot4env2


#### user config #####
filename = "Hists/16e/emu_etmiss_phi_nominal.root"
histname = "data"
savename = "testsave.png"
title    = "test title"
xlabel   = "met phi"
ylabel   = "events"
######################


import uproot as up
import matplotlib.pyplot as plt
import mplhep as hep


f = up.open(filename)
d = f[histname].to_numpy()
fig, ax = plt.subplots()
ATLAStext = "internal        " + title
hep.atlas.text(ATLAStext)
ax.hist(d[1][:-1], d[1], weights=d[0])
ax.set_xticks(d[1][::2])
ax.set_xlabel(xlabel)
ax.set_ylabel(ylabel)
fig.savefig(savename)

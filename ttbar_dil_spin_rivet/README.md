submit on the grid like:
```
lsetup panda
pathena --extOutFile=MyOutput.yoda.gz \
--inDS=mc15_13TeV.123456.MySampleOfInterest.evgen.EVNT.e1234 \
--outDS=user.`whoami`.ttbar_dil_spin.v1 \
grid_run.py
```
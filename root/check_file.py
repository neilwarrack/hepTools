#!/usr/bin/env python3

# you may need to do this first:
# conda activate myuproot4env2

import uproot as up

listfile = open('user/addresses/user.rlysak.410472.PhPy8EG.DAOD_TOPQ1.e6348_s3126_r10724_p4031.AB-21.2.109-MC16eSignalNominal-v01_out_root.txt', 'r')

entryctr = 0

for line in listfile:
    #print(line)
# signal
    f = up.open(line[:-1])
    entryctr += f["nominal"].num_entries
    print(entryctr)

exit()

f = up.open("/eos/user/n/nwarrack/data/samples_AB212109_com_01_MC16e_only/user.rlysak.410472.PhPy8EG.DAOD_TOPQ1.e6348_s3126_r10724_p4031.AB-21.2.109-MC16eSignalNominal-v01_out_root/user.rlysak.20651281._000001.out.root")
f["truth"].keys()

# eft
f = up.open("/eos/user/n/nwarrack/data/samples_AB212109_com_01_MC16e_only/user.nwarrack.504488.MGPy8.DAOD_TOPQ1.e8276_a875_r10724_p4346.AB-21.2.158-MC16eEFT-eftv02_out_root/user.nwarrack.24279982._000001.out.root")
f["truth"].keys()

#!/usr/bin/env python

import yoda
from yoda.script_helpers import filter_aos

aodict1 = yoda.read("full_stats.yoda")
aodict2 = yoda.read("out_fid_18files_all.yoda")

for path in sorted(set(list(aodict1.keys()) + list(aodict2.keys()))):

    print(path)

    ao1 = aodict1.get(path, None)
    ao2 = aodict2.get(path, None)
        
    if ao2 is None or ao2 is None:
        continue
    else:
        s1 = ao1.mkScatter()
        s2 = ao2.mkScatter()

        for i, (p1, p2) in enumerate(zip(s1.points, s2.points)):
            if p1.y == 0:
                print("ZERO in histo 1!", p1.x)
            if p2.y == 0:
                print("ZERO in histo 2 (skipping ratio)!", p2.x)
            else:
                print("ratio: ", p1.y/p2.y)

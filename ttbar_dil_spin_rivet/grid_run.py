#theApp.EvtMax = -1
theApp.EvtMax = 200

import AthenaPoolCnvSvc.ReadAthenaPool

from AthenaCommon.AlgSequence import AlgSequence
job = AlgSequence()

from Rivet_i.Rivet_iConf import Rivet_i
rivet = Rivet_i()
import os
rivet.AnalysisPath = os.environ['PWD']

rivet.Analyses += [ 'ttbar_dil_spin' ]
rivet.RunName = ''
rivet.HistoFile = 'MyOutput.yoda.gz'
rivet.CrossSection = 1.0
#rivet.IgnoreBeamCheck = True
#rivet.SkipWeights = True
job += rivet

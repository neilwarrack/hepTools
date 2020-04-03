#!/usr/bin/env python
import os
from os.path import join
import subprocess

from optparse import OptionParser

#This script creates a file list out of data samples found via rucio


#----------------------Functions-----------------------
#Get data files from rucio output
def get_data(output, user):
   Sample = []
#Rucio uses | delimiter
   for line in output.split('|'):
      if user in line:
         Sample.append(line.strip()) 
   return Sample 

#user.spieker:user.spieker.20180926.data16_13TeV.00310473.physics_Main.deriv.DAOD_EXOT5.Skim16_v5_outputXAODStream
def get_did( sample ):
      sampleName =  sample.split(".") 
      did = 0
      if len(sampleName) > 6:
        did = sampleName[5]
      else:
         print 'sample: %s has a wrong nomenclature' %(sample)
         exit()
      return did

#def get_version( sample ):
#      sampleName =  sample.split(".") 
#      did = 0
#      if len(sampleName) > 6:
#      #Last element
#      end = sampleName[-1]
#      skim = end.split("_")
#      version = skim[1] 
#      ver = ""
#      if version[0] not "v":
#         print 'Not picked up version!'
#         exit()
#         return 0
#      else return int(version[1:-1]) 
#      #Double check that the first letter is v


#Loop through files with the same DIDs and choose the one with latest version
#def get_latest(samples):
##Loop over files and check whether any two shares did
#  #for sample in samples:
#  for i in range(0,len(samples)):
#      did = get_did(samples[i]) 
#      saveSample = True
#      for j in range(i+1, len(samples)):  
#         did2 = get_did(samples[j]) 
#         if( did == did2 ):
#                #Find out which sample version is higher
#                if ( get_version( samples[i] ) < get_version( samples[j] ) ):             
            
      


#----------------------Begin-----------------------------
p = OptionParser()

p.add_option('--user', '-u', type='string',  default='nwarrack',    dest='user')
p.add_option('--sample', '-s', type='string',  default=None,    dest='sample')
p.add_option('--status',  type='string',  default=None,    dest='status')
p.add_option('--resub',  action="store_true",  default=False,    dest='resub')
p.add_option('--kill',  action="store_true",  default=False,    dest='kill')
#p.add_option('--resub',  action="store_true",  default=False,    dest='resub')
p.add_option('--outputDIDs',  action="store_true",  default=False,    dest='dids')

(options,arg) = p.parse_args()

user = 'group.' + options.user
#user = 'user.' + options.user

if options.sample == None:
  print 'No sample name provided!'
  exit()


search = user + "*" + options.sample

#PandaMonitoring command to list jobs
cmdPM = 'pandamon '
cmdPM_test = 'pandamon '
cmdPM += search
cmdPM_test += search




getID = str(cmdPM)
#pandamon | awk '$1 ~ "finished" {print $2}' | panda-resub-taskid
if options.status != None:
   jobStatus = '"%s"' %options.status
   cmdPM += " | awk '$1 ~ %s {print $1, $2, $3, $4}'  " %jobStatus
   cmdPM_test += " | awk '$1 ~ %s {print $4}'  " %jobStatus

   getID += " | awk '$1 ~ %s {print $2}'  " %jobStatus

#List all data samples sharing the version of the skim and run number
#cmd = 'rucio list-dids %s' % sampleName
#print 'Working on %s' %sampleName
print cmdPM
print 'you are here'
#print getID
rucioOutput = subprocess.Popen( cmdPM , stdout=subprocess.PIPE,shell=True)
rucioOutput_test = subprocess.Popen( cmdPM_test , stdout=subprocess.PIPE,shell=True)
(out, err) = rucioOutput.communicate()
(out_test, err) = rucioOutput_test.communicate()

#FIXME what if no status specified
print 'List of jobs with the status: %s' %options.status
print out


getIDout = subprocess.Popen( getID , stdout=subprocess.PIPE,shell=True)
(outID, errID) = getIDout.communicate()

print 'outputting listID...'
listID = outID.split()
print listID


if options.resub:
   for task in listID:
      print 'Resubmiting %s' %task 
      cmd =  'panda-resub-taskid %s' %task
      print cmd
      os.system( cmd)


if options.kill:
   for task in listID:
      print 'Killing %s' %task 
      cmd =  'panda-kill-taskid %s' %task
      print cmd
      os.system( cmd)

if options.dids:
   print '------------------ LIST OF OUTPUT DIDS ----------------'
   print out_test
   print '...'

   did = out_test
   print 'did='
   print did

   did = did[:-2]
   print did

   print '------------------ LIST OF OUTPUT DIDS FINAL ----------------'
   did = did.replace("/\n", "\n")
   did = did.replace(user, user + ":" + user)

   print did

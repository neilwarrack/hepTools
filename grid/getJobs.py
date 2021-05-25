#!/usr/bin/env python

import os
from os.path import join
import subprocess
from optparse import OptionParser

# This script creates to-your-terminal list of data samples found using rucio
# This script relies on pandamon being executable in your bash shell
# For more info about pandamon see:  https://github.com/dguest/pandamonium


# Functions for string manipulations
def get_data(output, user):
   Sample = []
   for line in output.split('|'):
      if user in line:
         Sample.append(line.strip()) 
   return Sample 


def get_did( sample ):
      sampleName =  sample.split(".") 
      did = 0
      if len(sampleName) > 6:
        did = sampleName[5]
      else:
         print 'sample: %s has a wrong nomenclature' %(sample)
         exit()
      return did


p = OptionParser()

p.add_option('--user', '-u', type='string',  default='nwarrack',    dest='user')
p.add_option('--sample', '-s', type='string',  default=None,    dest='sample')
p.add_option('--status',  type='string',  default=None,    dest='status')
p.add_option('--resub',  action="store_true",  default=False,    dest='resub')
p.add_option('--kill',  action="store_true",  default=False,    dest='kill')
p.add_option('--outputDIDs',  action="store_true",  default=False,    dest='dids')


(options,arg) = p.parse_args()

# options to search using 'group' or 'user'
user = 'group.' + options.user # then use "-u phys-top", or something like that...
#user = 'user.' + options.user

if options.sample == None:
  print 'No sample name provided!'
  exit()


search = user + "*" + options.sample

#PandaMonitoring command to list jobs
cmdPM = 'pandamon '
cmdPM_test = 'pandamon '

# comment this line in to search for more/less 'days back' than default
#cmdPM += '-d 40 '

cmdPM += search
cmdPM_test += search

getID = str(cmdPM)
if options.status != None:
   jobStatus = '"%s"' %options.status
   cmdPM += " | awk '$1 ~ %s {print $1, $2, $3, $4}'  " %jobStatus
   cmdPM_test += " | awk '$1 ~ %s {print $4}'  " %jobStatus

   getID += " | awk '$1 ~ %s {print $2}'  " %jobStatus

# List all data samples sharing the version of the skim and run number
#cmd = 'rucio list-dids %s' % sampleName
#print 'Working on %s' %sampleName
print cmdPM
print 'you are here'
#print getID
rucioOutput = subprocess.Popen( cmdPM , stdout=subprocess.PIPE,shell=True)
rucioOutput_test = subprocess.Popen( cmdPM_test , stdout=subprocess.PIPE,shell=True)
(out, err) = rucioOutput.communicate()
(out_test, err) = rucioOutput_test.communicate()

# todo: what if no status specified
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

***Updating your CERN grid Certifiate***

(10 steps to grid happiness, presuming you already have an old expired grid certificate)
(some help for CERN users here: https://ca.cern.ch/ca/Help/?kbid=024010)



Your grid certificate has run out, you need to renew it.

**step #1**

Go here: https://ca.cern.ch/ca/

**step #2** 

Click the link that says something like: "New Grid User certificate"

**step #3** 

Pick a certificate password if you want - you don't have to (this password will allow you to generate your usercert.pem and userkey.pem which will allow you to use the grid).

**step #4** 

When prompted, download your new certificate (which will be called something like myCertificate.p12, you may need to type your pssword if you chose one in the previous step)

**step #5** 

All the following instructions should be typed into a terminal. Move your myCertificate.p12 to the machine that requires grid access. You can do this by using something like `rsync`. For example, from the place where your downloaded certificate is, do something like:
```bash
rsync -avz myCertificate.p12 username@lxplus.cern.ch:~/
```

**step #6** 

Log onto the machine that requires grid access (i.e. where you just sent your myCertificate.p12 to) and extract your public key (you will be prompted to enter the 'Import Password' you created in *step #3*): 

`openssl pkcs12 -in myCertificate.p12 -clcerts -nokeys -out usercert.pem`

**step #7** 

In this step you will be be prompted to enter the 'Import Password', that's the one you created in *step #3*, and then you will be prompted to enter and confirm a *NEW* 'PEM pass phrase' which is what you can think of as your 'grid password'. You may be asked to enter this 'PEM pass phrase' twice. To extract your private key, enter the following on the command line.

`openssl pkcs12 -in myCertificate.p12 -nocerts -out userkey.pem`

**step #8** 

make sure the permissions are correctly set for these files by typing the following two commands:

1) `chmod 400 userkey.pem`

2) `chmod 444 usercert.pem`


**step #9** 

Move the new files `userkey.pem` and `usercert.pem` to the directory "~/.globus/" (make it if it doesn't already exist and remove any old stuff from it if it does). 

1) `mv userkey.pem ~/.globus/`

2) `mv usercert.pem ~/.globus/`


**step #10**
Check things work (if you are part of the ATLAS collaboration) by typing `setupATLAS` and then `lsetup rucio`. When you set up rucio you will be prompted to enter "voms-proxy-init -voms atlas" which will then ask for your new pass phrase (this is the PEM pass phrase you chose in Step 7 - not your 'Import password').

**step #11**
If nothing goes wrong, you should have access to the grid and can delete your myCertificate.p12 file (both copies!). The grid is now accessed via your keys in the `.globus/` directory and your PEM pass phrase.


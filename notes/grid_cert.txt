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

Move your myCertificate.p12 to the machine that requires grid access

**step #6** 

To extract your certificate which contains your public key enter the following on the command line where your downloaded .p12 file is (you will be prompted to enter the 'Import Password' you created in *step #3*): 

`openssl pkcs12 -in myCertificate.p12 -clcerts -nokeys -out usercert.pem`

**step #7** 

To extract your private key, enter the following on the command line (you will be prompted to enter the 'Import Password' you created in *step #3*, and then you will be prompted to enter and confirm a *NEW* 'PEM pass phrase' which is what you can think of as your 'grid password'):

`openssl pkcs12 -in myCertificate.p12 -nocerts -out userkey.pem`

**step #8** 

change some permissions by typing the following two commands:

1) `chmod 400 userkey.pem`

2) `chmod 444 usercert.pem`


**step #9** 

Move the new files `userkey.pem` and `usercert.pem` to the directory "~/.globus/" (make it if it doesn't already exist and remove any old stuff from it if it does). 


**step #10** 
Delete myCertificate.p12  
Go grid yourself!

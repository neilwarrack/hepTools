**Updateing your grid Certifiate**
*(Presuming you already have one)*

Your grid certificate has run out, you need to renew it.
*step #1*
Go here: https://ca.cern.ch/ca/

*step #2*
Click the link that says something like: "New Grid User certificate"

*step #3*
Pick a certificate password (that will allow you to generate your usercert.pem and userkey.pem which will allow you to use the grid).

*step #4*
When prompted, download your new certificate (which will be called something like myCertificate.p12)

*step 5*
Move your myCertificate.p12 to the machine that requires grid access

*step 6*
Enter the following lines on the command line (where you will be prompted to enter your password and create a NEW password which will be the one you get asked for when you try to use the grid etc.):
> openssl pkcs12 -in mycert.p12 -clcerts -nokeys -out usercert.pem
> openssl pkcs12 -in mycert.p12 -nocerts -out userkey.pem
> chmod 400 userkey.pem
> chmod 444 usercert.pem


*step 7*
Move the files userkey.pem and usercert.pem to your directory ".globus/" (make it if it's not there and remove any old stuff). You should be good to go!


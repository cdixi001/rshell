#! /bin/bash

#read recipient sender
website=`echo $2 | cut -d '@' -f 2`
body=`cat`

echo "recipiennt $1 sender $2"

(
sleep 3
echo "helo $website"
sleep 2
echo "mail from: $2"
sleep 2
echo "rcpt to: $1"
sleep 2
echo "data"
sleep 2
echo "Subject: I'm da captain"
sleep 2
echo "$body"
sleep 2
echo "."
) | 
telnet mail.cs.ucr.edu 25

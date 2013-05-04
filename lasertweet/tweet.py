def removeNonAscii(s): return "".join(filter(lambda x: ord(x)<128, s))

def waitForReady():
	#clears out any "ready" responses previously received
	sleep(2)
	while ser.inWaiting() > 0:
		line = ser.readline()

	#wait for a fresh "ready" before sending next tweet (or any string followed by new line)
	print "Waiting for ready before sending string..."
	line = ser.readline()
	print "Received:" + line
	return


import serial
from time import sleep
import twitter

api = twitter.Api()
ser = serial.Serial('COM7', 9600)

#searchstring in CAPS
searchString = "#INSTRUCTABLES"
tweetsToGet = 20
filterRetweets = True

#keeps arduino from rebooting
sleep(2)
ser.setDTR(level=0)
sleep(2)

#main loop
while True:
	
	#hard coded 'intro' to display each loop - always end messages with \r\n!
	waitForReady();
	ser.write ("LASERTWEET - LINK TO INSTRUCTABLE AT NOTHINGLABS.COM\r\n")

	statuses = api.GetSearch(searchString, per_page=tweetsToGet, page=1)
	
	statusesNoRT = []
	for tweet in statuses:
		if tweet.text.find("RT @") == -1:
			statusesNoRT.append(tweet)
	
	summary = "-----" + str(len(statuses)) + " TWEETS FOUND CONTAINING '" + searchString + "' (" + \
			str(len(statuses)-len(statusesNoRT)) + " RETWEETS)" + "-----\r\n"
	
	if filterRetweets:
		statuses = statusesNoRT
		summary = summary.replace("RETWEETS", "RETWEETS REMOVED")

	waitForReady();
	print (summary)
	ser.write (summary)

	for tweet in statuses:
		
		#convert to uppercase / cleanup
		laserString = tweet.user.screen_name.upper() + ":" + removeNonAscii(tweet.text).upper()
		laserString = laserString .replace("&AMP", "&")
		
		#remove any stray new lines - and add \r\n at end
		laserString = laserString .replace("\n", "")
		laserString = laserString .replace("\r", "")
		laserString = laserString + "\r\n"
		
		waitForReady();
		print (laserString)
		ser.write(laserString)


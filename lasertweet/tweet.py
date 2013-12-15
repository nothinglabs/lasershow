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

api = twitter.Api(consumer_key='yDkaORxEcwX6SheX6pa1fw',
                      consumer_secret='VYIGd2KITohR4ygmHrcyZgV0B74CXi5wsT1eryVtw',
                      access_token_key='227846642-8IjK2K32CDFt3682SNOOpnzegAja3TyVpzFOGrQj',
                      access_token_secret='L6of20EZdBv48EA2GE8Js6roIfZFnCKBpoPwvBDxF8',
                      cache=None)

ser = serial.Serial('COM21', 2400)

#searchstring in CAPS
searchString = "ADASBOOKS"
tweetsToGet = 10
filterRetweets = True

#keeps arduino from rebooting
sleep(2)
ser.setDTR(level=0)
sleep(2)

#main loop
while True:

	print ("Starting script")
	
	#hard coded 'intro' to display each loop - always end messages with \r\n!
	waitForReady();
	ser.write ("LASERTWEET - LINK TO INSTRUCTABLE AT NOTHINGLABS.COM\r\n")

	print ("Scanning twitter")

	waitForReady();



	try:

		statuses = api.GetSearch(searchString, count = 10)

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
		break
	except:
		print ("NETWORK FAIL!!!")
		waitForReady();
		ser.write ("UNABLE TO ACCESS TWITTER - NETWORK FAIL!!!\r\n")


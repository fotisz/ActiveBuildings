#!/usr/local/bin/python

import pymongo
import datetime
import re

def recordHasLocation(ltag, rtag, location_tokens):
	for loc in location_tokens[1:]:
		tagValue = int(loc)
		if (ltag == tagValue or rtag == tagValue):
			return True
	return False
	
	
participant_tags = [line.strip() for line in open('participant_tags.csv')]

stationary_tags = [line.strip() for line in open('location_tags.csv')]

seen_tags = {}

print "participant tags:"
for participant in participant_tags:
	print participant

print "infrastructure tags:"
for i_tag in stationary_tags:
	print i_tag

#print "keys : " , seen_tags.keys()

#connect to openbeacon
from pymongo import Connection
#this connects to the default port and stuff for the mogodb daemon
connection = Connection()
#change to openbeacon database
db = connection.openbeacon
#print the collections or "tables"
print db.collection_names()

participant_name, participant_tag = re.split(',', participant_tags[0])

#print "number of records is: ", sightings.count()
for current_location in stationary_tags:
	location_tokens = re.split(',', current_location)
	print "for location " , location_tokens[0], ":"
	#start with my desk, 157
	isParticipantAtLocation = False
	lastTimeSeenAtLocation = 0
	timeThreshold = 20
	sightings = db.edge.find ( { "tag" : int(participant_tag)},{"_id":0}).sort("time")
	for sighting in sightings:
		#print sighting
		#right and left tags	
		ltag = int(sighting['tag'][0])
		rtag = int(sighting['tag'][1])  
		power = int(sighting['power'])
		time = int(sighting['time'])
	
		if(isParticipantAtLocation == True):
			
			if(recordHasLocation(ltag, rtag, location_tokens) == True):
				lastTimeSeenAtLocation = time
			elif(time - lastTimeSeenAtLocation > timeThreshold):
				humanReadableDate = datetime.datetime.fromtimestamp(lastTimeSeenAtLocation)
				print participant_name, "finished ", location_tokens[0], " at time: " , humanReadableDate 
				isParticipantAtLocation=False;
		if(recordHasLocation(ltag, rtag, location_tokens) == True):
			if(isParticipantAtLocation == False):
				isParticipantAtLocation = True
				lastTimeSeenAtLocation = time
				humanReadableDate = datetime.datetime.fromtimestamp(time)
				print participant_name, "started ", location_tokens[0], " at time: " , humanReadableDate 
	if(isParticipantAtLocation == True):
		humanReadableDate = datetime.datetime.fromtimestamp(lastTimeSeenAtLocation)
		print participant_name, "finished ", location_tokens[0], " at time: " , humanReadableDate 
	
		

	
		
		

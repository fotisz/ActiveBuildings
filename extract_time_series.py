#!/usr/local/bin/python

import pymongo
import datetime
import re
participant_tags = [line.strip() for line in open('participant_tags.csv')]

stationary_tags = [line.strip() for line in open('experimental_tag.csv')]

seen_tags = {}


for i_tag in stationary_tags:
	seen_tags[int(i_tag)] = False
#print "keys : " , seen_tags.keys()

#connect to openbeacon
from pymongo import Connection
#this connects to the default port and stuff for the mogodb daemon
connection = Connection()
#change to openbeacon database
db = connection.openbeacon
#print the collections or "tables"
#print db.collection_names()

participant_name, participant_tag = re.split(',', participant_tags[0])
locationOfInterest = int(stationary_tags[0])
#db.edge.ensureIndex( { "time" : -1 } )
sightings = db.edge.find ( { "tag" : int(participant_tag) },{"_id":0}).sort("time")
#print "number of records is: ", sightings.count()

#start with my desk, 157
isParticipantAtLocation = False;
lastTimeSeenAtLocation = 0;
timeThreshold = 20
for sighting in sightings:
	#right and left tags	
	ltag = int(sighting['tag'][0])
	rtag = int(sighting['tag'][1])  
	power = int(sighting['power'])
	time = int(sighting['time'])
	humanReadableDate = datetime.datetime.fromtimestamp(time)
	print ltag, "," , rtag, "," , humanReadableDate, "," , power
	
		

	
		
		

#!/usr/local/bin/python

import pymongo
import datetime
participant_tags = [line.strip() for line in open('participant_tags.csv')]

stationary_tags = [line.strip() for line in open('desk_tags.csv')]

seen_tags = {}

print "participant tags:"
for participant in participant_tags:
	print participant

print "infrastructure tags:"
for i_tag in stationary_tags:
	print i_tag
	seen_tags[int(i_tag)] = False
#print "keys : " , seen_tags.keys()

#connect to openbeacon
from pymongo import Connection
#this connects to the default port and stuff for the mogodb daemon
connection = Connection()
#change to openbeacon database
db = connection.openbeacon
#print the collections or "tables"
print db.collection_names()

participant = int(participant_tags[0])
locationOfInterest = int(stationary_tags[0])
#db.edge.ensureIndex( { "time" : -1 } )
sightings = db.edge.find ( { "tag" : participant },{"_id":0}).sort("time")
print "number of records is: ", sightings.count()

for sighting in sightings:
	#right and left tags	
	ltag = int(sighting['tag'][0])
	rtag = int(sighting['tag'][1])  
	power = int(sighting['power'])
	time = int(sighting['time'])

	#if(ltag == locationOfInterest or rtag == locationOfInterest):
	humanReadableDate = datetime.datetime.fromtimestamp(time)
	print ltag, " " , rtag, " ", humanReadableDate

		

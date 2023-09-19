#!/usr/local/bin/python

import pymongo

participant_tags = [line.strip() for line in open('participant_tags.csv')]

stationary_tags = [line.strip() for line in open('stationary_tags_basement.csv')]

seen_tags = {}

print "participant tags:"
for participant in participant_tags:
	print participant

print "infrastructure tags:"
for i_tag in stationary_tags:
	print i_tag
	seen_tags[int(i_tag)] = False
print "keys : " , seen_tags.keys()

#connect to openbeacon
from pymongo import Connection
#this connects to the default port and stuff for the mogodb daemon
connection = Connection()
#change to openbeacon database
db = connection.openbeacon
#print the collections or "tables"
print db.collection_names()

#db.edge.ensureIndex( { "time" : -1 } )
sightings = db.edge.find ( { "tag" : 149 },{"_id":0}).sort("time")
print "number of records is: ", sightings.count()

for sighting in sightings:
	#right and left tags	
	#print sighting
	ltag = sighting['tag'][0]
	rtag = sighting['tag'][1]  
	power = sighting['power']
	time = sighting['time']
	#print ltag, " " , rtag, " " , power, " " , time
	if ltag in seen_tags:
		seen_tags[ltag] = True
		
	if rtag in seen_tags:
		seen_tags[rtag] = True
		
for tag in seen_tags:
	print tag ," ", seen_tags[tag]
	
		
		

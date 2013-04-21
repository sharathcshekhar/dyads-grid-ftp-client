#!/usr/bin/python
import pickle 
#
# This file does not run on its own! import to a python
# intereter and call ftp_list("path_name") to get the listing
# of that directory
#
# default database name is ftp_db
def ftp_loadDB(db_file="ftp_db"):
	fd = open(db_file, 'r')
	# ugliest way possible is to load the entir flat-file 
	# to memory and access it this is a proto, who cares?
	listing = pickle.load(fd)
	#close file after reading into memory
	fd.close()
	return listing

def ftp_list(path, db):
	#takes full path name
	for i in db[path]:
		print i

#END ftp_list

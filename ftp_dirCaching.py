#! /usr/bin/python

# This is a prototype to download the metadata information from
# a remote FTP server and store it in a local cache
#
# Better ways to update the cache based on timestamps and storing in 
# better database like MySQL has to be implemented

import ftplib
import pickle
import sys
import collections
import time

def main():
	tree = collections.OrderedDict();
	if len(sys.argv) != 3:
		print "Usage: python ftp_dirCaching server_addr db_file_name"
		return
	ftpAddr = sys.argv[1]
	dataBase = sys.argv[2]
	try:
		ftp = ftplib.FTP(ftpAddr)
		#Proto supports only anonymous logins
		ftp.login()
	except Exception, e:
		print e
		return
	recursive_listing(ftp, tree)
	print_dirTree(tree)
	fd = open(dataBase, 'w')
	#TODO: dump to a well defined database
	#dump to a flatfile using pickle API
	pickle.dump(tree, fd)
	fd.close()
	ftp.close()
	return

#to iterate is human, to recurse is divine :)
def recursive_listing(ftp, tree):
	#when first called from main() the pwd will be set to '/'
	#recurse from there
	ls = []
	no_of_tries = 0
	successful = False
	while not successful:
		successful = True
		try:
			ftp.dir(ls.append)
			#ugly way of doing it!
			nlst = ftp.nlst()
			tree[ftp.pwd()] = ls
		except ftplib.error_perm, e:
			#Throws error_prem exception if the dir is empty
			print e, "No permission on dir/empty dir"
			return;
		except ftplib.error_temp, e:
			print e, "Temporary network failure"
			#might be a tmp network error, sleep for 1 sec and try again upto 5 times
			no_of_tries += 1
			if no_of_tries < 5:
				time.sleep(1)
				successful = False
			else:
				sys.exit("Network error")
		except Exception, e:
			#Any other exception, log and continue
			print e
			return
	
	for i in range(len(ls)):
		#if the first char is 'd', the file is a directory
		if ls[i][0] == 'd':
			filename = getFilenameFromList(ls[i], nlst)
			if filename == None:
				#directory not found, something has gone wrong
				continue
			try:
				#FTP is connection oriented!
				#save the pwd and restore it after the recursive calls return
				path = ftp.pwd()
				print "present working directory", path, "subdir", filename 
				ftp.cwd(filename)
				recursive_listing(ftp, tree)
				ftp.cwd(path)
			except ftplib.error_perm, e:
				print e, "No permissions for ", filename
				continue
			except Exception, e:
				#Any other exception, log and continue
				print e
				continue
	return

#ultra-inefficient, ugly, shamefully ugly
#doesn't work for links
def getFilenameFromList(long_ls, nlst):
	for i in nlst:
		if long_ls.endswith(i):
			return i 
	return None
						
def print_dirTree(tree):
	for key in tree:
		print key, ":"
		for i in tree[key]:
			print i;
	return

if __name__ == "__main__":
	main()

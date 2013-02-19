#! /usr/bin/python

# This is a prototype to download the metadata information from
# a remote FTP server and store it in a local cache
#
# Better ways to update the cache based on timestamps and storing in 
# better database like MySQL has to be implemented

import ftplib, pickle, sys, collections

def main():
	tree = collections.OrderedDict();
	if len(sys.argv) != 3:
		print "Usage: python ftp_dirCaching server_addr db_file_name"
		return
	ftpAddr = sys.argv[1]
	dataBase = sys.argv[2]
	ftp = ftplib.FTP(ftpAddr)
	#Proto supports only anonymous logins
	ftp.login()
	recursive_listing(ftp, tree)
	for key in tree:
		print key, ":"
		for i in tree[key]:
			print i;
	
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
	ftp.dir(ls.append)
	try:
		#ugly way of doing it!
		nlst = ftp.nlst()
	except ftplib.error_perm:
		#Throws error_prem exception if the dir is empty
		print "No permission on dir/empty dir"
		return;
	tree[ftp.pwd()] = ls
	print "nlst len", len(nlst), "ls len", len(ls)
	for i in range(len(ls)):
		#if the first char is "d", the file is a directory
		#probably not the best way to do it. But works for proto :)
		if ls[i][0] == 'd':
			try:
				#FTP is connection oriented!
				#save the pwd and restore it after the recursive calls return
				path = ftp.pwd()
				print "present working directory", path 
				filename = ""
				#ultra-inefficient, ugly, shamefully ugly
				#doesn't work for links
				found = False
				for j in ls:
					for k in nlst:
						if ls[i].endswith(k):
							filename = k;	
							found = True
					if found == True:
						break
				
				if found == True:
					print "changing to dir", filename
					ftp.cwd(filename)
				else:
					continue
			except ftplib.error_perm:
				print "No permissions for ", filename
				continue
			recursive_listing(ftp, tree)
			#restore the working directory
			#we can probably do a cwd('..')
			ftp.cwd(path)
	return

if __name__ == "__main__":
	main()

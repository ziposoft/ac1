#!/Python27/python
#!/usr/bin/python
# Import modules for CGI handling 




import cgi, cgitb, os





def zerror():
	

	return

# Function definition is here
def savefile( name,path ):
	try:
		if(path == None):
			print("remove!")
			os.remove(name)
			return True;
	
		f = open(name, 'w')
		f.write(path)
		f.close()
		return True;
	except os.error as e:
		print ("I/O error({0}): {1}".format(e.errno, e.strerror))
		pass
		
	except IOError as e:
		print ("I/O error({0}): {1}".format(e.errno, e.strerror))
		pass
	return False;






# Create instance of FieldStorage 
form = cgi.FieldStorage() 



# Get data from fields
name = form.getvalue('name')
path  = form.getvalue('path')


print ('Content-type:text/html\r\n\r\n'
 "<html>"
 "<head>"
 "<title>Hello - Second CGI Program</title>")

print ( "</head><body><pre>")


cgi.print_arguments();

#cgi.print_environ();
cgi.print_directory();


if (name == None):
	print ('no name given')
	exit
else:
	if(savefile(name,path)):
		print ("saved!")
	else:
		print("error!")



print ("</pre></body>")
print ("</html>")




#!c:/Python27/python
# Import modules for CGI handling 
import cgi, cgitb, os


def zerror():
	

	return

# Function definition is here
def savefile( name,path ):
	try:
		if(path == None):
			os.remove(name)
		return
	
		f = open(name, 'w')
		f.write(path)
		f.close()
	except os.error as e:
		#print "I/O error({0}): {1}".format(e.errno, e.strerror)
		pass
		
	except IOError as e:
		#print "I/O error({0}): {1}".format(e.errno, e.strerror)
		pass
	return


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
cgi.print_environ();
cgi.print_directory();


if (name == None):
	print ('no name given')
	exit
else:
	savefile(name,path)
	print ("saved!")



print ("</pre></body>")
print ("</html>")

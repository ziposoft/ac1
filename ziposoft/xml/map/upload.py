#!c:/Python27/python.exe
#!/usr/bin/python
# Import modules for CGI handling 




import cgi, cgitb, os


dbg = None

def dbgprint(x):
	if(dbg):
		print(x)
	return

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
dbg = form.getvalue('dbg')
path  = form.getvalue('path')
if(name):
	name="../data/"+os.path.basename(name)



print ('Content-type:text/html\r\n\r\n'
	"<html>"
	"<head>"
	"<title>Hello - Second CGI Program</title></head><body><pre>")
cgi.print_arguments();
cgi.print_environ();
cgi.print_directory();






if (name == None):
	print ('no name given')
	
else:
	if(savefile(name,path)):
		print ("saved!")
	else:
		print("error!")



print ("</pre></body></html>")




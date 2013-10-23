#!/usr/bin/python
# Import modules for CGI handling 




import cgi, cgitb, os


dbg = None

def dbgprint(x):
	if(dbg):
		print(x)
	return

def listroutes():
	list=os.listdir("../map/routes/");
	str = '';
	print ('Content-type:text/html\r\n\r\n');
	for file in list:
		str += file;
		str += ';';
	print (str);
	return


def zerror():
	

	return

# Function definition is here
def savefile( name,path ):
	try:
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
act = form.getvalue('act')
name = form.getvalue('name')
dbg = form.getvalue('dbg')
path  = form.getvalue('path')

if(act == 'list'):
	listroutes()
	exit()

if(act == 'debug'):
	print ('Content-type:text/html\r\n\r\n'
		"<html>"
		"<head>"
		"<title>Hello - Second CGI Program</title></head><body><pre>")
	cgi.print_arguments();
	cgi.print_environ();
	cgi.print_directory();
	exit()

if(act == 'save'):
	dbgprint('Content-type:text/html\r\n\r\n');
	if (name == None):
		dbgprint ("no name!")
		exit();
	if(name):
		name="../map/routes/"+os.path.basename(name)
	if(savefile(name,path)):
		dbgprint ("saved!")
	else:
		dbgprint("error!")
	exit();


print ('Content-type:text/html\r\n\r\n'
	"<html>"
	"<head>"
	"<title>Trail Map</title></head><body><pre>")

print ("list, debug, save</pre></body></html>")

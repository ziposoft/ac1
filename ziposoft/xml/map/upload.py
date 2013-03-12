#!c:/Python27/python
# Import modules for CGI handling 
import cgi, cgitb 


# Function definition is here
def savefile( name,path ):
   "This prints a passed string into this function"
   
   return;


# Create instance of FieldStorage 
form = cgi.FieldStorage() 



# Get data from fields
name = form.getvalue('name')
path  = form.getvalue('path')
print "Content-type:text/html\r\n\r\n"
print "<html>"
print "<head>"
print "<title>Hello - Second CGI Program</title>"
print "</head>"
print "<body>"

if (name == 'None') or (path == 'None'):
    printf "no name given"
    exit
else:
    savefile(name,path)
    printf "saved!"



print "</body>"
print "</html>"
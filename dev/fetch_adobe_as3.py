import sys
import urllib

print(sys.argv[1])

pagehandler = urllib.urlopen(sys.argv[1])

text = pagehandler.read();

def remove_html(text):
	tag_start = text.find("<")
	tag_end = text.find(">")
	
	while(tag_start > -1 and tag_end > -1):
		text = (text[0:tag_start] + text[tag_end+1:len(text)])
		tag_start = text.find("<")
		tag_end = text.find(">")
	
	return text

def print_return(type):
	if(type == "Boolean"):
		print("{ return false; }"),
	
	elif(type == "int"):
		print("{ return 0; }"),
	
	elif(type == "Number"):
		print("{ return 0; }"),
	
	elif(type == "String"):
		print("{ return \"\"; }"),
	
	elif(type == "void" or type == ""):
		print("{}"),
	
	else:
		print("{ return null; }"),

def print_expression(start_text, end_text, attribs = False):
	decl_start = text.find(start_text);
	decl_end = text.find("</code>", decl_start);

	while(decl_start > -1 and decl_end > -1):
		decl_start += len(start_text)
		decl = remove_html(text[decl_start:decl_end]);
		if(len(decl) > 0):
			if(attribs):
				if(decl.find("(") < 0):
					sys.stdout.write("p")
					print(decl),
					print("")
			else:
				print(decl),
				return_delim = decl.rfind(":")
				if(return_delim < 0):
					return_type = ""
				else:
					return_type = decl[return_delim+1:len(decl)]
				print_return(return_type);
				print("")

		decl_start = text.find(start_text, decl_end+1);
		decl_end = text.find("</code>", decl_end+1);

print("/* Constructor */")
print_expression("Constructor</td></tr></table><div class=\"detailBody\"><code>", "</code>")
print("")
print("/* attributes */")
print_expression("property</td></tr></table><div class=\"detailBody\"><code>p", "</code>", True);
print_expression("</td></tr></table><div class=\"detailBody\"><code>p", "</code>", True);
print("")
print("/* getters & setters */")
print_expression("Implementation </span><br/><code>&nbsp;&nbsp;&nbsp;&nbsp;", "</code>");
print("")
print("/* methods */")
print_expression("</td></tr></table><div class=\"detailBody\"><code> ", "</code>");

pagehandler.close()

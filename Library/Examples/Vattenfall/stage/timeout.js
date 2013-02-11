function replace (u, d) {
	if (!d) d = document;
	if (!u) u = d.location;
	d.location.href = u;
}
function timeout (t, u, d) { //v3.0
	if (!d) d = document;
	if (!u) u = d.location;
	timeOut = eval('window.setTimeout("replace(\\"'+u+'\\")",'+t+' * 1000);');	
	return;
}

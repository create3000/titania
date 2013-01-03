<?php 
	header ("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
	header ("Last-Modified: " . gmdate ("D, d M Y H:i:s") . " GMT");
	header ("Cache-Control: no-cache, must-revalidate");
	header ("Pragma: no-cache");

	header ('Content-Type: text/html');
?>
<BASE HREF="http://vr.create3000.tv/vrml/models/scenes/mikroben/">
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">
<HTML>
<HEAD>
    <META NAME="Generator" CONTENT="Cosmo Create 1.0.3">
    <TITLE>Mikroben</TITLE>

<SCRIPT TYPE="text/javascript" LANGUAGE="JavaScript" SRC="/scripts/openWindow.js"></SCRIPT>
<SCRIPT TYPE="text/javascript" LANGUAGE="JavaScript" SRC="/scripts/browserCheck.js"></SCRIPT>

</HEAD>
<BODY BGCOLOR="#000000">

<SCRIPT TYPE="text/javascript" LANGUAGE="JavaScript" SRC="/scripts/vb_VRMLDetect.js"></SCRIPT>
<SCRIPT TYPE="text/javascript" LANGUAGE="JavaScript"><!--

if (is_cosmo) {
	document.write('<TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" HEIGHT="95%" WIDTH="100%">');
	document.write('<TR><TD ALIGN="CENTER" VALIGN="MIDDLE">');
	document.write('<EMBED TYPE="model/vrml" SRC="game-index.wrl" WIDTH="744" HEIGHT="558" BORDER="0">');
	document.write('</TD></TR>');
	document.write('</TABLE>');
} else {
	w=400; h=300;
	openWindow(
		is_mac ? '/vrml/cosmo/mac.html' : '/vrml/cosmo/win.html',
		'CosmoPlayerInstall',
		'left='+centerX(w)+',top='+centerY(h)+',width='+w+',height='+h
	);
	location = 'http://www.das-grossmaul.de/home/inhalt_news.html';
//	history.go(-1);
}

//--></SCRIPT>

</BODY>
</HTML>

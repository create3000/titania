/* SiteCatalyst code version: H.17.
Copyright 1997-2008 Omniture, Inc. More info available at
http://www.omniture.com */
/************************ ADDITIONAL FEATURES ************************
     Plugins
*/

var s_account="nvnvidiacomdev"
var s=s_gi(s_account)
s.dynamicAccountSelection=true
if (window.location.href == "http://www.nvidia.com.br") {
 s.dynamicAccountMatch="http://br.nvidia.com";
}
else if (window.location.href == "http://www.nvidia.com.tw") {
 s.dynamicAccountMatch="http://tw.nvidia.com";
}
else if (window.location.href == "http://www.nvidia.com.mx") {
 s.dynamicAccountMatch="nvidiala";
}
else if (window.location.href == "http://www.nvidia.in") {
 s.dynamicAccountMatch="http://www.nvidia.co.in";
}
else if (window.location.href == "http://kr.nvidia.com") {
 s.dynamicAccountMatch="http://www.nvidia.co.kr";
}
else if (window.location.href == "http://th.nvidia.com") {
s.dynamicAccountMatch="http://www.nvidia.co.th";
}
else if (window.location.href == "http://cn.geforce.com") {
s.dynamicAccountMatch="http://www.geforce.cn";
}
else if (window.location.href == "http://www.geforce.com.tw") {
 s.dynamicAccountMatch="http://tw.geforce.com";
}
else if (window.location.href == "http://jp.tegrazone.com") {
s.dynamicAccountMatch="http://www.tegrazone.jp";
}
else {
 s.dynamicAccountMatch=window.location.href
s.dynamicAccountList="nvidiabr=http://www.nvidia.com.br; nvidiatw=http://www.nvidia.com.tw; nvidiala=http://www.nvidia.com.mx; nvidiatr=http://www.nvidia.com.tr; nvidiaus=http://www.nvidia.com; nvidiabr=http://br.nvidia.com; nvidiacn=http://www.nvidia.cn; nvidiacn=http://cn.nvidia.com; nvidiade=http://www.nvidia.de; nvidiaes=http://www.nvidia.es; nvidiaes=http://es.nvidia.com; nvidiafr=http://www.nvidia.fr; nvidiain=http://www.nvidia.in; nvidiain=http://www.nvidia.co.in; nvidiait=http://www.nvidia.it; nvidiajp=http://www.nvidia.co.jp; nvidiajp=http://jp.nvidia.com; nvidiakr=http://www.nvidia.co.kr; nvidiala=http://ven.nvidia.com; nvidiala=http://la.nvidia.com; nvidiala=http://arg.nvidia.com; nvidiala=http://chl.nvidia.com; nvidiala=http://clm.nvidia.com; nvidiapl=http://www.nvidia.pl; nvidiaru=http://www.nvidia.ru; nvidiasg=http://sg.nvidia.com; nvidiatw=http://www.nvidia.com.tw; nvidiatw=http://tw.nvidia.com; nvidiauk=http://www.nvidia.co.uk; nvidianzoneus=http://www.nzone.com; nvidianzonede=http://de.nzone.com; nvidianzonees=http://es.nzone.com; nvidianzonefr=http://fr.nzone.com; nvidianzonefr=http://www.nzone.fr; nvidianzonein=http://in.nzone.com; nvidianzonein=http://www.nzone.in; nvidianzoneit=http://it.nzone.com; nvidianzoneuk=http://uk.nzone.com; nvidianzoneus=nzone.com; nvidiaslizoneus=http://www.slizone.com; nvidiaslizonecn=http://cn.slizone.com; nvidiaslizonecn=http://www.slizone.cn; nvidiaslizonede=http://de.slizone.com; nvidiaslizonees=http://es.slizone.com; nvidiaslizonefr=http://fr.slizone.com; nvidiaslizonein=http://in.slizone.com; nvidiaslizoneit=http://it.slizone.com; nvidiaslizonejp=http://jp.slizone.com; nvidiaslizonekr=http://kr.slizone.com; nvidiaslizonepl=http://pl.slizone.com; nvidiaslizoneru=http://www.slizone.ru; nvidiaslizoneru=http://ru.slizone.com; nvidiaslizonesg=http://sg.slizone.com; nvidiaslizonetw=http://tw.slizone.com; nvidiaslizoneuk=http://uk.slizone.com; nvidianvidiausstore=http://store.nvidia.com; nvidiaspeakvisual=http://www.speakvisual.com; nvidiadeveloperus=http://developer.nvidia.com; nvidiadeveloperus=https://developer.nvidia.com; nvidiadeveloperus=http://nvdeveloper.nvidia.com; nvidianvinfo=http://em3.nvidia.com; nvidiauk=http://www.nvidia.se; nvidiablogs=http://blogs.nvidia.com; nvidiaintelsinsides=http://www.intelsinsides.com; nvnvidiacomdev=http://developer.download.nvidia.com; nvnvidiacomdev=http://developer.download.nvidia.com; nvidianvidiaasia=http://www.nvidia.asia; nvidiageforceuk=http://uk.geforce.com; nvidiageforceuk=http://www.geforce.co.uk; nvidiaus=http://pressroom.nvidia.com; nvidiath=http://www.nvidia.co.th; nvidiageforcecn=http://www.geforce.cn; nvidiageforcecn=http://cn.geforce.com; nvidiageforcetw=http://tw.geforce.com; nvidiageforcetw=http://www.geforce.com.tw; nvidiageforceus=http://www.geforce.com; nvidiageforceus=http://geforce.com; nvidiagputechconf=http://www.gputechconf.com; nvidiaforums=http://forums.nvidia.com; nvidiategrazonede=http://de.tegrazone.com; nvidiategrazonees=http://es.tegrazone.com; nvidiategrazonefr=http://fr.tegrazone.com; nvidiategrazonein=http://in.tegrazone.com; nvidiategrazoneit=http://it.tegrazone.com; nvidiategrazonepl=http://pl.tegrazone.com; nvidiategrazoneru=http://ru.tegrazone.com; nvidiategrazoneuk=http://uk.tegrazone.com; nvidiategrazonecn=http://cn.tegrazone.com; nvidiategrazonejp=http://www.tegrazone.jp; nvidiategrazonekr=http://kr.tegrazone.com; nvidiategrazonetw=http://tw.tegrazone.com; nvidiategrazonebr=http://br.tegrazone.com; nvidiategrazonela=http://la.tegrazone.com; nvidiategrazoneus=http://www.tegrazone.com; nvidiategrazoneus=http://tegrazone.com; nvidiagputechconfcn=http://www.gputechconf.cn; nvidiagputechconfjp=http://www.gputechconf.jp; nvidiagputechconfreg=https://registration.gputechconf.com; nvidiategrazonela=http://ar.tegrazone.com; nvidiategrazonela=http://cl.tegrazone.com; nvidiategrazonela=http://co.tegrazone.com; nvidiategrazonela=http://mx.tegrazone.com; nvidiategrazonela=http://ve.tegrazone.com; nvidiacudadocs=http://docs.nvidia.com; nvidia3dvisionlive=http://www.3dvisionlive.com; nvidia3dvisionlive=http://photos.3dvisionlive.com; nvidiaforums=http://forums.geforce.com; nvidiaforums=https://forums.geforce.com; nvidiadevtalk=http://devtalk.nvidia.com;  nvidiadevtalk=https://devtalk.nvidia.com; nvidiashield=http://shield.nvidia.com;"
}
/*s.dynamicAccountMatch=window.location.href*/
/************************** CONFIG SECTION **************************/
/* You may add or alter any code config here. */
/* Conversion Config */
s.currencyCode="USD"
/* Link Tracking Config */
// Thu Jun 27 13:34:27 2013 - enabling the following two lines breaks our quicksurvey functionality
//s.trackDownloadLinks=false
//s.linkDownloadFileTypes="exe,zip,wav,mp3,mov,mpg,avi,wmv,pdf,doc,docx,xls,xlsx,ppt,pptx,gz,run,tgz,rar,txt,pkg,m4v,swf"
s.trackExternalLinks=true
s.trackInlineStats=true
s.linkInternalFilters="javascript:,nvidia.com,slizone.com,nzone.com,nvidia.cn,nvidia.de,nvidia.es,nvidia.fr,nvidia.in,nvidia.co.in,nvidia.it,nvidia.co.jp,nvidia.pl,nvidia.ru,nvidia.co.uk,nzone.fr,nzone.in,slizone.cn,slizone.ru,nvidia.se,nvidia.com.tw,speakvisual.com,intelsinsides.com,geforce.com,uk.geforce.com,nvidia.co.th,tegrazone.com,tegrazone.jp,gputechconf.com,gputechconf.cn"
s.linkLeaveQueryString=true
s.linkTrackVars="None"
s.linkTrackEvents="None"

/* For GeForce html tracking */
if(window.location.hostname == 'www.geforce.com' || window.location.hostname == 'nvidianews.nvidia.com')
s.pageName = window.location.href;

/*CookieDomainPeriod*/
var d = window.location.hostname;
if(d.indexOf('.com.br')>-1||d.indexOf('.co.in')>-1||d.indexOf('.co.jp')>-1||d.indexOf('.com.mx')>-1||d.indexOf('.com.tw')>-1||d.indexOf('.co.uk')>-1||d.indexOf('.co.kr')>-1||d.indexOf('.co.th')>-1)
s.cookieDomainPeriods="3";
else
s.cookieDomainPeriods="2";


/* Page Name Plugin Config */
s.siteID=""            // leftmost value in pagename
s.defaultPage=""       // filename to add when none exists
s.queryVarsList="partner,CMP,rssid,cid,gclid,seid,prid,game"     // query parameters to keep
s.pathExcludeDelim=";" // portion of the path to exclude
s.pathConcatDelim=""   // page name component separator
s.pathExcludeList=""   // elements to exclude from the path



/* Plugin Config */

/*Generic Event tracking*/
function genericTrackEvent(obj, Category, Action, label, site )
{
	//Pass on the parameters to GA
	try {
		/*var Category = Category;
		var Action = Action;
		var label = label;*/
		//pageTracker._trackEvent(Category, Action, label);
		_gaq.push(['_trackEvent',Category,Action,label]);
	}
	catch(err) {}

	//Below lines are for Driver matrix tracking
	//var DrvrMtx=s.getValOnce(s.eVar18,'s_drvrmtx',0);
	if(Category=='Drvr Agree'){
	//s.events=s.apl(s.events,"event18",",",1);
	s.events="event18";
	}
	//end of Driver matrix tracking

	//Pass on the parameters to Omniture
	if(Category != '') var customid = Category;
	else customid = '';
	if(Action != '') var eventName = " | " + Action;
	else eventName = '';
	if(label != '') var lbl =  " | " + label;
	else lbl = '';
	if(site != '') var url =  " | " + site;
	else url = '';

	omnTrack= customid + eventName + lbl + url;
	customTracking(obj, omnTrack );
}

function featuresTracking(obj, featureId) {
var link_Name = featureId;
s.linkTrackVars='prop4';
//s.linkTrackEvents='event1';
if(link_Name){
s.prop4 = link_Name;
//s.eVar4 = s.prop4;
}
s.tl(obj,'o',link_Name);
}


/* tpl tracking */
function customTracking(obj, customId) {
var link_Name = customId;
s.linkTrackVars='prop4';
//s.linkTrackEvents='event1';
if(link_Name){
s.prop4 = link_Name;
//s.eVar4 = s.prop4;
}
s.tl(obj,'o',link_Name);
}

/* Videos Tracking */
function videosTracking(obj, videoId) {
var link_Name = videoId;

s.tl(obj,'o',link_Name);
}

/* Driver Matrix Tracking */
function DrvrMtxTracking(obj, DrvrMtxOpt) {
var DrvrMtxSel = DrvrMtxOpt;
s.linkTrackVars='prop18';
if(DrvrMtxSel){
	s.linkTrackVars='prop18';
	s.linkTrackEvents='event18';
	s.prop18 = DrvrMtxSel;
	s.eVar18 = DrvrMtxSel;
}
s.tl(obj,'o',DrvrMtxSel);
}


/* DynamicObjectIDs config */
function s_getObjectID(o) {
var ID=o.href;
return ID;
}
s.getObjectID=s_getObjectID;

s.usePlugins=true
function s_doPlugins(s) {
	s.setupDynamicObjectIDs();


	/* Add calls to plugins here */

/* Plugin Example: getPagename v2.1*/
	if(!s.pageType && !s.pageName)
		s.pageName=s.getPageName();
		s.prop30=window.location.href;

/* Plugin Examples: downloadLinkHandler  */
	/*s.downloadLinkHandler("pdf,ppt,doc")*/
	s.downloadLinkHandler()

	/* Plugin Example: ExitLinkHandler */

s.exitLinkHandler("javascript:,nvidia.com,slizone.com,nzone.com,www.slizone.com,www.nvidia.com,www.nzone.com,nvidia.cn,nvidia.de,nvidia.es,nvidia.fr,nvidia.in,nvidia.co.in,nvidia.it,nvidia.co.jp,nvidia.co.kr,nvidia.pl,nvidia.ru,nvidia.co.uk,nzone.fr,nzone.in,slizone.cn,slizone.ru,nvidia.se,nvidia.com.tw,speakvisual.com,intelsinsides.com,geforce.com,uk.geforce.com,nvidia.co.th,tegrazone.com,tegrazone.jp,gputechconf.com,gputechconf.cn")

/* Plugin Example: getNewRepeat 1.0 */
	s.prop15=s.getNewRepeat();

	s.prop3=navigator.userAgent;


	/* External Campaigns */
	if(!s.campaign)
		s.campaign=s.getQueryParam('cid,CMP,id,siteid,pageid,adid,creativeid,rssid,gclid,seid,prid,game');

	/* Plugin Example: setCustomPath v1.0 */
if(!s.campaign) s.campaign=s.getQueryParam('cid')
s.campaign=s.getValOnce(s.campaign,'s_campaign',0)
s.prop19=s.setCustomPath(s.campaign,'s_campaign','ExtCmp=',s.pageName);

/*Campaign ID into s.prop for Visits and Unique Visitors and into eVar*/
if(s.campaign){
	s.eVar20=s.prop20=s.campaign;
	}


/*Get and Persist Campaign ID and concat with Page Name - Place into s.prop for Path*/
var getCampaign = s.getAndPersistValue(s.campaign,'s_cp_persist',0);
var NoCampaignPageName = 'Non CMP : ' + s.pageName;
var CampaignandPage= getCampaign + ' : ' + s.pageName;
if(!getCampaign){
	s.prop21=NoCampaignPageName;
} else {
	s.prop21=CampaignandPage;
}

	/* Site Search
	if(s.prop6){
		s.prop6=s.prop6.toLowerCase();
		s.eVar4=s.prop6;
		var t_search=s.getValOnce(s.eVar4,'ev4',0);
		if(t_search){
			s.events=s.apl(s.events,"event5",",",2);
		}
	}


*/

	/* Based on the name of the page - choose query param to populate IS (search term), set to lowercase, copy to evar, and set deduped event */
if(s.pageName=='object/nv_search_us.html' || 'object/nv_search_sg.html' || 'object/nv_search_jp.html' || 'object/nv_search_cn.html' || 'object/nv_search_tw.html' || 'object/nv_search_kr.html' || 'object/nv_search_uk.html' || 'object/nv_search_de.html' || 'object/nv_search_fr.html' || 'object/nv_search_it.html' || 'object/nv_search_la.html ' || 'object/nv_search_br.html' || 'object/nv_search_in.html' || 'object/nv_search_pl.html' || 'object/nv_search_ru.html' || 'object/nv_search_es.html' || 'object/search_cn.html' || 'object/search_fr.html ' || 'object/search_de.html ' || 'object/search_in.html ' || 'object/search_it.html ' || 'object/search_jp.html' || 'object/search_kr.html' || 'object/search_pl.html' || 'object/search_ru.html' || 'object/search_sg.html' || 'object/search_es.html ' || 'object/search_tw.html' || 'object/search_uk.html ' || 'object/search_us.html' ){
	s.prop31=s.getQueryParam('q');
	s.prop31=s.prop31.toLowerCase();
	s.eVar31=s.prop31;
	var t_search=s.getValOnce(s.eVar31,'ev1',0);
	if(t_search){
		s.events=s.apl(s.events,'event1',',',1);}
}else if(s.pageName=='Search/SearchResults'){
s.prop31=s.getQueryParam('keywords');
s.prop31=s.prop31.toLowerCase();
	s.eVar31=s.prop31;
	var t_search=s.getValOnce(s.eVar31,'ev1',0);
	if(t_search){
		s.events=s.apl(s.events,'event1',',',1);}
}
}
s.doPlugins=s_doPlugins
/************************** PLUGINS SECTION *************************/
/* You may insert any plugins you wish to use here.                 */




/*********************************************************************
 *
 *  Plugins
 *
 ********************************************************************/
/*
 * Plugin: downloadLinkHandler 0.5 - identify and report download links
 */
s.downloadLinkHandler=new Function("p",""
+"var s=this,h=s.p_gh(),n='linkDownloadFileTypes',i,t;if(!h||(s.linkT"
+"ype&&(h||s.linkName)))return '';i=h.indexOf('?');t=s[n];s[n]=p?p:t;"
+"if(s.lt(h)=='d')s.linkType='d';else h='';s[n]=t;return h;");

/*
 * Plugin: setCustomPath v1.0
 */
s.setCustomPath=new Function("v","c","p","pt",""
+"var s=this,pt=pt?pt:'no value sent',k=s.c_r(c);if(v){return p+v+':'"
+"+pt} else return k?pt:'';");

/*
 * Plugin: exitLinkHandler 0.5 - identify and report exit links
 */
s.exitLinkHandler=new Function("p",""
+"var s=this,h=s.p_gh(),n='linkInternalFilters',i,t;if(!h||(s.linkTyp"
+"e&&(h||s.linkName)))return '';i=h.indexOf('?');t=s[n];s[n]=p?p:t;h="
+"s.linkLeaveQueryString||i<0?h:h.substring(0,i);if(s.lt(h)=='e')s.li"
+"nkType='e';else h='';s[n]=t;return h;");

/*
 * Plugin: getNewRepeat 1.0 - Return whether user is new or repeat
 */
s.getNewRepeat=new Function(""
+"var s=this,e=new Date(),cval,ct=e.getTime(),y=e.getYear();e.setTime"
+"(ct+30*24*60*60*1000);cval=s.c_r('s_nr');if(cval.length==0){s.c_w("
+"'s_nr',ct,e);return 'New';}if(cval.length!=0&&ct-cval<30*60*1000){s"
+".c_w('s_nr',ct,e);return 'New';}if(cval<1123916400001){e.setTime(cv"
+"al+30*24*60*60*1000);s.c_w('s_nr',ct,e);return 'Repeat';}else retur"
+"n 'Repeat';");

/*
 * Plugin: linkHandler 0.5 - identify and report custom links
 */
s.linkHandler=new Function("p","t",""
+"var s=this,h=s.p_gh(),i,l;t=t?t:'o';if(!h||(s.linkType&&(h||s.linkN"
+"ame)))return '';i=h.indexOf('?');h=s.linkLeaveQueryString||i<0?h:h."
+"substring(0,i);l=s.pt(p,'|','p_gn',h.toLowerCase());if(l){s.linkNam"
+"e=l=='[['?'':l;s.linkType=t;return h;}return '';");
s.p_gn=new Function("t","h",""
+"var i=t?t.indexOf('~'):-1,n,x;if(t&&h){n=i<0?'':t.substring(0,i);x="
+"t.substring(i+1);if(h.indexOf(x.toLowerCase())>-1)return n?n:'[[';}"
+"return 0;");

/*
 * Plugin: getQueryParam 2.1 - return query string parameter(s)
 */
s.getQueryParam=new Function("p","d","u",""
+"var s=this,v='',i,t;d=d?d:'';u=u?u:(s.pageURL?s.pageURL:s.wd.locati"
+"on);if(u=='f')u=s.gtfs().location;while(p){i=p.indexOf(',');i=i<0?p"
+".length:i;t=s.p_gpv(p.substring(0,i),u+'');if(t)v+=v?d+t:t;p=p.subs"
+"tring(i==p.length?i:i+1)}return v");
s.p_gpv=new Function("k","u",""
+"var s=this,v='',i=u.indexOf('?'),q;if(k&&i>-1){q=u.substring(i+1);v"
+"=s.pt(q,'&','p_gvf',k)}return v");
s.p_gvf=new Function("t","k",""
+"if(t){var s=this,i=t.indexOf('='),p=i<0?t:t.substring(0,i),v=i<0?'T"
+"rue':t.substring(i+1);if(p.toLowerCase()==k.toLowerCase())return s."
+"epa(v)}return ''");
/*
 * Plugin: getValOnce 0.2 - get a value once per session or number of days
 */
s.getValOnce=new Function("v","c","e",""
+"var s=this,k=s.c_r(c),a=new Date;e=e?e:0;if(v){a.setTime(a.getTime("
+")+e*86400000);s.c_w(c,v,e?a:0);}return v==k?'':v");
/*
 * Utility Function: split v1.5 - split a string (JS 1.0 compatible)
 */
s.split=new Function("l","d",""
+"var i,x=0,a=new Array;while(l){i=l.indexOf(d);i=i>-1?i:l.length;a[x"
+"++]=l.substring(0,i);l=l.substring(i+d.length);}return a");
/*
 * Plugin Utility: apl v1.1
 */
s.apl=new Function("L","v","d","u",""
+"var s=this,m=0;if(!L)L='';if(u){var i,n,a=s.split(L,d);for(i=0;i<a."
+"length;i++){n=a[i];m=m||(u==1?(n==v):(n.toLowerCase()==v.toLowerCas"
+"e()));}}if(!m)L=L?L+d+v:v;return L");

/*
 * Plugin: getPageName v2.1 - parse URL and return
 */
s.getPageName=new Function("u",""
+"var s=this,v=u?u:''+s.wd.location,x=v.indexOf(':'),y=v.indexOf('/',"
+"x+4),z=v.indexOf('?'),c=s.pathConcatDelim,e=s.pathExcludeDelim,g=s."
+"queryVarsList,d=s.siteID,n=d?d:'',q=z<0?'':v.substring(z+1),p=v.sub"
+"string(y+1,q?z:v.length);z=p.indexOf('#');p=z<0?p:s.fl(p,z);x=e?p.i"
+"ndexOf(e):-1;p=x<0?p:s.fl(p,x);p+=!p||p.charAt(p.length-1)=='/'?s.d"
+"efaultPage:'';y=c?c:'/';while(p){x=p.indexOf('/');x=x<0?p.length:x;"
+"z=s.fl(p,x);if(!s.pt(s.pathExcludeList,',','p_c',z))n+=n?y+z:z;p=p."
+"substring(x+1)}y=c?c:'?';while(g){x=g.indexOf(',');x=x<0?g.length:x"
+";z=s.fl(g,x);z=s.pt(q,'&','p_c',z);if(z){n+=n?y+z:z;y=c?c:'&'}g=g.s"
+"ubstring(x+1)}return n");

/*
* Plugin Utility: Replace v1.0
*/
s.repl=new Function("x","o","n",""
+"var i=x.indexOf(o),l=n.length;while(x&&i>=0){x=x.substring(0,i)+n+x."
+"substring(i+o.length);i=x.indexOf(o,i+l)}return x");

/*
* DynamicObjectIDs v1.4: Setup Dynamic Object IDs based on URL
*/
s.setupDynamicObjectIDs=new Function(""
+"var s=this;if(!s.doi){s.doi=1;if(s.apv>3&&(!s.isie||!s.ismac||s.apv"
+">=5)){if(s.wd.attachEvent)s.wd.attachEvent('onload',s.setOIDs);else"
+" if(s.wd.addEventListener)s.wd.addEventListener('load',s.setOIDs,fa"
+"lse);else{s.doiol=s.wd.onload;s.wd.onload=s.setOIDs}}s.wd.s_semapho"
+"re=1}");
s.setOIDs=new Function("e",""
+"var s=s_c_il["+s._in+"],b=s.eh(s.wd,'onload'),o='onclick',x,l,u,c,i"
+",a=new Array;if(s.doiol){if(b)s[b]=s.wd[b];s.doiol(e)}if(s.d.links)"
+"{for(i=0;i<s.d.links.length;i++){l=s.d.links[i];c=l[o]?''+l[o]:'';b"
+"=s.eh(l,o);z=l[b]?''+l[b]:'';u=s.getObjectID(l);if(u&&c.indexOf('s_"
+"objectID')<0&&z.indexOf('s_objectID')<0){u=s.repl(u,'\"','');u=s.re"
+"pl(u,'\\n','').substring(0,97);l.s_oc=l[o];a[u]=a[u]?a[u]+1:1;x='';"
+"if(c.indexOf('.t(')>=0||c.indexOf('.tl(')>=0||c.indexOf('s_gs(')>=0"
+")x='var x=\".tl(\";';x+='s_objectID=\"'+u+'_'+a[u]+'\";return this."
+"s_oc?this.s_oc(e):true';if(s.isns&&s.apv>=5)l.setAttribute(o,x);l[o"
+"]=new Function('e',x)}}}s.wd.s_semaphore=0;return true");

/****************************** MODULES *****************************/
/* Module: Media */
s.m_Media_c="(`OWhilePlaying~='s_media_'+m._in+'_~unc^D(~;`E~m.ae(mn,l,\"'+p+'\",~){var m=this~o;w.percent=((w.off^e+1)/w`X)*100;w.percent=w.percent>1~o.'+f~=new ~o.Get~:Math.floor(w.percent);w.timeP"
+"layed=i.t~}`x p');p=tcf(o)~Time~x,x!=2?p:-1,o)}~if(~m.monitor)m.monitor(m.s,w)}~m.s.d.getElementsByTagName~ersionInfo~'^N_c_il['+m._in+'],~'o','var e,p=~else~i.to~=Math.floor(~}catch(e){p=~m.track~"
+"s.wd.addEventListener~.name~m.s.rep(~layState~||^8~Object~m.s.wd[f1]~^A+=i.t+d+i.s+d+~.length~parseInt(~Player '+~s.wd.attachEvent~'a','b',c~Media~pe='m~;o[f1]~m.s.isie~.current~);i.~p<p2||p-p2>5)~"
+".event=~m.close~i.lo~vo.linkTrack~=v+',n,~.open~){w.off^e=~;n=m.cn(n);~){this.e(n,~v=e='None';~Quick~MovieName()~);o[f~out(\"'+v+';~return~1000~i.lx~m.ol~o.controls~m.s.ape(i.~load',m.as~)}};m.~scr"
+"ipt';x.~,t;try{t=~Version()~n==~'--**--',~pev3~o.id~i.ts~tion~){mn=~1;o[f7]=~();~(x==~){p='~&&m.l~l[n])~:'')+i.e~':'E')+o~var m=s~!p){tcf~xc=m.s.~Title()~()/~7+'~+1)/i.l~;i.e=''~3,p,o);~m.l[n]=~Dat"
+"e~5000~;if~i.lt~';c2='~tm.get~Events~set~Change~)};m~',f~(x!=~4+'=n;~~^N.m_i('`c');m.cn=f`2n`5;`x `Rm.s.rep(`Rn,\"\\n\",''),\"\\r\",''),^9''^g`o=f`2n,l,p,b`5,i`8`U,tm`8^X,a='',x`ql=`Yl)`3!l)l=1`3n&"
+"&p){`E!m.l)m.l`8`U`3m.^K`k(n)`3b&&b.id)a=b.id;for (x in m.l)`Em.l[x]^J[x].a==a)`k(m.l[x].n`hn=n;i.l=l;i.p=m.cn(p`ha=a;i.t=0;^C=0;i.s`M^c`C^R`y`hlx=0;^a=i.s;`l=0^U;`L=-1;^Wi}};`k=f`2n`r0,-1^g.play=f"
+"`2n,o`5,i;i=m.e(n,1,o`hm`8F`2`Ii`3m.l){i=m.l[\"'+`Ri.n,'\"','\\\\\"')+'\"]`3i){`E`z==1)m.e(i.n,3,-1`hmt=^e`Cout(i.m,^Y)}}'`hm(^g.stop=f`2n,o`r2,o)};`O=f`2n`5^Z `0) {m.e(n,4,-1^4e=f`2n,x,o`5,i,tm`8^"
+"X,ts`M^c`C^R`y),ti=`OSeconds,tp=`OMilestones,z`8Array,j,d=^9t=1,b,v=`OVars,e=`O^d,`dedia',^A,w`8`U,vo`8`U`qi=n^J&&m.l[n]?m.l[n]:0`3i){w`Q=n;w`X=i.l;w.playerName=i.p`3`L<0)w`j\"OPEN\";`K w`j^H1?\"PL"
+"AY\":^H2?\"STOP\":^H3?\"MONITOR\":\"CLOSE\")));w`o`C`8^X^Gw`o`C.^e`C(i.s*`y)`3x>2||^i`z&&^i2||`z==1))) {b=\"`c.\"+name;^A = ^2n)+d+i.l+d+^2p)+d`3x){`Eo<0&&^a>0){o=(ts-^a)+`l;o=o<i.l?o:i.l-1}o`Mo)`3"
+"x>=2&&`l<o){i.t+=o-`l;^C+=o-`l;}`Ex<=2){i.e+=^H1?'S^M;`z=x;}`K `E`z!=1)m.e(n,1,o`hlt=ts;`l=o;`W`0&&`L>=0?'L'+`L^L+^i2?`0?'L^M:'')^Z`0){b=0;`d_o'`3x!=4`p`600?100`A`3`F`E`L<0)`d_s';`K `Ex==4)`d_i';`K"
+"{t=0;`sti=ti?`Yti):0;z=tp?m.s.sp(tp,','):0`3ti&&^C>=ti)t=1;`K `Ez){`Eo<`L)`L=o;`K{for(j=0;j<z`X;j++){ti=z[j]?`Yz[j]):0`3ti&&((`L^T<ti/100)&&((o^T>=ti/100)){t=1;j=z`X}}}}}}}`K{m.e(n,2,-1)^Z`0`pi.l`6"
+"00?100`A`3`F^W0`3i.e){`W`0&&`L>=0?'L'+`L^L^Z`0){`s`d_o'}`K{t=0;m.s.fbr(b)}}`K t=0;b=0}`Et){`mVars=v;`m^d=e;vo.pe=pe;vo.^A=^A;m.s.t(vo,b)^Z`0){^C=0;`L=o^U}}}}`x i};m.ae=f`2n,l,p,x,o,b){`En&&p`5`3!m."
+"l||!m.^Km`o(n,l,p,b);m.e(n,x,o^4a=f`2o,t`5,i=^B?^B:o`Q,n=o`Q,p=0,v,c,c1,c2,^Ph,x,e,f1,f2`1oc^h3`1t^h4`1s^h5`1l^h6`1m^h7`1c',tcf,w`3!i){`E!m.c)m.c=0;i`1'+m.c;m.c++}`E!^B)^B=i`3!o`Q)o`Q=n=i`3!^0)^0`8"
+"`U`3^0[i])`x;^0[i]=o`3!xc)^Pb;tcf`8F`2`J0;try{`Eo.v`H&&o`g`c&&^1)p=1`N0`B`3^O`8F`2`J0^6`9`t`C^7`3t)p=2`N0`B`3^O`8F`2`J0^6`9V`H()`3t)p=3`N0`B}}v=\"^N_c_il[\"+m._in+\"],o=^0['\"+i+\"']\"`3p==1^IWindo"
+"ws `c `Zo.v`H;c1`np,l,x=-1,cm,c,mn`3o){cm=o`g`c;c=^1`3cm&&c^Ecm`Q?cm`Q:c.URL;l=cm.dura^D;p=c`gPosi^D;n=o.p`S`3n){`E^88)x=0`3^83)x=1`3^81`T2`T4`T5`T6)x=2;}^b`Ex>=0)`4`D}';c=c1+c2`3`f&&xc){x=m.s.d.cr"
+"eateElement('script');x.language='j^5type='text/java^5htmlFor=i;x`j'P`S^f(NewState)';x.defer=true;x.text=c;xc.appendChild(x`v6]`8F`2c1+'`E^83){x=3;'+c2+'}^e`Cout(`76+',^Y)'`v6]()}}`Ep==2^I`t`C `Z(`"
+"9Is`t`CRegistered()?'Pro ':'')+`9`t`C^7;f1=f2;c`nx,t,l,p,p2,mn`3o^E`9`u?`9`u:`9URL^Gn=`9Rate^Gt=`9`CScale^Gl=`9Dura^D^Rt;p=`9`C^Rt;p2=`75+'`3n!=`74+'||`i{x=2`3n!=0)x=1;`K `Ep>=l)x=0`3`i`42,p2,o);`4"
+"`D`En>0&&`7^S>=10){`4^V`7^S=0}`7^S++;`7^j`75+'=p;^e`C`w`72+'(0,0)\",500)}'`e`8F`2`b`v4]=-^F0`e(0,0)}`Ep==3^IReal`Z`9V`H^Gf1=n+'_OnP`S^f';c1`nx=-1,l,p,mn`3o^E`9^Q?`9^Q:`9Source^Gn=`9P`S^Gl=`9Length^"
+"R`y;p=`9Posi^D^R`y`3n!=`74+'){`E^83)x=1`3^80`T2`T4`T5)x=2`3^80&&(p>=l||p==0))x=0`3x>=0)`4`D`E^83&&(`7^S>=10||!`73+')){`4^V`7^S=0}`7^S++;`7^j^b`E`72+')`72+'(o,n)}'`3`V)o[f2]=`V;`V`8F`2`b1+c2)`e`8F`2"
+"`b1+'^e`C`w`71+'(0,0)\",`73+'?500:^Y);'+c2`v4]=-1`3`f)o[f3]=^F0`e(0,0^4as`8F`2'e',`Il,n`3m.autoTrack&&`G){l=`G(`f?\"OBJECT\":\"EMBED\")`3l)for(n=0;n<l`X;n++)m.a(^K;}')`3`a)`a('on^3);`K `E`P)`P('^3,"
+"false)";
s.m_i("Media");

 /* Utility Function: p_gh*/
s.p_gh=new Function(""
+"var s=this;if(!s.eo&&!s.lnk)return '';var o=s.eo?s.eo:s.lnk,y=s.ot("
+"o),n=s.oid(o),x=o.s_oidt;if(s.eo&&o==s.eo){while(o&&!n&&y!='BODY'){"
+"o=o.parentElement?o.parentElement:o.parentNode;if(!o)return '';y=s."
+"ot(o);n=s.oid(o);x=o.s_oidt}}return o.href?o.href:'';");

/*
 * Utility Function: p_c
 */
s.p_c=new Function("v","c",""
+"var x=v.indexOf('=');return c.toLowerCase()==v.substring(0,x<0?v.le"
+"ngth:x).toLowerCase()?v:0");

/*
 * Plugin: getAndPersistValue 0.3 - get a value on every page
 */
s.getAndPersistValue=new Function("v","c","e",""
	+"var s=this,a=new Date;"
	+"e=e?e:0;"
	+"a.setTime(a.getTime()+e*86400000);"
	+"if(v)s.c_w(c,v,e?a:0);"
	+"return s.c_r(c);"
);



s.loadModule("Media")
s.Media.autoTrack=true
s.Media.trackVars="None"
s.Media.trackEvents="None"


/* WARNING: Changing any of the below variables will cause drastic
changes to how your visitor data is collected.  Changes should only be
made when instructed to do so by your account manager.*/
s.visitorNamespace="nvidia"
s.trackingServer="nsomniture.nvidia.com"
s.trackingServerSecure="omniture.nvidia.com"
s.dc=122

/************* DO NOT ALTER ANYTHING BELOW THIS LINE ! **************/
var s_code='',s_objectID;function s_gi(un,pg,ss){var c="=fun@6(~){`Ks=^S~$h ~.substring(~.indexOf(~;@t~';`Bt`t~=new Fun@6(~.toLowerCase()~s_c_il['+s^sn+']~};s.~`m@t~.length~.toUpperCase~=new Object~s"
+".wd~','~){@t~')q='~.location~var ~s.pt(~dynamicAccount~link~s.apv~='+@y(~)@tx^m!Object$eObject.prototype$eObject.prototype[x])~);s.~Element~.getTime()~=new Array~ookieDomainPeriods~s.m_~referrer~.p"
+"rotocol~=new Date~BufferedRequests~}c$s(e){~visitor~;@X^js[k],255)}~=''~javaEnabled~conne@6^M~@0c_i~Name~:'')~onclick~}@t~else ~ternalFilters~javascript~s.dl~@Os.b.addBehavior(\"# default# ~=parseF"
+"loat(~'+tm.get~=='~cookie~s.rep(~s.^T~track~o@0oid~browser~.parent~window~colorDepth~String~while(~.host~.lastIndexOf('~s.sq~s.maxDelay~s.vl_g~r=s.m(f)?s[f](~for(~s.un~s.eo~&&s.~parseInt(~t=s.ot(o)"
+"~j='1.~#4URL~lugins~dynamicVariablePrefix~document~Type~Sampling~s.rc[un]~Download~Event~');~this~tfs~resolution~s.c_r(~s.c_w(~s.eh~s.isie~s.vl_l~s.vl_t~Height~t,h){t=t?t~tcf~isopera~ismac~escape(~"
+".href~screen.~s.fl(~Version~harCode~&&(~_'+~variableProvider~s.pe~)?'Y':'N'~:'';h=h?h~._i~e&&l$HSESSION'~f',~onload~name~home#4~objectID~}else{~.s_~s.rl[u~Width~s.ssl~o.type~Timeout(~ction~Lifetime"
+"~.mrq(\"'+un+'\")~sEnabled~;i++)~'){q='~&&l$HNONE'){~ExternalLinks~charSet~onerror~lnk~currencyCode~.src~s=s_gi(~etYear(~&&!~Opera~'s_~;try{~Math.~s.fsg~s.ns6~s.oun~InlineStats~Track~'0123456789~&&"
+"t~s[k]=~s.epa(~m._d~n=s.oid(o)~,'sqs',q);~LeaveQuery~')>=~'=')~)+'/~){n=~\",''),~vo)~s.sampled~=s.oh(o);~+(y<1900?~s.disable~ingServer~n]=~true~sess~campaign~lif~if(~'http~,100)~s.co(~x in ~s.ape~f"
+"fset~s.c_d~s.br~'&pe~s.gg(~s.gv(~s[mn]~s.qav~,'vo~s.pl~=(apn~Listener~\"s_gs(\")~vo._t~b.attach~d.create~=s.n.app~(''+~!='~'||t~'+n~s()+'~){p=~():''~a):f(~+1))~a['!'+t]~){v=s.n.~channel~un)~.target"
+"~o.value~g+\"_c\"]~\".tl(\")~etscape~(ns?ns:~s_')t=t~k',s.bc~omePage~s.d.get~')<~||!~[b](e);~m[t+1](~return~mobile~height~events~random~code~'MSIE ~rs,~un,~,pev~floor(~atch~s.num(~[\"s_\"+~s.c_gd~s"
+".dc~s.pg~,'lt~.inner~transa~;s.gl(~\"m_\"+n~idt='+~page~Group,~.fromC~sByTag~?'&~+';'~t&&~1);~){s.~[t]=~>=5)~[t](~=l[n];~!a[t])~~s._c=@Nc';`F=^1`5!`F`hn){`F`hl`U;`F`hn=0;}s^sl=`F`hl;s^sn=`F`hn;s^sl"
+"[s^s@os;`F`hn++;s.m`0m){`2$Gm)`4'{$d0`Afl`0x,l){`2x?$Gx)`30,l):x`Aco`0o`H!o)`2o;`Kn`E,x;^B@xo)@tx`4'select$d0&&x`4'filter$d0)n[x]=o[x];`2n`Anum`0x){x`e+x;^B`Kp=0;p<x`C;p++)@t(@V')`4x`3p,p$O<0)`20;`"
+"21`Arep=s_r;@y`0x`1,h=@VABCDEF',i,c=s.@E,n,l,e,y`e;c=c?c`D$M`5x){x`e+x`5c`tAUTO'^m'').c^lAt){^Bi=0;i<x`C@A{c=x`3i,i+#Bn=x.c^lAt(i)`5n>127){l=0;e`e;^4n||l<4){e=h`3n%16,n%16+1)+e;n=(n-n%16)/16;l++}y+"
+"='%u'+e}`Bc`t+')y+='%2B';`my+=^gc)}x=y^zx=x?`v^g''+x),'+`G%2B'):x`5x&&c^Eem==1&&x`4'%u$d0&&x`4'%U$d0){i=x`4'%^R^4i>=0){i++`5h`38)`4x`3i,i+1)`D())>=0)`2x`30,i)+'u00'+x`3i);i=x`4'%',i)}}}}`2x`Aepa`0x"
+"`1;`2x?un^g`v''+x,'+`G ')):x`Apt`0x,d,f,a`1,t=x,z=0,y,r;^4t){y=t`4d);y=y<0?t`C:y;t=t`30,y);^At,$Nt,a)`5r)`2r;z+=y+d`C;t=x`3z,x`C);t=z<x`C?t:''}`2''`Aisf`0t,a){`Kc=a`4':')`5c>=0)a=a`30,c)`5t`30,2)`t"
+"$Z`32);`2(t!`e@W==a)`Afsf`0t,a`1`5`La,`G,'is^ut))@Q+=(@Q!`e?`G`j+t;`20`Afs`0x,f`1;@Q`e;`Lx,`G,'fs^uf);`2@Q`Ac_d`e;$vf`0t,a`1`5!$tt))`21;`20`Ac_gd`0`1,d=`F`J^5^w,n=s.fpC`V,p`5!n)n=s.c`V`5d@L$0@gn?^F"
+"n):2;n=n>2?n:2;p=d^6.')`5p>=0){^4p>=0&&n>1$Ld^6.',p-#Bn--}$0=p>0&&`Ld,'.`Gc_gd^u0)?d`3p):d}}`2$0`Ac_r`0k`1;k=@y(k);`Kc=' '+s.d.`u,i=c`4' '+k+@e,e=i<0?i:c`4';',i),v=i<0?'':@Yc`3i+2+k`C,e<0?c`C:e));`"
+"2v$H[[B]]'?v:''`Ac_w`0k,v,e`1,d=$v(),l=s.`u@7,t;v`e+v;l=l?$Gl)`D$M`5^t@Ct=(v!`e?^Fl?l:0):-60)`5t){e`Z;e.setTime(e`T+(t*1000))}`lk@Cs.d.`u=k+'`Pv!`e?v:'[[B]]')+'; path=/;'+(^t?' expires='+e.toGMT^3("
+")#9`j+(d?' domain='+d#9`j;`2^Vk)==v}`20`Aeh`0o,e,r,f`1,b='s^ne+'^ns^sn,n=-1,l,i,x`5!^Xl)^Xl`U;l=^Xl;^Bi=0;i<l`C&&n<0;i++`Hl[i].o==o&&l[i].e==e)n=i`ln<0@gi;l[n]`E}x#Gx.o=o;x.e=e;f=r?x.b:f`5r||f){x.b"
+"=r?0:o[e];x.o[e]=f`lx.b){x.o[b]=x.b;`2b}`20`Acet`0f,a,t,o,b`1,r,^d`5`O>=5^m!s.^e||`O>=7)){^d`7's`Gf`Ga`Gt`G`Ke,r@O^A$Na)`br=s.m(t)?s#Fe):t(e)}`2r^Rr=^d(s,f,a,t)^z@ts.^f^Eu`4$n4@d0)r=s.m(b)?s[b](a):"
+"b(a);else{^X(`F,'@F',0,o);^A$Na`Reh(`F,'@F',1)}}`2r`Ag^Tet`0e`1;`2`w`Ag^Toe`7'e`G`Ks=`9,c;^X(^1,\"@F\",1`Re^T=1;c=s.t()`5c)s.d.write(c`Re^T=0;`2@p'`Rg^Tfb`0a){`2^1`Ag^Tf`0w`1,p=w^0,l=w`J;`w=w`5p&&p"
+"`J!=l&&p`J^5==l^5){`w=p;`2s.g^Tf(`w)}`2`w`Ag^T`0`1`5!`w){`w=`F`5!s.e^T)`w=s.cet('g^T^u`w,'g^Tet',s.g^Toe,'g^Tfb')}`2`w`Amrq`0u`1,l=@1],n,r;@1]=0`5l)^Bn=0;n<l`C;n++){r#Gs.mr(0,0,r.r,0,r.t,r.u)}`Abr`"
+"0id,rs`1`5@m`a$e^W@Nbr',rs))$1l=rs`Aflush`a`0`1;s.fbr(0)`Afbr`0id`1,br=^V@Nbr')`5!br)br=$1l`5br`H!@m`a)^W@Nbr`G'`Rmr(0,0,br)}$1l=0`Amr`0@q,q,$oid,ta,u`1,dc=$w,t1=s.`x@n,t2=s.`x@nSecure,ns=s.`c`ispa"
+"ce,un=u?u:$Ys.f$S,unc=`v$p'_`G-'),r`E,l,imn=@Ni^n($S,im,b,e`5!rs){rs=@u'+(@3?'s'`j+'://'+(t1?(@3@W2?t2:t1):($Y(@3?'102':unc))+'.'+($w?$w:112)+'.2o7.net')@fb/ss/'+^C+'/'+(s.$i?'5.1':'1'@fH.17/'+@q+'"
+"?AQB=1&ndh=1'+(q?q`j+'&AQE=1'`5^Y@Ls.^f`H`O>5.5)rs=^j$o4095);`mrs=^j$o2047)`lid){$1(id,rs);$h}`ls.d.images&&`O>=3^m!s.^e||`O>=7)^m@R<0||`O>=6.1)`H!s.rc)s.rc`E`5!^O){^O=1`5!s.rl)s.rl`E;@1n]`U;set@5'"
+"@t^1`hl)^1.`9@8',750)^zl=@1n]`5l){r.t=ta;r.u=un;r.r=rs;l[l`C]=r;`2''}imn+='^n^O;^O++}im=`F[imn]`5!im)im=`F[im@onew Image;im@0l=0;im.^v`7'e`G^S@0l=1`5^1`hl)^1.`9@8^Rim@I=rs`5rs`4$2=@d0^m!ta||ta`t_se"
+"lf$Ia`t_top'||(`F.^w@Wa==`F.^w))){b=e`Z;^4!im@0l&&e`T-b`T<500)e`Z}`2''}`2'<im'+'g sr'+'c=\"'+rs+'\" width=1 $j=1 border=0 alt=\"\">'`Agg`0v`1`5!`F['s^nv])`F['s^nv]`e;`2`F['s^nv]`Aglf`0t,a`Ht`30,2)`"
+"t$Z`32);`Ks=^S,v=$3t)`5v)s#Dv`Agl`0v`1`5$x)`Lv,`G,'gl^u0)`Agv`0v`1;`2s['vpm^nv]?s['vpv^nv]:(s[v]?s[v]`j`Ahavf`0t,a`1,b=t`30,4),x=t`34),n=^Fx),k='g^nt,m='vpm^nt,q=t,v=s.`N@UVa$oe=s.`N@U^Qs,mn;@X$4t)"
+"`5s.@G||^D||^p`H^p^Epe`30,4)$H@G_'){mn=^p`30,1)`D()+^p`31)`5$5){v=$5.`xVars;e=$5.`x^Qs}}v=v?v+`G+^Z+`G+^Z2:''`5v@L`Lv,`G,'is^ut))s[k]`e`5t`t$k'&&e)@Xs.fs(s[k],e)}s[m]=0`5t`t^K`ID`6`cID`Ivid`6^I@Bg'"
+"`d`Bt`t`X@Br'`d`Bt`tvmk`Ivmt`6@E@Bce'`5s[k]&&s[k]`D()`tAUTO')@X'ISO8859-1';`Bs[k]^Eem==2)@X'UTF-8'}`Bt`t`c`ispace`Ins`6c`V`Icdp`6`u@7`Icl`6^o`Ivvp`6@H`Icc`6$R`Ich`6#0@6ID`Ixact`6@r`Iv0`6^U`Is`6^2`I"
+"c`6`o^k`Ij`6`f`Iv`6`u@9`Ik`6`z@2`Ibw`6`z^b`Ibh`6`g`Ict`6^x`Ihp`6p^J`Ip';`B$tx)`Hb`tprop`Ic$J;`Bb`teVar`Iv$J;`Bb`thier@Bh$J`d`ls[k]@W$H`N`i'@W$H`N^M')$6+='&'+q+'`Ps[k]);`2''`Ahav`0`1;$6`e;`L^a,`G,'h"
+"av^u0);`2$6`Alnf`0^c`8^r`8:'';`Kte=t`4@e`5t@We>0&&h`4t`3te$O>=0)`2t`30,te);`2''`Aln`0h`1,n=s.`N`is`5n)`2`Ln,`G,'ln^uh);`2''`Altdf`0^c`8^r`8:'';`Kqi=h`4'?^Rh=qi>=0?h`30,qi):h`5#Ah`3h`C-(t`C$O`t.'+t)"
+"`21;`20`Altef`0^c`8^r`8:''`5#Ah`4t)>=0)`21;`20`Alt`0h`1,lft=s.`N^PFile^Ms,lef=s.`NEx`n,@s=s.`NIn`n;@s=@s?@s:`F`J^5^w;h=h`8`5s.`x^PLinks&&lf#A`Llft,`G$yd^uh))`2'd'`5s.`x@D&&h`30,1)$H# '^mlef||@s)^m!"
+"lef||`Llef,`G$ye^uh))^m!@s$e`L@s,`G$ye^uh)))`2'e';`2''`Alc`7'e`G`Ks=`9,b=^X(^S,\"`k\"`R@G=@w^S`Rt(`R@G=0`5b)`2^S$f`2@p'`Rbc`7'e`G`Ks=`9,f,^d`5s.d^Ed.all^Ed.all.cppXYctnr)$h;^D=e@I`S?e@I`S:e$T;^d`7"
+"\"s\",\"`Ke@O@t^D^m^D.tag`i||^D^0`S||^D^0Node))s.t()`b}\");^d(s`Reo=0'`Roh`0o`1,l=`F`J,h=o^h?o^h:'',i,j,k,p;i=h`4':^Rj=h`4'?^Rk=h`4'/')`5h^mi<0||(j>=0&&i>j)||(k>=0&&i>k))$Lo`Y&&o`Y`C>1?o`Y:(l`Y?l`Y"
+"`j;i=l.path^w^6/^Rh=(p?p+'//'`j+(o^5?o^5:(l^5?l^5`j)+(h`30,1)$H/'?l.path^w`30,i<0?0:i@f'`j+h}`2h`Aot`0o){`Kt=o.tag`i;t=t@W`D?t`D$M`5t`tSHAPE')t`e`5t`Ht`tINPUT'&&@4&&@4`D)t=@4`D();`B!#Ao^h)t='A';}`2"
+"t`Aoid`0o`1,^G,p,c,n`e,x=0`5t@L`y$Lo`Y;c=o.`k`5o^h^mt`tA$I`tAREA')^m!c$ep||p`8`4'`o$d0))n@k`Bc@g`vs.rep(`vs.rep$Gc,\"\\r@h\"\\n@h\"\\t@h' `G^Rx=2}`B$U^mt`tINPUT$I`tSUBMIT')@g$U;x=3}`Bo@I@W`tIMAGE')"
+"n=o@I`5n){`y=^jn@v;`yt=x}}`2`y`Arqf`0t,un`1,e=t`4@e,u=e>=0?`G+t`30,e)+`G:'';`2u&&u`4`G+un+`G)>=0?@Yt`3e$O:''`Arq`0un`1,c=un`4`G),v=^V@Nsq'),q`e`5c<0)`2`Lv,'&`Grq^u$S;`2`L$p`G,'rq',0)`Asqp`0t,a`1,e="
+"t`4@e,q=e<0?'':@Yt`3e+1)`Rsqq[q]`e`5e>=0)`Lt`30,e),`G@b`20`Asqs`0$pq`1;^7u[u@oq;`20`Asq`0q`1,k=@Nsq',v=^Vk),x,c=0;^7q`E;^7u`E;^7q[q]`e;`Lv,'&`Gsqp',0);`L^C,`G@bv`e;^B@x^7u`Q)^7q[^7u[x]]+=(^7q[^7u[x"
+"]]?`G`j+x;^B@x^7q`Q&&^7q[x]^mx==q||c<2)){v+=(v#8'`j+^7q[x]+'`Px);c++}`2^Wk,v,0)`Awdl`7'e`G`Ks=`9,r=@p,b=^X(`F,\"^v\"),i,o,oc`5b)r=^S$f^Bi=0;i<s.d.`Ns`C@A{o=s.d.`Ns[i];oc=o.`k?\"\"+o.`k:\"\"`5(oc`4$"
+"B<0||oc`4\"@0oc(\")>=0)&&oc`4$W<0)^X(o,\"`k\",0,s.lc);}`2r^R`Fs`0`1`5`O>3^m!^Y$es.^f||`O#E`Hs.b^E$D^Q)s.$D^Q('`k',s.bc);`Bs.b^Eb.add^Q$A)s.b.add^Q$A('clic$a,false);`m^X(`F,'^v',0,`Fl)}`Avs`0x`1,v=s"
+".`c^N,g=s.`c^N#5k=@Nvsn^n^C+(g?'^ng`j,n=^Vk),e`Z,y=e.g@K);e.s@Ky+10@l1900:0))`5v){v*=100`5!n`H!^Wk,x,e))`20;n=x`ln%10000>v)`20}`21`Adyasmf`0t,m`H#Am&&m`4t)>=0)`21;`20`Adyasf`0t,m`1,i=t?t`4@e:-1,n,x"
+"`5i>=0&&m){`Kn=t`30,i),x=t`3i+1)`5`Lx,`G,'dyasm^um))`2n}`20`Auns`0`1,x=s.`MSele@6,l=s.`MList,m=s.`MM$s,n,i;^C=^C`8`5x&&l`H!m)m=`F`J^5`5!m.toLowerCase)m`e+m;l=l`8;m=m`8;n=`Ll,';`Gdyas^um)`5n)^C=n}i="
+"^C`4`G`Rfun=i<0?^C:^C`30,i)`Asa`0un`1;^C=un`5!@S)@S=un;`B(`G+@S+`G)`4$S<0)@S+=`G+un;^Cs()`Am_i`0n,a`1,m,f=n`30,1),r,l,i`5!`Wl)`Wl`E`5!`Wnl)`Wnl`U;m=`Wl[n]`5!a&&m&&m._e@Lm^s)`Wa(n)`5!m){m`E,m._c=@Nm"
+"';m^sn=`F`hn;m^sl=s^sl;m^sl[m^s@om;`F`hn++;m.s=s;m._n=n;m._l`U('_c`G_in`G_il`G_i`G_e`G_d`G_dl`Gs`Gn`G_r`G_g`G_g1`G_t`G_t1`G_x`G_x1`G_l'`Rm_l[@om;`Wnl[`Wnl`C]=n}`Bm._r@Lm._m){r=m._r;r._m=m;l=m._l;^B"
+"i=0;i<l`C@A@tm[l[i]])r[l[i]]=m[l[i]];r^sl[r^s@or;m=`Wl[@or`lf==f`D())s[@om;`2m`Am_a`7'n`Gg`G@t!g)g=#2;`Ks=`9,c=s[$V,m,x,f=0`5!c)c=`F$u$V`5c&&s_d)s[g]`7\"s\",s_ft(s_d(c)));x=s[g]`5!x)x=`F$ug];m=`Wi("
+"n,1)`5x){m^s=f=1`5(\"\"+x)`4\"fun@6\")>=0)x(s);`m`Wm(\"x\",n,x)}m=`Wi(n,1)`5@Zl)@Zl=@Z=0;`pt();`2f'`Rm_m`0t,n,d){t='^nt;`Ks=^S,i,x,m,f='^nt`5`Wl&&`Wnl)^Bi=0;i<`Wnl`C@A{x=`Wnl[i]`5!n||x==n){m=`Wi(x)"
+"`5m[t]`Ht`t_d')`21`5d)m#Fd);`mm#F)`lm[t+1]@Lm[f]`Hd)$gd);`m$g)}m[f]=1}}`20`AloadModule`0n,u,d,l`1,m,i=n`4':'),g=i<0?#2:n`3i+1),o=0,f,c=s.h?s.h:s.b,^d`5i>=0)n=n`30,i);m=`Wi(n)`5(l$e`Wa(n,g))&&u^Ed&&"
+"c^E$E`S`Hd){@Z=1;@Zl=1`l@3)u=`vu,@u:`Ghttps:^Rf`7'e`G`9.m_a(\"$J+'\",\"'+g+'\")^R^d`7's`Gf`Gu`Gc`G`Ke,o=0@Oo=s.$E`S(\"script\")`5o){@4=\"text/`o\"`5f)o.^v=f;o@I=u;c.appendChild(o)}`bo=0}`2o^Ro=^d(s"
+",f,u,c)}`mm=`Wi(n);m._e=1;`2m`Avo1`0t,a`Ha[t]||$P)^S#Da[t]`Avo2`0t,a`H#H{a#D^S[t]`5#H$P=1}`Adlt`7'`Ks=`9,d`Z,i,vo,f=0`5`pl)^Bi=0;i<`pl`C@A{vo=`pl[i]`5vo`H!`Wm(\"d\")||d`T-$C>=^8){`pl[i]=0;s.t(@i}`m"
+"f=1}`l`pi)clear@5`pi`Rdli=0`5f`H!`pi)`pi=set@5`pt,^8)}`m`pl=0'`Rdl`0vo`1,d`Z`5!@ivo`E;`L^9,`G$72',@i;$C=d`T`5!`pl)`pl`U;`pl[`pl`C]=vo`5!^8)^8=250;`pt()`At`0vo,id`1,trk=1,tm`Z,sed=Math&&@P$l?@P$r@P$"
+"l()*10000000000000):tm`T,@q='s'+@P$rtm`T/10800000)%10+sed,y=tm.g@K),vt=tm.getDate(@f`sMonth(@f'@ly+1900:y)+' `sHour$K:`sMinute$K:`sSecond$K `sDay()+' `sTimezoneO@z(),^d,^T=s.g^T(),ta`e,q`e,qs`e,$m`"
+"e,vb`E#1^9`Runs()`5!s.td){`Ktl=^T`J,a,o,i,x`e,c`e,v`e,p`e,bw`e,bh`e,^H0',k=^W@Ncc`G@p',0^q,hp`e,ct`e,pn=0,ps`5^3&&^3.prototype){^H1'`5j.m$s){^H2'`5tm.setUTCDate){^H3'`5^Y^E^f&&`O#E^H4'`5pn.toPrecis"
+"ion){^H5';a`U`5a.forEach){^H6';i=0;o`E;^d`7'o`G`Ke,i=0@Oi=new Iterator(o)`b}`2i^Ri=^d(o)`5i&&i.next)^H7'}}}}`l`O>=4)x=^iwidth+'x'+^i$j`5s.isns||s.^e`H`O>=3$Q`f(^q`5`O>=4){c=^ipixelDepth;bw=`F$z@2;b"
+"h=`F$z^b}}$8=s.n.p^J}`B^Y`H`O>=4$Q`f(^q;c=^i^2`5`O#E{bw=s.d.^L`S.o@z@2;bh=s.d.^L`S.o@z^b`5!s.^f^Eb){^d`7's`Gtl`G`Ke,hp=0`qh$b\");hp=s.b.isH$b(tl)?\"Y\":\"N\"`b}`2hp^Rhp=^d(s,tl);^d`7's`G`Ke,ct=0`qc"
+"lientCaps\");ct=s.b.`g`b}`2ct^Rct=^d(s)}}}`mr`e`l$8)^4pn<$8`C&&pn<30){ps=^j$8[pn].^w@v#9`5p`4ps)<0)p+=ps;pn++}s.^U=x;s.^2=c;s.`o^k=j;s.`f=v;s.`u@9=k;s.`z@2=bw;s.`z^b=bh;s.`g=ct;s.^x=hp;s.p^J=p;s.td"
+"=1`l@i{`L^9,`G$72',vb);`L^9,`G$71',@i`ls.useP^J)s.doP^J(s);`Kl=`F`J,r=^T.^L.`X`5!s.^I)s.^I=l^h?l^h:l`5!s.`X@Ls._1_`X#C`X=r;s._1_`X=1}`Wm('g')`5(vo&&$C)$e`Wm('d')`Hs.@G||^D){`Ko=^D?^D:s.@G`5!o)`2'';"
+"`Kp=$4'#4`i'),w=1,^G,@a,x=`yt,h,l,i,oc`5^D&&o==^D){^4o@Ln@W$HBODY'){o=o^0`S?o^0`S:o^0Node`5!o)`2'';^G;@a;x=`yt}oc=o.`k?''+o.`k:''`5(oc`4$B>=0&&oc`4\"@0oc(\")<0)||oc`4$W>=0)`2''}ta=n?o$T:1;h@ki=h`4'"
+"?^Rh=s.`N@c^3||i<0?h:h`30,i);l=s.`N`i?s.`N`i:s.ln(h);t=s.`N^M?s.`N^M`8:s.lt(h)`5t^mh||l))q+=$2=@G^n(t`td$I`te'?@y(t):'o')+(h?$2v1`Ph)`j+(l?$2v2`Pl)`j;`mtrk=0`5s.`x@T`H!p$L$4'^I^Rw=0}^G;i=o.sourceIn"
+"dex`5$3'^y')@g$3'^y^Rx=1;i=1`lp&&n@W)qs='&pid`P^jp,255))+(w#8p#3w`j+'&oid`P^jn@v)+(x#8o#3x`j+'&ot`Pt)+(i#8oi='+i`j}`l!trk@Lqs)`2'';@j=s.vs(sed)`5trk`H@j)$m=s.mr(@q,(vt#8t`Pvt)`j+s.hav()+q+(qs?qs:s."
+"rq(^C)),0,id,ta);qs`e;`Wm('t')`5s.p_r)s.p_r(`R`X`e}^7(qs);^z`p(@i;`l@i`L^9,`G$71',vb`R@G=^D=s.`N`i=s.`N^M=`F@0^y=s.ppu=^p=^pv1=^pv2=^pv3`e`5$x)`F@0@G=`F@0eo=`F@0`N`i=`F@0`N^M`e`5!id@Ls.tc#Ctc=1;s.f"
+"lush`a()}`2$m`Atl`0o,t,n,vo`1;s.@G=@wo`R`N^M=t;s.`N`i=n;s.t(@i}`5pg){`F@0co`0o){`K@J\"_\",1,#B`2@wo)`Awd@0gs`0$S{`K@J$p1,#B`2s.t()`Awd@0dc`0$S{`K@J$p#B`2s.t()}}@3=(`F`J`Y`8`4@us@d0`Rd=^L;s.b=s.d.bo"
+"dy`5$c`S#7`i#Ch=$c`S#7`i('HEAD')`5s.h)s.h=s.h[0]}s.n=navigator;s.u=s.n.userAgent;@R=s.u`4'N$X6/^R`Kapn$F`i,v$F^k,ie=v`4$n'),o=s.u`4'@M '),i`5v`4'@M@d0||o>0)apn='@M';^Y$9`tMicrosoft Internet Explore"
+"r'`Risns$9`tN$X'`R^e$9`t@M'`R^f=(s.u`4'Mac@d0)`5o>0)`O`rs.u`3o+6));`Bie>0){`O=^Fi=v`3ie+5))`5`O>3)`O`ri)}`B@R>0)`O`rs.u`3@R+10));`m`O`rv`Rem=0`5^3#6^l){i=^g^3#6^l(256))`D(`Rem=(i`t%C4%80'?2:(i`t%U0"
+"100'?1:0))}s.sa(un`Rvl_l='^K,`cID,vmk,ppu,@E,`c`ispace,c`V,`u@7,#4`i,^I,`X,@H';^a=^Z+',^o,$R,server,#4^M,#0@6ID,purchaseID,@r,state,zip,$k,products,`N`i,`N^M';^B`Kn=1;n<51;n++)^a+=',prop$J+',eVar$J"
+"+',hier$J;^Z2=',^U,^2,`o^k,`f,`u@9,`z@2,`z^b,`g,^x,pe$q1$q2$q3,p^J';^a+=^Z2;^9=^a+',$i,`c^N,`c^N#5`MSele@6,`MList,`MM$s,`x^PLinks,`x@D,`x@T,`N@c^3,`N^PFile^Ms,`NEx`n,`NIn`n,`N@UVa$o`N@U^Qs,`N`is,@G"
+",eo';$x=pg#1^9)`5!ss)`Fs()",
w=window,l=w.s_c_il,n=navigator,u=n.userAgent,v=n.appVersion,e=v.indexOf('MSIE '),m=u.indexOf('Netscape6/'),a,i,s;if(un){un=un.toLowerCase();if(l)for(i=0;i<l.length;i++){s=l[i];if(s._c=='s_c'){if(s.oun==un)return s;else if(s.fs&&s.sa&&s.fs(s.oun,un)){s.sa(un);return s}}}}
w.s_r=new Function("x","o","n","var i=x.indexOf(o);if(i>=0&&x.split)x=(x.split(o)).join(n);else while(i>=0){x=x.substring(0,i)+n+x.substring(i+o.length);i=x.indexOf(o)}return x");
w.s_d=new Function("x","var t='`^@$#',l='0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz',d,n=0,b,k,w,i=x.lastIndexOf('~~');if(i>0){d=x.substring(0,i);x=x.substring(i+2);while(d){w=d;i"
+"=d.indexOf('~');if(i>0){w=d.substring(0,i);d=d.substring(i+1)}else d='';b=(n-n%62)/62;k=n-b*62;k=t.substring(b,b+1)+l.substring(k,k+1);x=s_r(x,k,w);n++}for(i=0;i<5;i++){w=t.substring(i,i+1);x=s_r(x"
+",w+' ',w)}}return x");
w.s_fe=new Function("c","return s_r(s_r(s_r(c,'\\\\','\\\\\\\\'),'\"','\\\\\"'),\"\\n\",\"\\\\n\")");
w.s_fa=new Function("f","var s=f.indexOf('(')+1,e=f.indexOf(')'),a='',c;while(s>=0&&s<e){c=f.substring(s,s+1);if(c==',')a+='\",\"';else if((\"\\n\\r\\t \").indexOf(c)<0)a+=c;s++}return a?'\"'+a+'\"':"
+"a");
w.s_ft=new Function("c","c+='';var s,e,o,a,d,q,f,h,x;s=c.indexOf('=function(');while(s>=0){s++;d=1;q='';x=0;f=c.substring(s);a=s_fa(f);e=o=c.indexOf('{',s);e++;while(d>0){h=c.substring(e,e+1);if(q){i"
+"f(h==q&&!x)q='';if(h=='\\\\')x=x?0:1;else x=0}else{if(h=='\"'||h==\"'\")q=h;if(h=='{')d++;if(h=='}')d--}if(d>0)e++}c=c.substring(0,s)+'new Function('+(a?a+',':'')+'\"'+s_fe(c.substring(o+1,e))+'\")"
+"'+c.substring(e+1);s=c.indexOf('=function(')}return c;");
c=s_d(c);if(e>0){a=parseInt(i=v.substring(e+5));if(a>3)a=parseFloat(i)}else if(m>0)a=parseFloat(u.substring(m+10));else a=parseFloat(v);if(a>=5&&v.indexOf('Opera')<0&&u.indexOf('Opera')<0){w.s_c=new Function("un","pg","ss","var s=this;"+c);return new s_c(un,pg,ss)}else s=new Function("un","pg","ss","var s=new Object;"+s_ft(c)+";return s");return s(un,pg,ss)}

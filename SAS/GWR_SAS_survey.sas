<html><head>
<meta http-equiv="content-type" content="text/html; charset=windows-1252"><link href="GWR_SAS_survey_arquivos/screen_002.css" rel="stylesheet" type="text/css"><link href="GWR_SAS_survey_arquivos/screen_003.css" rel="stylesheet" type="text/css"><link href="GWR_SAS_survey_arquivos/screen_004.css" rel="stylesheet" type="text/css"><link href="GWR_SAS_survey_arquivos/screen.css" rel="stylesheet" type="text/css"></head><body><div class="fixed leftAlign">
<em>/******************************************************/</em><br>
/* Written By Alan Ricardo da Silva&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;*/<br>
/* University of Brasilia, Brazil (08/2014)&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;*/<br>
/* E-mail: alansilva@unb.br&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;*/<br>
<em>/******************************************************/</em><br>
<br>
<em>/********************************************************************************/</em><br>
/* Macro for searching the Bandwidth */<br>
/*<br>
/* REQUIRED PARAMETERS<br>
/*&nbsp; &nbsp; DATA = the name of the SAS data set to be used<br>
/*&nbsp; &nbsp; YVAR = the name of the dependent or response variable<br>
/*&nbsp; &nbsp; XVAR = the name of the independent or explicative variables. A blank space<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;should separate the names. Note: an intercept variable must not be<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;created in advance<br>
/*&nbsp; DCOORD = the name of the SAS data set with the geographic coordinates<br>
/*&nbsp; OUTPUT = the name of the SAS data set to be used as output results<br>
/*&nbsp; &nbsp; MINV = the minimum distance between two locations i and k to be consider<br>
/* MIDDLEV = the middle distance between two locations i and k to be consider<br>
/*&nbsp; &nbsp; MAXV = the maximum distance between two locations i and k to be consider<br>
/*&nbsp; METHOD = there are three choices:<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;FIXED_G asks the program to compute the bandwidth as fixed gaussian;<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;FIXED_BSQ to compute the bandwidth as fixed bi-square; <br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;ADAPTIVEN to compute the bandwidth as adaptive bi-square ($n$ values) and<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp;&nbsp; ADAPTIVE_BSQ to compute the bandwidth as adaptive 
bi-square ($one$ value)<br>
/*&nbsp; DISTANCEKM = if the distances between two locations will be computed in Km<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;using the Basic formulae for calculating spherical distance. The <br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;default value is NO, so the distance is computed using euclidian<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;distance.<br>
<em>/********************************************************************************/</em><br>
<br>
%macro Golden(DATA=,YVAR=,XVAR=,WEIGHT=,DCOORD=,OUTPUT=,MINV=,MIDDLEV=,MAXV=,METHOD=,DISTANCEKM=NO);<br>
/* remember that pi=arcos(-1)=3.1415926536 */<br>
proc iml;<br>
use &amp;DATA;<br>
read all var {&amp;YVAR} into y;<br>
read all var {&amp;XVAR} into x;<br>
n=nrow(y);<br>
%IF &amp;WEIGHT NE %THEN %DO;<br>
read all var {&amp;WEIGHT} into wt;<br>
%END;<br>
%ELSE %DO;<br>
wt=j(n,1,1);<br>
%END;<br>
close &amp;DATA;<br>
x=j(n,1,1)||x;<br>
yhat=j(n,1,0);<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; hv=j(1,1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhat=j(1,1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; create &amp;OUTPUT from hv[colname='h'];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; do i=1 to n;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ax=&amp;MINV;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; bx=&amp;MIDDLEV;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; cx=&amp;MAXV;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; r=0.61803399;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; tol=0.001;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; c=1-r;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h0=ax;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h3=cx;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; *bx=c*(cx-ax);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if abs(cx-bx)&gt;abs(bx-ax) then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h1=bx;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h2=bx+c*(cx-bx);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h2=bx;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h1=bx-c*(bx-ax);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; end;<br>
<br>
use &amp;DCOORD;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br>
read all var{x y} into COORD;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; <br>
close &amp;DCOORD;<br>
<br>
start cv1;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; do i=1 to n;<br>
%END;<br>
*** Basic formulae for calculating spherical distance *** ;<br>
d=j(1,3,0);<br>
dist=d;<br>
&nbsp; &nbsp; &nbsp; &nbsp; do j=1 to n;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dif=abs(COORD[i,1]-COORD[j,1]);&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; raio=arcos(-1)/180;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
argument=sin(COORD[i,2]*raio)*sin(COORD[j,2]*raio)+cos(COORD[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if argument&gt;=1 then arco=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; /* Law of Cosines */&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
arco=arcos(sin(COORD[i,2]*raio)*sin(COORD[j,2]*raio)+cos(COORD[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio));<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=arco*6371 /*Earth's Radius = 6371 (aproximately)*/;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=1e-3 then d1=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=sqrt((COORD[i,1]-COORD[j,1])**2+(COORD[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END; <br>
%IF %UPCASE(&amp;METHOD)=FIXED_G %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=h1 &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1 ^=0 then do;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[1]=i;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[2]=j;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=arco*6371 /*Earth's Radius = 6371 (approximately)*/;&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=sqrt((COORD[i,1]-COORD[j,1])**2+(COORD[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dist=dist//d;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
u=nrow(dist);<br>
w=j(u,1,0);<br>
x1=x[i,];<br>
y1=y[i,];<br>
wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=2 to u;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=exp(-(dist[jj,3]/h1)**2);<br>
%IF %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=(1-(dist[jj,3]/h1)**2)**2;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; call sort(dist,{3});<br>
&nbsp; &nbsp; &nbsp; &nbsp; dist=dist||(1:nrow(dist))`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=j(n,2,0);&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; hn=dist[h1,3];<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=2 to n;<br>
&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if dist[jj,4]&lt;=h1 then<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,1]=(1-(dist[jj,3]/hn)**2)**2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else w[jj,1]=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,2]=dist[jj,2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; position=w[loc(w[,1]&gt;0),2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; w={0}//w[position,1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[position,];<br>
%END;<br>
if det(x1`*(w#x1#wt1))=0 then b=j(ncol(x),1,0);<br>
else<br>
b=inv(x1`*(w#x1#wt1))*x1`*(w#y1#wt1);<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhat[i]=x[i,]*b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
CV1=((y-yhat)#wt)`*(y-yhat);<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhat[1]=x[i,]*b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; CV1=((y[i]-yhat)#wt)`*(y[i]-yhat);<br>
%END;<br>
free dist w;<br>
finish cv1;<br>
<br>
start cv2;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
*** Basic formulae for calculating spherical distance *** ;<br>
&nbsp; &nbsp; &nbsp; &nbsp; do i=1 to n;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d=j(1,3,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dist=d;<br>
&nbsp; &nbsp; &nbsp; &nbsp; do j=1 to n;&nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dif=abs(COORD[i,1]-COORD[j,1]);&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; raio=arcos(-1)/180;&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
argument=sin(COORD[i,2]*raio)*sin(COORD[j,2]*raio)+cos(COORD[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if argument&gt;=1 then arco=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; /* Law of Cosines */&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
arco=arcos(sin(COORD[i,2]*raio)*sin(COORD[j,2]*raio)+cos(COORD[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio));<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=arco*6371 /*Earth's Radius = 6371 (aproximately)*/;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=1e-3 then d1=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=sqrt((COORD[i,1]-COORD[j,1])**2+(COORD[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=&amp;MAXV*1 &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=h1 &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1 ^=0 then do;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[1]=i;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[2]=j;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=arco*6371 /*Earth's Radius = 6371 (approximately)*/;&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=sqrt((COORD[i,1]-COORD[j,1])**2+(COORD[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dist=dist//d;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; u=nrow(dist);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w=j(u,1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; x1=x[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; y1=y[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; do jj=2 to u;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=exp(-(dist[jj,3]/h1)**2);<br>
%IF %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=(1-(dist[jj,3]/h1)**2)**2;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; call sort(dist,{3});<br>
&nbsp; &nbsp; &nbsp; &nbsp; dist=dist||(1:nrow(dist))`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=j(n,2,0);&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; hn=dist[h1,3];<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=2 to n;<br>
&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if dist[jj,4]&lt;=h1 then<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,1]=(1-(dist[jj,3]/hn)**2)**2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else w[jj,1]=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,2]=dist[jj,2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; position=w[loc(w[,1]&gt;0),2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; w={0}//w[position,1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[position,];<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if det(x1`*(w#x1#wt1))=0 then b=j(ncol(x),1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; b=inv(x1`*(w#x1#wt1))*x1`*(w#y1#wt1);<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; yhat[i]=x[i,]*b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
CV2=((y-yhat)#wt)`*(y-yhat);<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhat[1]=x[i,]*b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; CV2=((y[i]-yhat)#wt)`*(y[i]-yhat);<br>
%END;<br>
free dist w;<br>
finish cv2;<br>
<br>
run cv1;<br>
run cv2;<br>
<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; create &amp;OUTPUT var{h1 cv1 h2 cv2};<br>
%END;<br>
do while(abs(h3-h0) &gt; tol*(abs(h1)+abs(h2)));<br>
&nbsp; &nbsp; &nbsp; &nbsp; if CV2&lt;CV1 then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h0=h1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h1=h2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h2=r*h1+c*h3;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; CV1=CV2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; run cv2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; else do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h3=h2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h2=h1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h1=r*h2+c*h0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; CV2=CV1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; run cv1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; append;<br>
%END;<br>
end;<br>
if CV1&lt;CV2 then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; golden=CV1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; xmin=h1;<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; xmin=floor(h1);<br>
%end;<br>
end;<br>
else do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; golden=CV2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; xmin=h2;<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; xmin=floor(h2);<br>
%end;<br>
end;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; print golden xmin;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; hv[1]=xmin;<br>
&nbsp; &nbsp; &nbsp; &nbsp; append from hv;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
%END;<br>
quit;<br>
<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G or %UPCASE(&amp;METHOD)=FIXED_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %global _h_;<br>
&nbsp; &nbsp; &nbsp; &nbsp; data _null_;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; set &amp;OUTPUT;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; if CV1&lt;CV2 then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;call symput('_h_',h1);<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; else do;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;call symput('_h_',h2);<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; run;<br>
%put h=&amp;_h_;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
%global _h_;<br>
&nbsp; &nbsp; &nbsp; &nbsp; data _null_;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; set &amp;OUTPUT;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; if CV1&lt;CV2 then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;call symput('_h_',floor(h1));<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; else do;<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;call symput('_h_',floor(h2));<br>
&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; run;<br>
%put h=&amp;_h_;<br>
%END;<br>
%mend Golden;<br>
<br>
<em>/*******************************************************************************/</em><br>
/* Macro for estimating GWR Model */<br>
/* REQUIRED PARAMETERS<br>
/*&nbsp; &nbsp; DATA = the name of the SAS data set to be used<br>
/*&nbsp; &nbsp; YVAR = the name of the dependent or response variable<br>
/*&nbsp; &nbsp; XVAR = the name of the independent or explicative variables. A blank space<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;should separate the names. Note: an intercept variable must not be<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;created in advance<br>
/*&nbsp; &nbsp;WEIGHT = the name of the sample weight variable <br>
/*&nbsp; CLUSTER = the name of the cluster variable<br>
/*&nbsp; &nbsp;STRATA = the name of the strata variable<br>
/*&nbsp; &nbsp; &nbsp; FPC = the name of the sampling rate (n/N) or probability selection variable<br>
/*&nbsp; DCOORD = the name of the SAS data set with the geographic coordinates<br>
/*&nbsp; &nbsp; GRID = the name of the SAS data set with the grid of geographic coordinates<br>
/*&nbsp; SIGMA2 = the value of the variance to be used in the standard errors<br>
/* GMATRIX = the name of the SAS data set with the G MATRIX to be used in the<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;the standard errors of complex data<br>
/*&nbsp; &nbsp; &nbsp;DHV = the name of the SAS data set with the bandwidth adaptive ($n$ values),<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;which must have an unique variable<br>
/*&nbsp; &nbsp; &nbsp; &nbsp;H = A pre-defined bandwidth value for METHOD equal to FIXED or ADAPTIVE1<br>
/*&nbsp; &nbsp; MAXV = the maximum distance between two locations i and k to be consider<br>
/*&nbsp; METHOD = there are three choices:<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;FIXED_G asks the program to compute the bandwidth as fixed gaussian;<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;FIXED_BSQ to compute the bandwidth as fixed bi-square; <br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;ADAPTIVEN to compute the bandwidth as adaptive bi-square ($n$ values) and<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp;&nbsp; ADAPTIVE_BSQ to compute the bandwidth as adaptive 
bi-square ($one$ value)<br>
/*&nbsp; DISTANCEKM = if the distances between two locations will be computed in Km<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;using the Basic formulae for calculating spherical distance. The <br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;default value is NO, so the distance is computed using euclidian<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;distance.<br>
<em>/********************************************************************************/</em><br>
<br>
%macro GWR(DATA=,YVAR=,XVAR=,WEIGHT=,CLUSTER=,STRATA=,FPC=,DCOORD=,GRID=,SIGMA2=,GMATRIX=,DHV=,MAXV=,METHOD=,DISTANCEKM=NO,H=);<br>
%IF &amp;CLUSTER NE AND &amp;STRATA NE %THEN %DO;<br>
proc sort data=&amp;data;by &amp;STRATA &amp;CLUSTER;<br>
%END;<br>
%ELSE %IF &amp;CLUSTER NE %THEN %DO;<br>
proc sort data=&amp;data;by &amp;CLUSTER;<br>
%END;<br>
%ELSE %IF &amp;STRATA NE %THEN %DO;<br>
proc sort data=&amp;data;by &amp;STRATA;<br>
%END;<br>
proc iml;<br>
use &amp;DATA;<br>
read all var {&amp;YVAR} into y;<br>
read all var {&amp;XVAR} into x;<br>
n=nrow(y);<br>
%IF &amp;CLUSTER NE %THEN %DO;<br>
read all var {&amp;CLUSTER} into ci;<br>
%END;<br>
%IF &amp;STRATA NE %THEN %DO;<br>
read all var {&amp;STRATA} into sh;<br>
%END;<br>
fh=j(n,1,0);<br>
%IF &amp;FPC NE %THEN %DO;<br>
read all var {&amp;FPC} into fh;<br>
%END;<br>
wt=j(n,1,1);<br>
%IF &amp;WEIGHT NE %THEN %DO;<br>
read all var {&amp;WEIGHT} into wt;<br>
%END;<br>
close &amp;DATA;<br>
ni=ncol(unique(ci));<br>
nh=ncol(unique(sh));<br>
<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; use &amp;DHV;read all into hv;<br>
%END;<br>
x=j(n,1,1)||x;<br>
yhat=j(n,1,0);<br>
<br>
%IF &amp;H NE %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; h=&amp;H;<br>
&nbsp; &nbsp; &nbsp; &nbsp; print h[label="Bandwidth"];<br>
%END;<br>
%ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;METHOD)=FIXED_G or 
%UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ 
%THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h=&amp;_h_;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; print h[label="Bandwidth"];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
%END;<br>
<br>
use &amp;DCOORD;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br>
read all var{x y} into COORD;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; <br>
%if &amp;grid= %then %do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; read all var{x y} into POINTS;&nbsp; &nbsp; &nbsp;<br>
%end;<br>
close &amp;DCOORD;<br>
%if &amp;grid^= %then %do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; use &amp;grid;&nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; read all var{x y} into POINTS;&nbsp; &nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; close &amp;grid;<br>
%end;<br>
m=nrow(POINTS);<br>
<br>
bi=j(ncol(x)*m,4,0);<br>
BB=j(ncol(x)*n,n,0);<br>
rsqri=j(m,1,0);<br>
sumwi=j(m,1,0);<br>
varbi=j(ncol(x)*m,1,0);<br>
varbigg=j(ncol(x)*m,1,0);<br>
varbis=j(ncol(x)*m,ncol(x),0);<br>
S=j(m,1,0);<br>
S2=j(m,1,0);<br>
biT=j(m,ncol(x)+1,0);<br>
ym=y-y[:];<br>
*rikl=j(n,comb(ncol(x)-1,2),0);<br>
vif=j(n,ncol(x)-1,0);<br>
<br>
<br>
<br>
*** Basic formulae for calculating spherical distance *** ;<br>
do i=1 to m;<br>
free dist;<br>
&nbsp; &nbsp; &nbsp; &nbsp; d=j(1,3,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; do j=1 to n;&nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dif=abs(POINTS[i,1]-COORD[j,1]);&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; raio=arcos(-1)/180;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
argument=sin(POINTS[i,2]*raio)*sin(COORD[j,2]*raio)+cos(POINTS[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio);
 <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if argument&gt;1 then arco=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; /* Law of Cosines */&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
arco=arcos(sin(POINTS[i,2]*raio)*sin(COORD[j,2]*raio)+cos(POINTS[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio));<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=arco*6371 /*Earth's Radius = 6371 (aproximately)*/;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=1e-3 then d1=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=sqrt((POINTS[i,1]-COORD[j,1])**2+(POINTS[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=&amp;MAXV*1 &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=FIXED_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=h &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=hv[i] &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1 ^=0 then do;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[1]=i;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[2]=j;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=arco*6371 /*Earth's Radius = 6371 (approximately)*/;&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=sqrt((POINTS[i,1]-COORD[j,1])**2+(POINTS[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dist=dist//d;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; u=nrow(dist);<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=j(u,1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %if &amp;grid= %then %do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym1=ym[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=1 to u;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=exp(-(dist[jj,3]/h)**2);<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=FIXED_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=(1-(dist[jj,3]/h)**2)**2;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=(1-(dist[jj,3]/hv[i])**2)**2;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %if &amp;grid= %then %do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ym1=ym1//ym[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; w={1}//w;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym1=ym[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; call sort(dist,{3});<br>
&nbsp; &nbsp; &nbsp; &nbsp; dist=dist||(1:nrow(dist))`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=j(n,2,0);&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; hn=dist[h,3];<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=1 to n-1;<br>
&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if dist[jj,4]&lt;=h then<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,1]=(1-(dist[jj,3]/hn)**2)**2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else w[jj,1]=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,2]=dist[jj,2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; position=w[loc(w[,1]&gt;0),2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; w={1}//w[loc(w[,1]&gt;0),1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym1=ym1//ym[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %else %do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; if jj=1 then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; x1=x[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; y1=y[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ym1=ym[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; else do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ym1=ym1//ym[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; call sort(dist,{3});<br>
&nbsp; &nbsp; &nbsp; &nbsp; dist=dist||(1:n)`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=j(n,2,0);&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; hn=dist[h,3];<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=1 to n;<br>
&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if dist[jj,4]&lt;=h then<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,1]=(1-(dist[jj,3]/hn)**2)**2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else w[jj,1]=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,2]=dist[jj,2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; position=w[loc(w[,1]&gt;0),2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=w[loc(w[,1]&gt;0),1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym1=ym[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; if det(x1`*(w#x1#wt1))=0 then b=j(ncol(x),1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; b=inv(x1`*(w#x1#wt1))*x1`*(w#y1#wt1);<br>
&nbsp; &nbsp; &nbsp; &nbsp; if det(x1`*(w#x1#wt1))=0 then C=j(ncol(x),1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; C=inv(x1`*(w#x1#wt1))*x1`#(w#wt1)`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbs=ginv(x1`*(w#x1#wt1));<br>
&nbsp; &nbsp; &nbsp; &nbsp; varb=C*C`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; /*** standard GWR variance ****/<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF &amp;WEIGHT NE %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbgg=varbs;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbgg=varb;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; <em>/*******************************/</em><br>
&nbsp; &nbsp; &nbsp; &nbsp; m1=(i-1)*ncol(x)+1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; m2=m1+(ncol(x)-1);<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,1]=i;<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,2]=b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,3]=POINTS[i,1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,4]=POINTS[i,2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbi[m1:m2,1]=vecdiag(varb);<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbigg[m1:m2,1]=vecdiag(varbgg);<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbis[m1:m2,]=varbs;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %if &amp;grid= %then %do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; if det(x1`*(w#x1#wt1))=0 then r=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; else <br>
&nbsp; &nbsp; &nbsp; &nbsp; r=x[i,]*C;<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhat[i]=x[i,]*b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; m1=(i-1)*ncol(x)+1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; m2=m1+(ncol(x)-1);<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,1]=i;<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,2]=b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; /** creating non-stationarity matrix **/<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;METHOD) ne ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; CCC=x1||w||wt1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; CCC1=CCC[1,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; if i=1 then CCC2=CCC;<br>
&nbsp; &nbsp; &nbsp; &nbsp; else if i=n then CCC2=CCC[2:i,]//CCC1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; else CCC2=CCC[2:i,]//CCC1//CCC[i+1:nrow(CCC),];<br>
&nbsp; &nbsp; &nbsp; &nbsp; 
BB[m1:m2,]=inv(CCC2[,1:ncol(x1)]`*(CCC2[,ncol(CCC)-1]#CCC2[,1:ncol(x1)]#CCC2[,ncol(CCC)]))*CCC2[,1:ncol(x1)]`#(CCC2[,ncol(CCC)-1]#CCC2[,ncol(CCC)])`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; <em>/*************************************/</em><br>
&nbsp; &nbsp; &nbsp; &nbsp; varbi[m1:m2,1]=vecdiag(varb);<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbigg[m1:m2,1]=vecdiag(varbgg);<br>
&nbsp; &nbsp; &nbsp; &nbsp; varbis[m1:m2,]=varbs;<br>
&nbsp; &nbsp; &nbsp; &nbsp; S[i]=r[1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; S2[i]=r*r`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; biT[i,1]=i;<br>
&nbsp; &nbsp; &nbsp; &nbsp; biT[i,2:ncol(x)+1]=b`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhati=x*b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhati1=yhati[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; yhati1=yhati1//yhati[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=1 to u;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; yhati1=yhati1//yhati[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; TSS=(y1#w#wt1)`*y1-((y1#w#wt1)[+]**2)/(w#wt1)[+];<br>
&nbsp; &nbsp; &nbsp; &nbsp; RSS=((y1-yhati1)#wt1)`*(w#(y1-yhati1));<br>
&nbsp; &nbsp; &nbsp; &nbsp; rsqri[i]=1-RSS/TSS;<br>
&nbsp; &nbsp; &nbsp; &nbsp; _w_=w;<br>
&nbsp; &nbsp; &nbsp; &nbsp; call sort(_w_,{1});<br>
&nbsp; &nbsp; &nbsp; &nbsp; sumwi[i]=_w_[1:int(nrow(_w_)*1)][+];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %end;<br>
/*<br>
*CALCULATING CORRELATION BETWEEN VARIABLE K AND L;<br>
we=w/sum(w);<br>
x1es=j(1,ncol(x1),0);<br>
do kk=1 to ncol(x1);<br>
x1es[kk]=sum(we#x1[,kk]);<br>
end;<br>
do kk=2 to ncol(x1)-1;<br>
do kkm=kk+1 to ncol(x1);<br>
if kk=2 then do;<br>
rikl[i,1+kkm-(kk+1)]=sum(we#(x1[,kk]-x1es[kk])#(x1[,kkm]-x1es[kkm]))/(sqrt(sum(we#((x1[,kk]-x1es[kk])#(x1[,kk]-x1es[kk]))))*sqrt(sum(we#((x1[,kkm]-x1es[kkm])#(x1[,kkm]-x1es[kkm])))));<br>
rikl[i,1+kkm-(kk+1)]=1/(1-rikl[i,1+kkm-(kk+1)]*rikl[i,1+kkm-(kk+1)]);<br>
end;<br>
else do;<br>
rikl[i,2*(kk-1)+kkm-(kk+1)]=sum(we#(x1[,kk]-x1es[kk])#(x1[,kkm]-x1es[kkm]))/(sqrt(sum(we#((x1[,kk]-x1es[kk])#(x1[,kk]-x1es[kk]))))*sqrt(sum(we#((x1[,kkm]-x1es[kkm])#(x1[,kkm]-x1es[kkm])))));<br>
rikl[i,2*(kk-1)+kkm-(kk+1)]=1/(1-rikl[i,2*(kk-1)+kkm-(kk+1)]*rikl[i,2*(kk-1)+kkm-(kk+1)]);<br>
end;<br>
end;<br>
end;<br>
*/<br>
<br>
/* Calculating VIF */<br>
if ncol(x)&gt;2 then do;<br>
do v=2 to ncol(x);<br>
&nbsp; &nbsp; &nbsp; &nbsp; if v=2 then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; yvif=x1[,v];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; xvif=x1[,v+1:ncol(x)];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; xv=x[,v+1:ncol(x)];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; else if v=ncol(x) then do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; yvif=x1[,v];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; xvif=x1[,2:v-1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; xv=x[,2:v-1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; else do;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; yvif=x1[,v];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; xvif=x1[,2:v-1]||x1[,v+1:ncol(x)];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; xv=x[,2:v-1]||x[,v+1:ncol(x)];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
if det(xvif`*(w#xvif))=0 then bvif=j(ncol(xvif),1,0);<br>
else<br>
bvif=inv(xvif`*(w#xvif))*xvif`*(w#yvif);<br>
yhatvi=xv*bvif;<br>
yhatvi1=yhatvi[i,];<br>
ymvif=x1[,v]-x1[:,v];<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ymvif1=ymvif;<br>
%END;<br>
%ELSE %DO;<br>
ymvif=x[,v]-x[:,v];<br>
ymvif1=ymvif[i,];<br>
do jj=1 to u;<br>
&nbsp; &nbsp; &nbsp; &nbsp; ymvif1=ymvif1//ymvif[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; yhatvi1=yhatvi1//yhatvi[dist[jj,2],];<br>
end;<br>
%END;<br>
TSS=ymvif1`*(w#ymvif1);<br>
RSS=(yvif-yhatvi1)`*(w#(yvif-yhatvi1));<br>
vif[i,v-1]=1/(1-(TSS-RSS)/TSS);<br>
end;<br>
end;<br>
else do;<br>
vif=repeat(1,n);<br>
end;<br>
/**********END VIF ************/<br>
if i=1 then W_f=W||(1:nrow(w))`;else W_f=W_f//(W||(1:nrow(w))`);<br>
end;<br>
create w_f from W_f;append from W_f;close w_f;free w_f;<br>
%if &amp;grid= %then %do;<br>
<br>
/***** incorporating survey design **************/<br>
v1=sum(S);<br>
*v11=sum(S);<br>
*v2=sum(S2);<br>
*v1=2*v11-v2;<br>
tab=y||x||wt;<br>
e=(y-yhat)#wt;<br>
e2=e#x;<br>
<br>
if nh&gt;0 &amp; ni&gt;0 then do;<br>
_h_=j(nrow(sh),1,0);<br>
_h_[1]=1;<br>
_i_=j(nrow(ci),1,0);<br>
_i_[1]=1;<br>
do k=2 to nrow(sh);<br>
if sh[k]=sh[k-1] then _h_[k]=_h_[k-1];<br>
else _h_[k]=_h_[k-1]+1;<br>
if ci[k]=ci[k-1] then _i_[k]=_i_[k-1];<br>
else _i_[k]=_i_[k-1]+1;<br>
end;<br>
tab=tab||_h_||_i_;<br>
&nbsp;ehi=j(1,ncol(x)+4,1);<br>
&nbsp;ehi[1,3:2+ncol(x)]=e2[1,];<br>
&nbsp;ehi[1]=tab[1,ncol(tab)-1];ehi[2]=tab[1,ncol(tab)];ehi[ncol(ehi)-1]=fh[1];<br>
&nbsp;do j=2 to nrow(tab);<br>
&nbsp; if tab[j,ncol(tab)-1]=tab[j-1,ncol(tab)-1] &amp; 
tab[j,ncol(tab)]=tab[j-1,ncol(tab)] then 
do;ehi[nrow(ehi),3:2+ncol(x)]=ehi[nrow(ehi),3:2+ncol(x)]+e2[j,];ehi[nrow(ehi),ncol(ehi)]=ehi[nrow(ehi),ncol(ehi)]+1;ehi[nrow(ehi),ncol(ehi)-1]=(ehi[nrow(ehi),ncol(ehi)-1]+fh[j])/2;end;<br>
&nbsp; else ehi=ehi//(tab[j,ncol(tab)-1]||tab[j,ncol(tab)]||e2[j,]||fh[j]||j(1,1,1));<br>
&nbsp;end;<br>
&nbsp;ee=ehi[,ncol(ehi)-1:ncol(ehi)];<br>
&nbsp;ehi=ehi[,1:ncol(ehi)-2]||j(nrow(ehi),3+ncol(x),0);ehi[1,3+ncol(x):2+2*ncol(x)]=ehi[1,3:2+ncol(x)];ehi[,ncol(ehi)-2:ncol(ehi)-1]=ee;ehi[1,ncol(ehi)]=ehi[1,ncol(ehi)-1];<br>
&nbsp;count=0;<br>
&nbsp;do j=2 to nrow(ehi);<br>
&nbsp; if ehi[j,1]=ehi[j-1,1] then 
do;ehi[j,3+ncol(x):2+2*ncol(x)]=ehi[j-1,3+ncol(x):2+2*ncol(x)]+ehi[j,3:2+ncol(x)];count=count+1;ehi[j,ncol(ehi)]=ehi[j-1,ncol(ehi)]+ehi[j,ncol(ehi)-1];ehi[j,ncol(ehi)-2]=ehi[j-1,ncol(ehi)-2];end;<br>
&nbsp; else do;<br>
&nbsp; &nbsp;if ehi[j-1,1]=1 then 
do;ehi[1:count+1,ncol(ehi)-1]=count+1;ehi[1:count+1,3+ncol(x):2+2*ncol(x)]=repeat(ehi[j-1,3+ncol(x):2+2*ncol(x)]/ehi[j-1,ncol(ehi)-1],count+1);in=ehi[j,2];ehi[1:count+1,ncol(ehi)]=repeat(ehi[j-1,ncol(ehi)],count+1);end;<br>
&nbsp; &nbsp;else 
do;ehi[in:in+count,ncol(ehi)-1]=count+1;ehi[in:in+count,3+ncol(x):2+2*ncol(x)]=repeat(ehi[j-1,3+ncol(x):2+2*ncol(x)]/ehi[j-1,ncol(ehi)-1],count+1);ehi[in:in+count,ncol(ehi)]=repeat(ehi[j-1,ncol(ehi)],count+1);in=ehi[j,2];end;<br>
&nbsp; &nbsp;ehi[j,3+ncol(x):2+2*ncol(x)]=ehi[j,3:2+ncol(x)];ehi[j,ncol(ehi)]=ehi[j,ncol(ehi)-1];count=0;<br>
&nbsp; end;<br>
&nbsp; if j=nrow(ehi) then do;<br>
&nbsp; 
&nbsp;ehi[in:in+count,ncol(ehi)-1]=count+1;ehi[in:in+count,3+ncol(x):2+2*ncol(x)]=repeat(ehi[j,3+ncol(x):2+2*ncol(x)]/ehi[j,ncol(ehi)-1],count+1);ehi[in:in+count,ncol(ehi)]=repeat(ehi[j,ncol(ehi)],count+1);in=ehi[j,2];<br>
&nbsp; end;<br>
&nbsp;end;<br>
print 'Design Summary',,'Number of Strata' (ehi[nrow(ehi),1]), 'Number of Clusters' (ehi[nrow(ehi),2]);<br>
*print ehi;<br>
do jj=1 to nrow(ehi);if ehi[jj,ncol(ehi)-1]=1 then 
do;ehi[jj,ncol(ehi)-1]=2;ehi[jj,3:2+ncol(x)]=0;ehi[jj,3+ncol(x):2+2*ncol(x)]=0;end;end;<br>
G=((n-1)/(n-v1))*(((ehi[,ncol(ehi)-1]/(ehi[,ncol(ehi)-1]-1))#(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]))`*(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]));<br>
%IF &amp;FPC NE %THEN %DO;<br>
G=((n-1)/(n-v1))*((((ehi[,ncol(ehi)-1]#(1-ehi[,ncol(ehi)-2]))/(ehi[,ncol(ehi)-1]-1))#(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]))`*(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]));<br>
%END;<br>
gl=ni-nh;<br>
print g;<br>
end;<br>
<br>
else if nh&gt;0 &amp; ni=0 then do;<br>
_h_=j(nrow(sh),1,0);<br>
_h_[1]=1;<br>
do k=2 to nrow(sh);<br>
if sh[k]=sh[k-1] then _h_[k]=_h_[k-1];<br>
else _h_[k]=_h_[k-1]+1;<br>
end;<br>
tab=tab||_h_;<br>
ehi=j(1,2*ncol(x)+3,0);<br>
&nbsp;ehi[1,2:1+ncol(x)]=e2[1,];<br>
&nbsp;ehi[1]=tab[1,ncol(tab)];ehi[ncol(ehi)-1]=fh[1];ehi[ncol(ehi)]=1;<br>
&nbsp;do j=2 to nrow(tab);<br>
&nbsp; if tab[j,ncol(tab)]=tab[j-1,ncol(tab)] then 
do;ehi[nrow(ehi),2:1+ncol(x)]=ehi[nrow(ehi),2:1+ncol(x)]+e2[j,];ehi[nrow(ehi),ncol(ehi)-1]=ehi[nrow(ehi),ncol(ehi)-1];ehi[nrow(ehi),ncol(ehi)]=ehi[nrow(ehi),ncol(ehi)]+1;end;<br>
&nbsp; else do;ehi=ehi//(tab[j,ncol(tab)]||e2[j,]||j(1,ncol(x),1)||fh[j]||j(1,1,1));end;<br>
&nbsp;end;<br>
&nbsp;ehi[,2+ncol(x):1+2*ncol(x)]=ehi[,2:1+ncol(x)]/ehi[,ncol(ehi)];<br>
do jj=1 to nrow(ehi);<br>
ehi2=ehi2//repeat(ehi[jj,2+ncol(x):3+2*ncol(x)],ehi[jj,ncol(ehi)]);<br>
end;<br>
eh=e2||ehi2[,1:ncol(ehi2)-1]||ehi2[,ncol(ehi2)];<br>
print 'Design Summary',,'Number of Strata' (ehi[&lt;&gt;,1]);<br>
*print eh;<br>
do jj=1 to nrow(eh);if eh[jj,ncol(eh)]=1 then eh[jj,ncol(eh)]=2;end;<br>
G=((n-1)/(n-v1))*(((eh[,ncol(eh)]/(eh[,ncol(eh)]-1))#(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]))`*(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]));<br>
%IF &amp;FPC NE %THEN %DO;<br>
G=((n-1)/(n-v1))*((((eh[,ncol(eh)]#(1-eh[,ncol(eh)-1]))/(eh[,ncol(eh)]-1))#(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]))`*(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]));<br>
%END;<br>
gl=nh-1;<br>
print g;<br>
end;<br>
<br>
else if ni&gt;0 &amp; nh=0 then do;<br>
_i_=j(nrow(ci),1,0);<br>
_i_[1]=1;<br>
do k=2 to nrow(ci);<br>
if ci[k]=ci[k-1] then _i_[k]=_i_[k-1];<br>
else _i_[k]=_i_[k-1]+1;<br>
end;<br>
tab=tab||_i_;<br>
ei=j(1,ncol(x)+3,0);<br>
&nbsp;ei[1,2:1+ncol(x)]=e2[1,];<br>
&nbsp;ei[1]=tab[1,ncol(tab)];ei[ncol(ei)]=fh[1];ei[1,ncol(ei)]=1;<br>
&nbsp;do j=2 to nrow(tab);<br>
&nbsp; if tab[j,ncol(tab)]=tab[j-1,ncol(tab)] then 
do;ei[nrow(ei),2:1+ncol(x)]=ei[nrow(ei),2:1+ncol(x)]+e2[j,];ei[nrow(ei),ncol(ei)]=(ei[nrow(ei),ncol(ei)]+fh[j])/2;ei[nrow(ei),ncol(ei)]=ei[nrow(ei),ncol(ei)]+1;end;<br>
&nbsp; else ei=ei//(tab[j,ncol(tab)]||e2[j,]||fh[j]||j(1,1,1));<br>
&nbsp;end;<br>
print 'Design Summary',,'Number of Clusters' (ei[&lt;&gt;,1]);<br>
*print ei;<br>
G=((n-1)/(n-v1))*(ei[&lt;&gt;,1]/(ei[&lt;&gt;,1]-1))*((ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)])`*(ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)]));<br>
%IF &amp;FPC NE %THEN %DO;<br>
G=((n-1)/(n-v1))*((((ei[&lt;&gt;,1]#(1-ei[,ncol(ei)-1]))/(ei[&lt;&gt;,1]-1))#(ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)]))`*(ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)]));<br>
%END;<br>
gl=ni-1;<br>
print G;<br>
end;<br>
<br>
else do;<br>
G=(n-1)/(n-v1)*(n/(n-1))*((e2-e2[:,])`*(e2-e2[:,]));<br>
gl=n-v1;<br>
*print g;<br>
end;<br>
create <u>_Gmatrix_</u> from G;append from G;close _Gmatrix_;<br>
<em>/***********************************************/</em><br>
v1=sum(S);<br>
*v11=sum(S);<br>
*v2=sum(S2);<br>
*v1=2*v11-v2;<br>
res=(y-yhat);<br>
rsqr1=(res#wt)`*res;<br>
ym=(y#wt)`*y;<br>
rsqr2=ym-((y#wt)[+]**2)/wt[+];<br>
rsqr=1-rsqr1/rsqr2;<br>
rsqradj=1-((n-1)/(n-v1))*(1-rsqr);<br>
sigma2=n*rsqr1/((n-v1)*wt[+]);<br>
root_mse=sqrt(sigma2);<br>
print sigma2[label='Sigma2e'] root_mse[label='Root MSE'] v1[label='#GWR 
parameters'] /*v11[label='#GWR parameters (model)'] v2[label='#GWR 
parameters (variance)']*/;<br>
influence=S;<br>
resstd=res/(sqrt(sigma2)*sqrt(abs(1-influence)));<br>
CooksD=resstd#resstd#influence/(v1*(1-influence));<br>
varbi=varbi*sigma2;<br>
/*** standard GWR variance ****/<br>
sigma2gg=rsqr1/(n-v1);<br>
varbigg=varbigg*sigma2gg;<br>
%IF &amp;FPC NE %THEN %DO;<br>
varbigg=varbigg*(1-n/wt[+]);<br>
%END;<br>
<em>/******************************/</em><br>
if ni&gt;0 | nh&gt;0 | wt[+] ^= n then do;<br>
do i=1 to m;<br>
m1=(i-1)*ncol(x)+1;<br>
m2=m1+(ncol(x)-1);<br>
varbi[m1:m2,1]=vecdiag(varbis[m1:m2,]*G*varbis[m1:m2,]);<br>
end;<br>
end;<br>
deffgwr=varbi/varbigg;<br>
stdbi=sqrt(varbi);<br>
tstat = bi[,2]/stdbi;<br>
probt=2*(1-probt(abs(tstat),gl));<br>
_malpha_=0.05*(ncol(x)/v1);<br>
_t_critical_=abs(tinv(_malpha_/2,gl));<br>
print rsqr[format=comma6.4 label="R-Square"] rsqradj[format=comma6.4 label="Adj-R-Square"];<br>
lnlcl=-n*log(rsqr1/n)/2-n*log(2*arcos(-1))/2-sum((y-yhat)#(y-yhat))/(2*(rsqr1/n));<br>
AIC=2*v1-2*lnlcl+((n+v1)/(n-2-v1));<br>
*AIC=2*n*log(rsqr1/n)+n*log(2*arcos(-1))+n*((n+v1)/(n-2-v1));<br>
print AIC lnlcl;<br>
_beta_=shape(bi[,1:2],n);<br>
i=do(2,ncol(_beta_),2);<br>
_beta_=_beta_[,i];<br>
call qntl(qntl,_beta_);<br>
qntl=qntl//(qntl[3,]-qntl[1,]);<br>
descriptb=_beta_[:,]//_beta_[&gt;&lt;,]//_beta_[&lt;&gt;,];<br>
<br>
print qntl[label="Quantiles of GWR Parameter Estimates" <br>
rowname={"P25", "P50", "P75", "IQR"} colname={'Intercept' &amp;xvar}],,<br>
descriptb[label="Descriptive Statistics" rowname={"Mean", "Min", "Max"} <br>
colname={'Intercept' &amp;xvar}];<br>
<br>
_stdbeta_=shape(stdbi,n);<br>
call qntl(qntls,_stdbeta_);<br>
qntls=qntls//(qntls[3,]-qntls[1,]);<br>
descripts=_stdbeta_[:,]//_stdbeta_[&gt;&lt;,]//_stdbeta_[&lt;&gt;,];<br>
<br>
print _malpha_[label="alpha-level=0.05"] _t_critical_[format=comma6.2 label="t-Critical"] gl;<br>
print qntls[label="Quantiles of GWR Standard Errors" <br>
rowname={"P25", "P50", "P75", "IQR"} colname={'Intercept' &amp;xvar}],,<br>
descripts[label="Descriptive Statistics of Standard Errors" rowname={"Mean", "Min", "Max"} <br>
colname={'Intercept' &amp;xvar}];<br>
<br>
_deffgwr_=shape(deffgwr,n);<br>
call qntl(qntldeff,_deffgwr_);<br>
descriptdeff=_deffgwr_[:,]//_deffgwr_[&gt;&lt;,]//_deffgwr_[&lt;&gt;,];<br>
<br>
print qntldeff[label="Quantiles of GWR Deff" <br>
rowname={"P25", "P50", "P75"} colname={'Intercept' &amp;xvar}],,<br>
descriptdeff[label="Descriptive Statistics of GWR Deff" rowname={"Mean", "Min", "Max"} <br>
colname={'Intercept' &amp;xvar}];<br>
%end;<br>
*print bi stdbi tstat probt influence cooksD;<br>
/****** Non-Stationarity Test *****************/<br>
%IF %UPCASE(&amp;METHOD) ne ADAPTIVE_BSQ %THEN %DO;<br>
BBk=j(n,n,0);<br>
Vk=j(ncol(x),1,0);<br>
gl1k=j(ncol(x),1,0);<br>
gl2k=j(ncol(x),1,0);<br>
do k=1 to ncol(x);<br>
ek=j(ncol(x),1,0);<br>
ek[k]=1;<br>
do i=1 to n;<br>
m1=(i-1)*ncol(x)+1;<br>
m2=m1+(ncol(x)-1);<br>
BBk[i,]=ek`*BB[m1:m2,];<br>
end;<br>
Vk[k]=y`*(1/n)*BBk`*(I(n)-(1/n)*J(n,n,1))*BBk*y;<br>
gl1k[k]=trace((1/n)*BBk`*(I(n)-(1/n)*J(n,n,1))*BBk);<br>
gl2k[k]=trace(((1/n)*BBk`*(I(n)-(1/n)*J(n,n,1))*BBk)**2);<br>
end;<br>
Fk=(Vk/gl1k)/sigma2;<br>
ndf=gl1k##2/gl2k;<br>
ddf=n-v1;<br>
ddf=repeat(ddf,ncol(x));<br>
probf=1-probf(Fk,ndf,ddf);<br>
print ,,"Non-Stationarity Test (Leung et al., 2000)",,<br>
Vk[label='' rowname={'Intercept' &amp;xvar} colname={"V"}] Fk[label=''&nbsp; colname={"F"}] ndf ddf <br>
probf[format=pvalue6. label="Pr &gt; F"];<br>
%END;<br>
<br>
/***** global estimates ***************/<br>
bg=inv(x`*(x#wt))*x`*(y#wt);<br>
s2g=((y-x*bg)#wt)`*(y-x*bg)/(n-nrow(bg));<br>
varg=vecdiag(inv(x`*(x#wt))*s2g);<br>
%IF &amp;FPC NE %THEN %DO;<br>
varg=vecdiag(inv(x`*(x#wt))*s2g)*(1-n/wt[+]);<br>
%end;<br>
/***** incorporating survey design **************/<br>
tab=y||x||wt;<br>
eg=(y-x*bg)#wt;<br>
e2g=eg#x;<br>
<br>
if nh&gt;0 &amp; ni&gt;0 then do;<br>
_h_=j(nrow(sh),1,0);<br>
_h_[1]=1;<br>
_i_=j(nrow(ci),1,0);<br>
_i_[1]=1;<br>
do k=2 to nrow(sh);<br>
if sh[k]=sh[k-1] then _h_[k]=_h_[k-1];<br>
else _h_[k]=_h_[k-1]+1;<br>
if ci[k]=ci[k-1] then _i_[k]=_i_[k-1];<br>
else _i_[k]=_i_[k-1]+1;<br>
end;<br>
tab=tab||_h_||_i_;<br>
&nbsp;ehi=j(1,ncol(x)+4,1);<br>
&nbsp;ehi[1,3:2+ncol(x)]=e2g[1,];<br>
&nbsp;ehi[1]=tab[1,ncol(tab)-1];ehi[2]=tab[1,ncol(tab)];ehi[ncol(ehi)-1]=fh[1];<br>
&nbsp;do j=2 to nrow(tab);<br>
&nbsp; if tab[j,ncol(tab)-1]=tab[j-1,ncol(tab)-1] &amp; 
tab[j,ncol(tab)]=tab[j-1,ncol(tab)] then 
do;ehi[nrow(ehi),3:2+ncol(x)]=ehi[nrow(ehi),3:2+ncol(x)]+e2g[j,];ehi[nrow(ehi),ncol(ehi)]=ehi[nrow(ehi),ncol(ehi)]+1;ehi[nrow(ehi),ncol(ehi)-1]=(ehi[nrow(ehi),ncol(ehi)-1]+fh[j])/2;end;<br>
&nbsp; else ehi=ehi//(tab[j,ncol(tab)-1]||tab[j,ncol(tab)]||e2g[j,]||fh[j]||j(1,1,1));<br>
&nbsp;end;<br>
&nbsp;ee=ehi[,ncol(ehi)-1:ncol(ehi)];<br>
&nbsp;ehi=ehi[,1:ncol(ehi)-2]||j(nrow(ehi),3+ncol(x),0);ehi[1,3+ncol(x):2+2*ncol(x)]=ehi[1,3:2+ncol(x)];ehi[,ncol(ehi)-2:ncol(ehi)-1]=ee;ehi[1,ncol(ehi)]=ehi[1,ncol(ehi)-1];<br>
&nbsp;count=0;<br>
&nbsp;do j=2 to nrow(ehi);<br>
&nbsp; if ehi[j,1]=ehi[j-1,1] then 
do;ehi[j,3+ncol(x):2+2*ncol(x)]=ehi[j-1,3+ncol(x):2+2*ncol(x)]+ehi[j,3:2+ncol(x)];count=count+1;ehi[j,ncol(ehi)]=ehi[j-1,ncol(ehi)]+ehi[j,ncol(ehi)-1];ehi[j,ncol(ehi)-2]=ehi[j-1,ncol(ehi)-2];end;<br>
&nbsp; else do;<br>
&nbsp; &nbsp;if ehi[j-1,1]=1 then 
do;ehi[1:count+1,ncol(ehi)-1]=count+1;ehi[1:count+1,3+ncol(x):2+2*ncol(x)]=repeat(ehi[j-1,3+ncol(x):2+2*ncol(x)]/ehi[j-1,ncol(ehi)-1],count+1);in=ehi[j,2];ehi[1:count+1,ncol(ehi)]=repeat(ehi[j-1,ncol(ehi)],count+1);end;<br>
&nbsp; &nbsp;else 
do;ehi[in:in+count,ncol(ehi)-1]=count+1;ehi[in:in+count,3+ncol(x):2+2*ncol(x)]=repeat(ehi[j-1,3+ncol(x):2+2*ncol(x)]/ehi[j-1,ncol(ehi)-1],count+1);ehi[in:in+count,ncol(ehi)]=repeat(ehi[j-1,ncol(ehi)],count+1);in=ehi[j,2];end;<br>
&nbsp; &nbsp;ehi[j,3+ncol(x):2+2*ncol(x)]=ehi[j,3:2+ncol(x)];ehi[j,ncol(ehi)]=ehi[j,ncol(ehi)-1];count=0;<br>
&nbsp; end;<br>
&nbsp; if j=nrow(ehi) then do;<br>
&nbsp; 
&nbsp;ehi[in:in+count,ncol(ehi)-1]=count+1;ehi[in:in+count,3+ncol(x):2+2*ncol(x)]=repeat(ehi[j,3+ncol(x):2+2*ncol(x)]/ehi[j,ncol(ehi)-1],count+1);ehi[in:in+count,ncol(ehi)]=repeat(ehi[j,ncol(ehi)],count+1);in=ehi[j,2];<br>
&nbsp; end;<br>
&nbsp;end;<br>
*print ehi;<br>
do jj=1 to nrow(ehi);if ehi[jj,ncol(ehi)-1]=1 then 
do;ehi[jj,ncol(ehi)-1]=2;ehi[jj,3:2+ncol(x)]=0;ehi[jj,3+ncol(x):2+2*ncol(x)]=0;end;end;<br>
Gg=((n-1)/(n-ncol(x)))*(((ehi[,ncol(ehi)-1]/(ehi[,ncol(ehi)-1]-1))#(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]))`*(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]));<br>
%IF &amp;FPC NE %THEN %DO;<br>
Gg=((n-1)/(n-ncol(x)))*((((ehi[,ncol(ehi)-1]#(1-ehi[,ncol(ehi)-2]))/(ehi[,ncol(ehi)-1]-1))#(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]))`*(ehi[,3:2+ncol(x)]-ehi[,3+ncol(x):2+2*ncol(x)]));<br>
%END;<br>
glg=ni-nh;<br>
end;<br>
<br>
else if nh&gt;0 &amp; ni=0 then do;<br>
_h_=j(nrow(sh),1,0);<br>
_h_[1]=1;<br>
do k=2 to nrow(sh);<br>
if sh[k]=sh[k-1] then _h_[k]=_h_[k-1];<br>
else _h_[k]=_h_[k-1]+1;<br>
end;<br>
tab=tab||_h_;<br>
ehi=j(1,2*ncol(x)+3,0);<br>
&nbsp;ehi[1,2:1+ncol(x)]=e2g[1,];<br>
&nbsp;ehi[1]=tab[1,ncol(tab)];ehi[ncol(ehi)-1]=fh[1];ehi[ncol(ehi)]=1;<br>
&nbsp;do j=2 to nrow(tab);<br>
&nbsp; if tab[j,ncol(tab)]=tab[j-1,ncol(tab)] then 
do;ehi[nrow(ehi),2:1+ncol(x)]=ehi[nrow(ehi),2:1+ncol(x)]+e2g[j,];ehi[nrow(ehi),ncol(ehi)-1]=ehi[nrow(ehi),ncol(ehi)-1];ehi[nrow(ehi),ncol(ehi)]=ehi[nrow(ehi),ncol(ehi)]+1;end;<br>
&nbsp; else do;ehi=ehi//(tab[j,ncol(tab)]||e2g[j,]||j(1,ncol(x),1)||fh[j]||j(1,1,1));end;<br>
&nbsp;end;<br>
&nbsp;ehi[,2+ncol(x):1+2*ncol(x)]=ehi[,2:1+ncol(x)]/ehi[,ncol(ehi)];<br>
do jj=1 to nrow(ehi);<br>
ehi2=ehi2//repeat(ehi[jj,2+ncol(x):3+2*ncol(x)],ehi[jj,ncol(ehi)]);<br>
end;<br>
eh=e2g||ehi2[,1:ncol(ehi2)-1]||ehi2[,ncol(ehi2)];<br>
do jj=1 to nrow(eh);if eh[jj,ncol(eh)]=1 then eh[jj,ncol(eh)]=2;end;<br>
Gg=((n-1)/(n-ncol(x)))*(((eh[,ncol(eh)]/(eh[,ncol(eh)]-1))#(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]))`*(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]));<br>
%IF &amp;FPC NE %THEN %DO;<br>
Gg=((n-1)/(n-ncol(x)))*((((eh[,ncol(eh)]#(1-eh[,ncol(eh)-1]))/(eh[,ncol(eh)]-1))#(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]))`*(eh[,1:ncol(x)]-eh[,1+ncol(x):2*ncol(x)]));<br>
%END;<br>
glg=nh-1;<br>
end;<br>
<br>
else if ni&gt;0 &amp; nh=0 then do;<br>
_i_=j(nrow(ci),1,0);<br>
_i_[1]=1;<br>
do k=2 to nrow(ci);<br>
if ci[k]=ci[k-1] then _i_[k]=_i_[k-1];<br>
else _i_[k]=_i_[k-1]+1;<br>
end;<br>
tab=tab||_i_;<br>
ei=j(1,ncol(x)+3,0);<br>
&nbsp;ei[1,2:1+ncol(x)]=e2g[1,];<br>
&nbsp;ei[1]=tab[1,ncol(tab)];ei[ncol(ei)]=fh[1];ei[1,ncol(ei)]=1;<br>
&nbsp;do j=2 to nrow(tab);<br>
&nbsp; if tab[j,ncol(tab)]=tab[j-1,ncol(tab)] then 
do;ei[nrow(ei),2:1+ncol(x)]=ei[nrow(ei),2:1+ncol(x)]+e2[j,];ei[nrow(ei),ncol(ei)]=(ei[nrow(ei),ncol(ei)]+fh[j])/2;ei[nrow(ei),ncol(ei)]=ei[nrow(ei),ncol(ei)]+1;end;<br>
&nbsp; else ei=ei//(tab[j,ncol(tab)]||e2g[j,]||fh[j]||j(1,1,1));<br>
&nbsp;end;<br>
Gg=((n-1)/(n-ncol(x)))*(ei[&lt;&gt;,1]/(ei[&lt;&gt;,1]-1))*((ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)])`*(ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)]));<br>
%IF &amp;FPC NE %THEN %DO;<br>
Gg=((n-1)/(n-ncol(x)))*((((ei[&lt;&gt;,1]#(1-ei[,ncol(ei)-1]))/(ei[&lt;&gt;,1]-1))#(ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)]))`*(ei[,2:1+ncol(x)]-ei[+,2:1+ncol(x)]/ei[+,ncol(ei)]));<br>
%END;<br>
glg=ni-1;<br>
end;<br>
<br>
else do;<br>
Gg=(n-1)/(n-ncol(x))*(n/(n-1))*((e2g-e2g[:,])`*(e2g-e2g[:,]));<br>
glg=n-1;<br>
end;<br>
<br>
vargd=vecdiag(inv(x`*(x#wt))*Gg*inv(x`*(x#wt)));<br>
%IF &amp;WEIGHT = %THEN %DO;<br>
vargd=varg;<br>
glg=n-ncol(x);<br>
%END;<br>
deff=vargd/varg;<br>
stdg=sqrt(vargd);<br>
tg=bg/stdg;<br>
probtg=2*(1-probt(abs(tg),glg));<br>
bg_stdg=bg||stdg;<br>
print "Global Parameter Estimates",,bg_stdg[label=' ' rowname={'Intercept' &amp;xvar}<br>
colname={"Par. Est." "Std Error"}] tg[format=comma6.2 label="t Value"] probtg[format=pvalue6. label="Pr &gt; |t|"]<br>
deff[format=comma6.2 label="Deff"],,<br>
"NOTE: The denominator degrees of freedom for the t tests is" glg[label=' ']".";<br>
resg=(y-x*bg);<br>
rsqr1g=(resg#wt)`*resg;<br>
ymg=(y#wt)`*y;<br>
rsqr2g=ymg-((y#wt)[+]**2)/wt[+];<br>
rsqrg=1-rsqr1g/rsqr2g;<br>
rsqradjg=1-((n-1)/(n-ncol(x)))*(1-rsqrg);<br>
sigma2g=n*rsqr1g/((n-ncol(x))*wt[+]);<br>
root_mseg=sqrt(sigma2g);<br>
print sigma2g[label='Sigma2e'] root_mseg[label='Root MSE'];<br>
print rsqrg[format=comma6.4 label="R-Square"] rsqradjg[format=comma6.4 label="Adj-R-Square"];<br>
<br>
<em>/****************************************/</em><br>
%if &amp;grid= %then %do;<br>
create <u>_res_</u> var{wt y yhat res resstd rsqri influence cooksD sumwi};<br>
append;<br>
create <u>_beta_</u> from bi[colname={"id" "B"}];<br>
append from bi;<br>
bistdt=bi||stdbi||tstat||probt||deffgwr;<br>
create <u>_parameters_</u> from bistdt[colname={"id" "B" "x" "y" "stdbi" "tstat" "probt" "GWRDeff"}];<br>
append from bistdt;<br>
_tstat_=_beta_/_stdbeta_;<br>
_probt_=2*(1-probt(abs(_tstat_),gl));<br>
_sig_=j(n,ncol(x),"not significant at 90%");<br>
v1=sum(S);<br>
do i=1 to n;<br>
do j=1 to ncol(x);<br>
if _probt_[i,j]&lt;0.01*(ncol(x)/v1) then _sig_[i,j]="significant at 99%";<br>
else if _probt_[i,j]&lt;0.05*(ncol(x)/v1) then _sig_[i,j]="significant at 95%";<br>
else if _probt_[i,j]&lt;0.1*(ncol(x)/v1) then _sig_[i,j]="significant at 90%";<br>
else _sig_[i,j]="not significant at 90%";<br>
end;<br>
end;<br>
_bistdt_=COORD||_beta_||_stdbeta_||_tstat_||_probt_||_deffgwr_;<br>
_colname1_={"Intercept" &amp;xvar};<br>
_label_=repeat("std_",ncol(x))//repeat("tstat_",ncol(x))//repeat("probt_",ncol(x))//repeat("GWRDeff_",ncol(x));<br>
_colname_={"x" "y"}||_colname1_||concat(_label_,repeat(_colname1_`,4))`;<br>
call change(_colname_, "_ ", "_");<br>
call change(_colname_, "_ ", "_");<br>
create <u>_parameters2_</u> from _bistdt_[colname=_colname_];<br>
append from _bistdt_;<br>
close _parameters2_;<br>
_label_=repeat("sig_",ncol(x));<br>
_colname_=concat(_label_,repeat(_colname1_`,1))`;<br>
create _sig_parameters2_ from _sig_[colname=_colname_];<br>
append from _sig_;<br>
create <u>_vif_</u> from vif[colname={&amp;xvar}];<br>
append from vif;<br>
%end;<br>
%else %do;<br>
create <u>_beta_</u> from bi[colname={"id" "B" "x" "y"}];<br>
append from bi;<br>
bistdt=bi||varbi;<br>
create <u>_parameters_</u> from bistdt[colname={"id" "B" "x" "y" "varbi"}];<br>
append from bistdt;<br>
close _parameters_;<br>
%if &amp;sigma2 ne and &amp;GMATRIX = %then %do;<br>
varbi=varbi*&amp;sigma2;<br>
varbiggg=varbigg*&amp;sigma2*wt[+]/n;<br>
%IF &amp;FPC NE %THEN %DO;<br>
varbiggg=varbiggg*(1-n/wt[+]);<br>
%END;<br>
deffgwr=varbi/varbiggg;<br>
stdbi=sqrt(varbi);<br>
tstat=bi[,2]/stdbi;<br>
do i=1 to nrow(tstat);<br>
if tstat[i]=. then tstat[i]=0;<br>
end;<br>
bistdt=bi||stdbi||tstat||deffgwr;<br>
create <u>_parameters_</u> from bistdt[colname={"id" "B" "x" "y" "stdbi" "tstat" "GWRDeff"}];<br>
append from bistdt;<br>
close _parameters_;<br>
%end;<br>
%else %if &amp;sigma2 = and &amp;GMATRIX ne %then %do;<br>
use &amp;GMATRIX;read all into G;close &amp;GMATRIX;<br>
if ni&gt;0 | nh&gt;0 then do;<br>
do i=1 to m;<br>
m1=(i-1)*ncol(x)+1;<br>
m2=m1+(ncol(x)-1);<br>
varbi[m1:m2,1]=vecdiag(varbis[m1:m2,]*G*varbis[m1:m2,]);<br>
end;<br>
end;<br>
stdbi=sqrt(varbi);<br>
tstat=bi[,2]/stdbi;<br>
do i=1 to nrow(tstat);<br>
if tstat[i]=. then tstat[i]=0;<br>
end;<br>
bistdt=bi||stdbi||tstat;<br>
create <u>_parameters_</u> from bistdt[colname={"id" "B" "x" "y" "stdbi" "tstat"}];<br>
append from bistdt;<br>
close _parameters_;<br>
%end;<br>
%else %if &amp;sigma2 ne and &amp;GMATRIX ne %then %do;<br>
use &amp;GMATRIX;read all into G;close &amp;GMATRIX;<br>
if ni&gt;0 | nh&gt;0 then do;<br>
do i=1 to m;<br>
m1=(i-1)*ncol(x)+1;<br>
m2=m1+(ncol(x)-1);<br>
varbi[m1:m2,1]=vecdiag(varbis[m1:m2,]*G*varbis[m1:m2,]);<br>
end;<br>
end;<br>
varbiggg=varbigg*&amp;sigma2*wt[+]/n;<br>
%IF &amp;FPC NE %THEN %DO;<br>
varbiggg=varbiggg*(1-n/wt[+]);<br>
%END;<br>
deffgwr=varbi/varbiggg;<br>
stdbi=sqrt(varbi);<br>
tstat=bi[,2]/stdbi;<br>
do i=1 to nrow(tstat);<br>
if tstat[i]=. then tstat[i]=0;<br>
end;<br>
bistdt=bi||stdbi||tstat||deffgwr;<br>
create <u>_parameters_</u> from bistdt[colname={"id" "B" "x" "y" "stdbi" "tstat" "GWRDeff"}];<br>
append from bistdt;<br>
close _parameters_;<br>
%end;<br>
%end;<br>
quit;<br>
%mend GWR;<br>
<br>
<em>/*****************************************************************************************/</em><br>
/* Macro to separe the datasets of GWR Grid Estimates*/<br>
/* REQUIRED PARAMETERS<br>
/*&nbsp; &nbsp; &nbsp;PAR = the number of explicative variables without considers the intercept<br>
<em>/*****************************************************************************************/</em><br>
<br>
%macro MAP(PAR=);<br>
proc iml;<br>
use _parameters_;<br>
read all into p[colname=names];<br>
var=1+&amp;par;<br>
n=nrow(p)/2;<br>
nvar=ncol(p);<br>
_beta_=shape(p,n);<br>
label=names;<br>
do i=1 to var;<br>
seq1=(i-1)*nvar+1;<br>
seq2=seq1+(nvar-1);<br>
name="B0":"B&amp;par";<br>
_betas_=_beta_[,seq1:seq2];<br>
create b from _betas_[colname=label];<br>
append from _betas_;<br>
close b;<br>
sastables=datasets("work");<br>
do j=1 to nrow(sastables);<br>
if sastables[j]=name[i] then <br>
call delete(name[i]);<br>
end; <br>
call rename(b,name[i]);<br>
end;<br>
quit;<br>
%mend MAP;<br>
<br>
<em>/*****************************************************************************************/</em><br>
/* Macro for Drawing the Maps in 2D */<br>
/* REQUIRED PARAMETERS<br>
/*&nbsp; TABLE = the name of the SAS data set to be used<br>
/*&nbsp; &nbsp; VAR = the name of the variable to be plotted<br>
/*&nbsp; &nbsp; MAP = the name of the SAS data set with the geographic coordinates<br>
/*&nbsp; SAMPLEDATA = the name of the SAS data set with the geographic coordinates of the sample<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;data<br>
/* METHOD = there are two choices to define the classes:<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; EQUAL: using the same rule of histograms (the number of classes is defined by<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;Sturges's ruke)<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; STD: using the standard deviation for creating 6 classes: media-3std;<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;media-2std;media-1std;media+1std;media+2std;media+3std;<br>
/*&nbsp; WHERE = using a conditional to draw the maps (for example, plot only the parameters that<br>
/*&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; are significant at 5% level)<br>
<em>/*****************************************************************************************/</em><br>
<br>
<br>
%macro map2d(table=,var=,map=,sampledata=,method=EQUAL,where=);<br>
data anno&amp;table;set &amp;table;<br>
length function style $10. color $8.;<br>
retain line 1 xsys ysys '2' hsys '3' color 'red';<br>
function='label';text='U';position='5';style='marker';size=2;<br>
&amp;where;<br>
run;<br>
proc sql noprint;<br>
select count(*) into:np from anno&amp;table where &amp;var&gt;=0;<br>
select count(*) into:nn from anno&amp;table where &amp;var&lt;0;<br>
select min(&amp;var) into:minp from anno&amp;table where &amp;var&gt;=0;<br>
select max(&amp;var) into:maxp from anno&amp;table where &amp;var&gt;=0;<br>
select min(&amp;var) into:minn from anno&amp;table where &amp;var&lt;0;<br>
select max(&amp;var) into:maxn from anno&amp;table where &amp;var&lt;0;<br>
quit;<br>
%put &amp;np &amp;nn;<br>
%if &amp;np &gt;0 and &amp;nn = 0 %then %do;<br>
data _null_;<br>
np=floor(1+3.3*log10(&amp;np))-1;<br>
call symput('np',np);<br>
run;<br>
%put &amp;np;<br>
%if %upcase(&amp;method)=EQUAL %then %do;<br>
%put &amp;np &amp;minp &amp;maxp %sysevalf((&amp;maxp-&amp;minp)/&amp;np);<br>
%colorscale(FFFFFF,,FF3333,&amp;np,clist,no);%patt;<br>
data _null_;set clist;<br>
call symput('color'||trim(left(_n_)),'cx'||rgb);<br>
run;<br>
%macro cl;<br>
data anno&amp;table;set anno&amp;table;<br>
if round(&amp;var,0.01)&lt;=round(&amp;minp+(&amp;maxp-&amp;minp)/&amp;np,0.01) then color="&amp;color1";<br>
%do i=2 %to &amp;np;<br>
else if round(&amp;var,0.01)&lt;=&amp;minp+round(&amp;i*(&amp;maxp-&amp;minp)/&amp;np,0.01) then color="&amp;&amp;color&amp;i";<br>
%end;<br>
run;<br>
%mend cl;<br>
%cl;<br>
%end;<br>
%else %if %upcase(&amp;method)=STD %then %do;<br>
data _null_;<br>
np=6;<br>
call symput('np',np);<br>
run;<br>
proc sql noprint;<br>
select mean(&amp;var) into:meanp from anno&amp;table where &amp;var&gt;=0;<br>
select std(&amp;var) into:stdp from anno&amp;table where &amp;var&gt;=0;<br>
quit;<br>
%put &amp;meanp &amp;stdp;<br>
%colorscale(FFFFFF,,FF3333,&amp;np,clist,no);%patt;<br>
data _null_;set clist;<br>
call symput('color'||trim(left(_n_)),'cx'||rgb);<br>
run;<br>
%macro cl;<br>
data anno&amp;table;set anno&amp;table;<br>
if round(&amp;var,0.01)&lt;=round(&amp;minp+(&amp;meanp-3*&amp;stdp),0.01) then color="&amp;color1";<br>
else if round(&amp;var,0.01)&lt;=&amp;minp+round(&amp;meanp-2*&amp;stdp,0.01) then color="&amp;color2";<br>
else if round(&amp;var,0.01)&lt;=&amp;minp+round(&amp;meanp-&amp;stdp,0.01) then color="&amp;color3";<br>
else if round(&amp;var,0.01)&lt;=&amp;minp+round(&amp;meanp+&amp;stdp,0.01) then color="&amp;color4";<br>
else if round(&amp;var,0.01)&lt;=&amp;minp+round(&amp;meanp+2*&amp;stdp,0.01) then color="&amp;color5";<br>
else color="&amp;color6";<br>
run;<br>
%mend cl;<br>
%cl;<br>
%end;<br>
data _null_;<br>
min=left(trim(putn(round(&amp;minp,0.1),'commax10.')));<br>
max=left(trim(putn(round(&amp;maxp,0.1),'commax10.')));<br>
call symput('min',min);<br>
call symput('max',max);<br>
run;<br>
%put &amp;min &amp;max;<br>
%bar(FF3333,FFFFFF,&amp;minp,&amp;maxp,vertical,y_i=30,x_i=90);<br>
%end;<br>
%else %if &amp;nn &gt; 0 and &amp;np = 0 %then %do;<br>
data _null_;<br>
nn=floor(1+3.3*log10(&amp;nn))-1;<br>
call symput('nn',nn);<br>
run;<br>
%put &amp;nn;<br>
%put &amp;nn &amp;minn &amp;maxn %sysevalf((&amp;maxn-&amp;minn)/&amp;nn);<br>
%colorscale(FFFFFF,,3333FF,&amp;nn,clist,no);%patt;<br>
data _null_;set clist;<br>
call symput('color'||trim(left(_n_)),'cx'||rgb);<br>
run;<br>
%macro cl;<br>
data anno&amp;table;set anno&amp;table;<br>
if round(&amp;var,0.01)&lt;=round(&amp;minn+(&amp;maxn-&amp;minn)/&amp;nn,0.01) then color="&amp;color1";<br>
%do i=2 %to &amp;nn;<br>
else if round(&amp;var,0.01)&lt;=&amp;minn+round(&amp;i*(&amp;maxn-&amp;minn)/&amp;nn,0.01) then color="&amp;&amp;color&amp;i";<br>
%end;<br>
run;<br>
%mend cl;<br>
%cl;<br>
data _null_;<br>
min=left(trim(putn(round(&amp;minn,0.1),'commax10.')));<br>
max=left(trim(putn(round(&amp;maxn,0.1),'commax10.')));<br>
call symput('min',min);<br>
call symput('max',max);<br>
run;<br>
%put &amp;min &amp;max;<br>
%bar(FF3333,3333FF,&amp;minn,&amp;maxn,vertical,y_i=30,x_i=90);<br>
%end;<br>
%else %do;<br>
data _null_;<br>
np=floor(1+3.3*log10(&amp;np))-1;<br>
nn=floor(1+3.3*log10(&amp;nn))-1;<br>
call symput('np',np);<br>
call symput('nn',nn);<br>
run;<br>
%put &amp;np &amp;nn;<br>
%put &amp;np &amp;minp &amp;maxp %sysevalf((&amp;maxp-&amp;minp)/&amp;np);<br>
%put &amp;nn &amp;minn &amp;maxn %sysevalf((&amp;maxn-&amp;minn)/&amp;nn);<br>
%colorscale(FFFFFF,,FF3333,&amp;np,clist,no);%patt;<br>
data _null_;set clist;<br>
call symput('color'||trim(left(_n_)),'cx'||rgb);<br>
run;<br>
%macro cl;<br>
data anno&amp;table.p;set anno&amp;table(where=(&amp;var&gt;=0));<br>
if round(&amp;var,0.01)&lt;=round(&amp;minp+(&amp;maxp-&amp;minp)/&amp;np,0.01) then color="&amp;color1";<br>
%do i=2 %to &amp;np;<br>
else if round(&amp;var,0.01)&lt;=&amp;minp+round(&amp;i*(&amp;maxp-&amp;minp)/&amp;np,0.01) then color="&amp;&amp;color&amp;i";<br>
%end;<br>
run;<br>
%mend cl;<br>
%cl;<br>
%colorscale(3333FF,,FFFFFF,&amp;nn,clist,no);%patt;<br>
data _null_;set clist;<br>
call symput('color'||trim(left(_n_)),'cx'||rgb);<br>
run;<br>
%macro cl;<br>
data anno&amp;table.n;set anno&amp;table(where=(&amp;var&lt;0));<br>
if round(&amp;var,0.01)&lt;=round(&amp;minn+(&amp;maxn-&amp;minn)/&amp;nn,0.01) then color="&amp;color1";<br>
%do i=2 %to &amp;nn;<br>
else if round(&amp;var,0.01)&lt;=&amp;minn+round(&amp;i*(&amp;maxn-&amp;minn)/&amp;nn,0.01) then color="&amp;&amp;color&amp;i";<br>
%end;<br>
run;<br>
%mend cl;<br>
%cl;<br>
data anno&amp;table;set anno&amp;table.p anno&amp;table.n;run;<br>
data _null_;<br>
min=left(trim(putn(round(&amp;minn,0.1),'commax10.')));<br>
max=left(trim(putn(round(&amp;maxp,0.1),'commax10.')));<br>
call symput('min',min);<br>
call symput('max',max);<br>
run;<br>
%put &amp;min &amp;max;<br>
%bar(FF3333,FFFFFF,0,&amp;maxp,vertical,y_i=50,x_i=90);<br>
data anno&amp;table;set <u>_a_</u> anno&amp;table;run;<br>
%bar(FFFFFF,3333FF,&amp;minn,0,vertical,y_i=16,x_i=90);<br>
%end;<br>
data anno&amp;table;set <u>_a_</u> anno&amp;table;run;<br>
data a;id=0;v=2;run;<br>
<br>
%if &amp;sampledata ne %then %do;<br>
data annodata;set &amp;sampledata;<br>
length function style $10. color $8.;<br>
retain line 1 xsys ysys '2' hsys '3' color 'black' when 'a';<br>
function='label';text='J';position='5';style='special';size=2;<br>
run;<br>
%end;<br>
*goptions reset=all reset=global;<br>
proc gmap data=a map=&amp;map all %if &amp;sampledata ne %then %do;anno=annodata%end;;<br>
id id;<br>
choro v / anno=anno&amp;table nolegend;<br>
run;<br>
quit;<br>
%mend map2d;<br>
<br>
/************** macro for stationarity test *************/<br>
<br>
%macro stationarity(DATA=,YVAR=,XVAR=,WEIGHT=,DCOORD=,MAXV=,METHOD=,DISTANCEKM=NO,H=,REP=99);<br>
proc iml;<br>
use &amp;DATA;<br>
read all var {&amp;YVAR} into y;<br>
read all var {&amp;XVAR} into x;<br>
n=nrow(y);<br>
wt=j(n,1,1);<br>
%IF &amp;WEIGHT NE %THEN %DO;<br>
read all var {&amp;WEIGHT} into wt;<br>
%END;<br>
close &amp;DATA;<br>
<br>
%IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; use &amp;DHV;read all into hv;<br>
%END;<br>
x=j(n,1,1)||x;<br>
<br>
%IF &amp;H NE %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; h=&amp;H;<br>
&nbsp; &nbsp; &nbsp; &nbsp; print h[label="Bandwidth"];<br>
%END;<br>
%ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;METHOD)=FIXED_G or 
%UPCASE(&amp;METHOD)=FIXED_BSQ or %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ 
%THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; h=&amp;_h_;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; print h[label="Bandwidth"];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
%END;<br>
<br>
use &amp;DCOORD;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br>
read all var{x y} into COORD;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; read all var{x y} into POINTS;&nbsp; &nbsp; &nbsp;<br>
close &amp;DCOORD;<br>
m=nrow(POINTS);<br>
<br>
bi=j(ncol(x)*m,4,0);<br>
biT=j(m,ncol(x)+1,0);<br>
ym=y-y[:];<br>
<br>
*** Basic formulae for calculating spherical distance *** ;<br>
do r=1 to &amp;REP;<br>
do i=1 to m;<br>
free dist;<br>
&nbsp; &nbsp; &nbsp; &nbsp; d=j(1,3,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; do j=1 to n;&nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dif=abs(POINTS[i,1]-COORD[j,1]);&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; raio=arcos(-1)/180;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
argument=sin(POINTS[i,2]*raio)*sin(COORD[j,2]*raio)+cos(POINTS[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio);
 <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if argument&gt;1 then arco=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; /* Law of Cosines */&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 
arco=arcos(sin(POINTS[i,2]*raio)*sin(COORD[j,2]*raio)+cos(POINTS[i,2]*raio)*cos(COORD[j,2]*raio)*cos(dif*raio));<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=arco*6371 /*Earth's Radius = 6371 (aproximately)*/;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=1e-3 then d1=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d1=sqrt((POINTS[i,1]-COORD[j,1])**2+(POINTS[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=&amp;MAXV*1 &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=FIXED_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=h &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1&lt;=hv[i] &amp; d1 ^=0 then do;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if d1 ^=0 then do;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[1]=i;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[2]=j;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;DISTANCEKM)=YES %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=arco*6371 /*Earth's Radius = 6371 (approximately)*/;&nbsp; &nbsp;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %ELSE %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; d[3]=sqrt((POINTS[i,1]-COORD[j,1])**2+(POINTS[i,2]-COORD[j,2])**2);<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; dist=dist//d;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; u=nrow(dist);<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=j(u,1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym1=ym[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=1 to u;<br>
%IF %UPCASE(&amp;METHOD)=FIXED_G %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=exp(-(dist[jj,3]/h)**2);<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=FIXED_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=(1-(dist[jj,3]/h)**2)**2;<br>
%END;<br>
%ELSE %IF %UPCASE(&amp;METHOD)=ADAPTIVEN %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj]=(1-(dist[jj,3]/hv[i])**2)**2;<br>
%END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; ym1=ym1//ym[dist[jj,2],];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; w={1}//w;<br>
&nbsp; &nbsp; &nbsp; &nbsp; %IF %UPCASE(&amp;METHOD)=ADAPTIVE_BSQ %THEN %DO;<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym1=ym[i,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; call sort(dist,{3});<br>
&nbsp; &nbsp; &nbsp; &nbsp; dist=dist||(1:nrow(dist))`;<br>
&nbsp; &nbsp; &nbsp; &nbsp; w=j(n,2,0);&nbsp; &nbsp; &nbsp; &nbsp;&nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; hn=dist[h,3];<br>
&nbsp; &nbsp; &nbsp; &nbsp; do jj=1 to n-1;<br>
&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; if dist[jj,4]&lt;=h then<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,1]=(1-(dist[jj,3]/hn)**2)**2;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; else w[jj,1]=0;<br>
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; w[jj,2]=dist[jj,2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; end;<br>
&nbsp; &nbsp; &nbsp; &nbsp; position=w[loc(w[,1]&gt;0),2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; w={1}//w[position,1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; x1=x1//x[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; y1=y1//y[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; wt1=wt1//wt[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym1=ym1//ym[position,];<br>
&nbsp; &nbsp; &nbsp; &nbsp; %END;<br>
&nbsp; &nbsp; &nbsp; &nbsp; <br>
&nbsp; &nbsp; &nbsp; &nbsp; if det(x1`*(w#x1#wt1))=0 then b=j(ncol(x),1,0);<br>
&nbsp; &nbsp; &nbsp; &nbsp; else<br>
&nbsp; &nbsp; &nbsp; &nbsp; b=inv(x1`*(w#x1#wt1))*x1`*(w#y1#wt1);<br>
<br>
&nbsp; &nbsp; &nbsp; &nbsp; <em>/*******************************/</em><br>
&nbsp; &nbsp; &nbsp; &nbsp; m1=(i-1)*ncol(x)+1;<br>
&nbsp; &nbsp; &nbsp; &nbsp; m2=m1+(ncol(x)-1);<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,1]=i;<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,2]=b;<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,3]=POINTS[i,1];<br>
&nbsp; &nbsp; &nbsp; &nbsp; bi[m1:m2,4]=POINTS[i,2];<br>
end;<br>
_beta_=shape(bi[,1:2],n);<br>
i=do(2,ncol(_beta_),2);<br>
_beta_=_beta_[,i];<br>
<br>
do i=1 to ncol(_beta_);<br>
vk=(_beta_[,i]-_beta_[:,i])`*(_beta_[,i]-_beta_[:,i])/n;<br>
vr=vr||vk;<br>
end;<br>
if r=1 then v=vr;else v=v//vr;<br>
&nbsp; &nbsp; ru = 1:n;<br>
&nbsp; &nbsp; &nbsp; &nbsp; *call randseed(r); <br>
&nbsp; &nbsp; call randgen(ru, "Uniform");<br>
&nbsp; &nbsp; &nbsp; &nbsp; y=ru`||y;<br>
&nbsp; &nbsp; &nbsp; &nbsp; x=ru`||x;<br>
&nbsp; &nbsp; call sort(y,{1});<br>
&nbsp; &nbsp; &nbsp; &nbsp; call sort(x,{1});<br>
&nbsp; &nbsp; &nbsp; &nbsp; y=y[,2];<br>
&nbsp; &nbsp; &nbsp; &nbsp; x=x[,2:ncol(x)];<br>
&nbsp; &nbsp; &nbsp; &nbsp; ym=y-y[:];<br>
&nbsp; &nbsp; &nbsp; &nbsp; free vr vk;<br>
end;<br>
<br>
pvalor=j(1,ncol(v),0);<br>
do var=1 to ncol(v);<br>
do i=1 to nrow(v);<br>
if v[i,var]&gt;=v[1,var] then pvalor[var]=pvalor[var]+1;<br>
end;<br>
end;<br>
pvalor=pvalor/nrow(v);<br>
_colname_={"Intercept" &amp;xvar};<br>
print pvalor[label="Nonstationarity test (p-value)" colname=_colname_];<br>
<br>
create <u>_stationarity_</u> from v[colname=_colname_];<br>
append from v;<br>
quit;<br>
%mend stationarity;</div></body></html>

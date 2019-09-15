/****************************************************/
/*               Title :  greeting					*/
/*     Execute Command :  nil						*/
/*              Author :  Vivien Luo                */
/*       Creation Date :  2013-05-10                */
/*              Update :  							*/
/*            Web site :  www.allegro-skill.com     */
/*		   Description :  							*/
/*	When each time you start allegro ,the warm 		*/
/*	greetings displayed in the command line			*/
/****************************************************/
let((CurTime CurHour savedTime savedMessage i isDate )
CurTime = timeToTm(stringToTime(getCurrentTime())) ;get the time as formate: month day hour:minute:second year
;Create date as formate :"1231"
if((CurTime->tm_mon+1) < 10
	then
		CurMonth = strcat("0" sprintf(nil "%d" (CurTime->tm_mon+1)))
	else
		CurMonth = sprintf(nil "%d" (CurTime->tm_mon+1))
)
if((CurTime->tm_mday) < 10
	then
		Curday = strcat("0" sprintf(nil "%d" (CurTime->tm_mday)))
	else
		Curday = sprintf(nil "%d" (CurTime->tm_mday))
)

Curdate = strcat(CurMonth Curday)
CurHour = CurTime->tm_hour
inPort = infile("greeting")
if(inPort
	then
	DateMessage = ""
	isDate = nil
	endFile = t
	while(DateMessage != "#Below is information about Hour\n"
		DateMessage = gets(line inPort)
		DateInfo = parseString(DateMessage)
		when(car(DateInfo) == Curdate
			axlUIWPrint(nil "^_^ Cadence Skill\302\333\314\263\316\302\334\260\314\341\320\321 - www.allegro-skill.com")
			axlUIWPrint(nil cadr(DateInfo))
			isDate = t
		);end when
	);end while
	when(isDate == nil
		while(DateMessage = gets(line inPort)
			savedTime = parseString(DateMessage)
			when(length(savedTime) == 2
				;savedTime = parseString(DateMessage)
				when(CurHour == atoi(car(savedTime))
					axlUIWPrint(nil "^_^ Cadence Skill\302\333\314\263\316\302\334\260\314\341\320\321 - www.allegro-skill.com")
					axlUIWPrint(nil cadr(savedTime))
					isDate = t
				);end when
			);end when
		);end while	
	);end when
	close(inPort)
	else
		axlUIWPrint(nil "Cannot find the file \"greeting\",Please check file directory!")
);end if
);end let

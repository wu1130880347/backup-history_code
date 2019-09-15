; ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;                      Net Length Report Generator
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;   DESCRIPTION
;
;   If you are looking for a program to find the total length of etch from a pin  
;   on the board file to another pin, then this program is for you.
;
;   The program finds the length of nets starting from a component(s) that you 
;   pick to all other components on the board.  In detail, when you run this 
;   program a form opens up.  On this form, you select the component(s) from where 
;   you would like to start measuring the distance.  You also select Power Nets 
;   (eg. VCC, GND, etc.) that are stitched to the plane.  Then the program goes 
;   through each pin on the component  If the net name attached to the pin is not 
;   a power pin then it measures the distance starting from that pin to other 
;   components.  It passes through DISCRETEs (resistors, capacitors, etc.), and 
;   selects the net on the other side of the DISCRETE.  If this net is one of the 
;   power nets, then it stops there and records that distance.  Otherwise, it keeps 
;   on going through the CLINES and recording their distances until it reaches a 
;   non-discrete component.  It records this total length and looks for more clines 
;   at this pin location.  If any cline is found, it goes through it and looks for 
;   more components through this path.  If it does find a component taking this path, 
;   it records its distance from the start pin (on the component you selected) to this 
;   location. When no more components are found on this path, then it goes back and 
;   explores all the other paths.  

;   The program also takes into account the length of arc CLINES.  It seems that the 
;   net length report in SpectraQuest assumes straight line segments.  Thus, the 
;   distances are always off by a few mils.

;   The program generates two reports.  In one of the reports, only the final distance 
;   to the pin on other componenets is recorded.  In the other report detailed 
;   information about how it got to that pin is recorded.
;    
;   Type "det" to run this program
;   
;
;   Written by Deepika Mehta
;   Celestica Memory Development
;   dmehta@celestica.com
;   June 15,2001
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


axlCmdRegister( "det" `det)


;##################################
;# _createForm                    #
;##################################
(defun _createForm ()

  Form = outfile("test.form" "w")

  fprintf(Form "FILE_TYPE=FORM_DEFN VERSION=2\n")
  fprintf(Form "FORM\n")
  fprintf(Form "FIXED\n")
  fprintf(Form "PORT 75 35\n")
  fprintf(Form "HEADER \"Netlist Generator\"\n")
  fprintf(Form "TILE\n")

  fprintf(Form "TEXT \"#################################################################\"\n")
  fprintf(Form "TLOC 2 2\n")
  fprintf(Form "ENDTEXT\n")

  fprintf(Form "TEXT \"         Please select the nets for which you would like to find lengths for.\"\n")
  fprintf(Form "TLOC 2 4\n")
  fprintf(Form "ENDTEXT\n")

  fprintf(Form "TEXT \"#################################################################\"\n")
  fprintf(Form "TLOC 2 6\n")
  fprintf(Form "ENDTEXT\n")

  fprintf(Form "TEXT \"Drivers\"\n")
  fprintf(Form "TLOC 2 10\n")
  fprintf(Form "ENDTEXT\n")

  fprintf(Form "TEXT \"Selected Drivers\"\n")
  fprintf(Form "TLOC 34 10\n")
  fprintf(Form "ENDTEXT\n")

  fprintf(Form "FIELD Drivers_List\n")
  fprintf(Form "FLOC 2 12\n")
  fprintf(Form "LIST \"\" 20 5\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD SelectedD_List\n")
  fprintf(Form "FLOC 34 12\n")
  fprintf(Form "LIST \"\" 20 5\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD right\n")
  fprintf(Form "FLOC 24 17\n")
  fprintf(Form "MENUBUTTON \"All ->\" 5 3\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD left\n")
  fprintf(Form "FLOC 24 14\n")
  fprintf(Form "MENUBUTTON \"<- All\" 5 3\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "TEXT \"Available Nets\"\n")
  fprintf(Form "TLOC 2 22\n")
  fprintf(Form "ENDTEXT\n")

  fprintf(Form "TEXT \"Power Nets\"\n")
  fprintf(Form "TLOC 34 22\n")
  fprintf(Form "ENDTEXT\n")

  fprintf(Form "FIELD Available_List\n")
  fprintf(Form "FLOC 2 24\n")
  fprintf(Form "LIST \"\" 20 5\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD PowerNets_List\n")
  fprintf(Form "FLOC 34 24\n")
  fprintf(Form "LIST \"\" 20 5\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD right2\n")
  fprintf(Form "FLOC 24 29\n")
  fprintf(Form "MENUBUTTON \"All ->\" 5 3\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD left2\n")
  fprintf(Form "FLOC 24 26\n")
  fprintf(Form "MENUBUTTON \"<- All\" 5 3\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD create\n")
  fprintf(Form "FLOC 2 36\n")
  fprintf(Form "MENUBUTTON \"Create Netlist\" 10 3\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "FIELD cancel\n")
  fprintf(Form "FLOC 20 36\n")
  fprintf(Form "MENUBUTTON \"CANCEL\" 10 3\n")
  fprintf(Form "ENDFIELD\n")

  fprintf(Form "ENDTILE\n")

  fprintf(Form "ENDFORM\n")
  close(Form)
); end defun
;##################################
;# End of _createForm             #
;##################################

;This function can be used to find any allegro object (eg. clines, nets,etc.)
;send the object type as argument.
(defun findAllOfType (findType "t")
  (axlClearSelSet) ;clear any previous selection

  (axlSetFindFilter ?enabled  (list "noall" findType "invisible")
    ?onButtons  (list "noall" findType)) ;"noall" is used to deselect any 
						     ;preselected buttons in allegro
  (axlAddSelectAll)
  (setq selSet (axlGetSelSet))
  selSet); end defun

;########################################################################
;#                                                                      #
;#                Function ashFindAllClines                             #
;#                                                                      #
;########################################################################
(defun ashFindAllClines ()	;find all clines
  (findAllOfType "CLINES")
); end ashFindAllClines

(defun ashFindAllComps ()	;find all components
  (findAllOfType "COMPONENTS")
); end ashFindAllComps

;Main function starts from here.
(defun det ()

  _createForm()  ;call the function to create test.form file
  Skill_Programs_Form=axlFormCreate( (gensym) "test.form" '(e outer) 'skill_programs_callback t)
  ;"skill_programs_callback" is the name of the function that controls the form

  axlFormDisplay(Skill_Programs_Form)
  

  ;Declare Variables
  netlist=list()
  powers=list()
  available=list()
  cl=list()
  net_cl=list()
  ch=list()
  drivers=list()
  seldri=list()
  drs=list()
  cpins=list()
  remvpins=list()

  netlist=axlDBGetDesign()->nets ;netlist=list containing dbids of all the nets
  ;instead of finding all nets, you could find components, drcs, padstacks, etc.
  ;see chapter 2, page 11 for a list of other design attributes that could be used here.

  if(netlist==nil then
    axlFormSetField(Skill_Programs_Form,"Available_List",list("No Netlist Loaded"))
  else
    foreach(onenet netlist
      available=append1(available,onenet->name)
	;Do not use "append" because that concatenates two lists.
	;whenever you use "append1", "sort", or any other functions that modify a 
	;list, then make sure you equate the list to the result (the manual does
	;not do this)
    ); endforeach
    available=sort(available 'alphalessp)	;sort the list in alphabetical order

    foreach(onename available
      axlFormSetField(Skill_Programs_Form,"Available_List",onename)
    ); endforeach
  );end if

  comps=axlDBGetDesign()->components
  if(comps==nil then
    axlFormSetField(Skill_Programs_Form,"Drivers_List",list("No Components in design"))
  else
    foreach(onec comps
      drivers=append1(drivers,(onec->name))
    ); endforeach
    drivers=sort(drivers 'alphalessp)
    foreach(onecomp drivers
      axlFormSetField(Skill_Programs_Form,"Drivers_List",onecomp)
    ); endforeach
  );end if


  ;The following function controls form and its fields
  (defun skill_programs_callback (Skill_Programs_Form)
    ;the following case statement checks which field has been selected
    ;and takes appropriate action
    (case Skill_Programs_Form->curField

      ("Available_List"
        ;if the cursor is in Available nets List Field then we need to 
	  ;move the selected item to the Power Nets List Field
	  powers=append1(powers,Skill_Programs_Form->curValue)
        axlFormSetField(Skill_Programs_Form,"PowerNets_List",Skill_Programs_Form->curValue)
	  axlFormSetField(Skill_Programs_Form,"PowerNets_List",nil)
		 
	  if( available!=nil then
	    axlFormListDeleteAll(Skill_Programs_Form,"Available_List")
	    available=remd((Skill_Programs_Form->curValue) available)
	  );end if

	  available=sort(available 'alphalessp)
	  if(  available!=nil then
	    foreach(one available
            axlFormSetField(Skill_Programs_Form,"Available_List",one)
	    ); endforeach
	  );end if
	  axlFormSetField(Skill_Programs_Form,"Available_List",nil)
	t)


      ("PowerNets_List"
	  available=append1(available,(Skill_Programs_Form->curValue))
 	  axlFormListDeleteAll(Skill_Programs_Form,"Available_List")
	  available=sort(available 'alphalessp)
	  foreach(onename available
          axlFormSetField(Skill_Programs_Form,"Available_List",onename)
        ); endforeach
	  axlFormSetField(Skill_Programs_Form,"Available_List",nil)

	  if( powers!=nil then
	    axlFormListDeleteAll(Skill_Programs_Form,"PowerNets_List")
	    powers=remd((Skill_Programs_Form->curValue) powers)
	  )
	  if( powers!=nil then
	    foreach(one powers
            axlFormSetField(Skill_Programs_Form,"PowerNets_List",one)
          ); endforeach
        )
	  axlFormSetField(Skill_Programs_Form,"PowerNets_List",nil)
	t)

      ("Drivers_List"
	  seldri=append1(seldri,Skill_Programs_Form->curValue)
 	  axlFormListDeleteAll(Skill_Programs_Form,"SelectedD_List")
	  foreach(onedri seldri
          axlFormSetField(Skill_Programs_Form,"SelectedD_List",onedri)
        ); endforeach
	  axlFormSetField(Skill_Programs_Form,"SelectedD_List",nil)
		 
	  if( drivers!=nil then
	    axlFormListDeleteAll(Skill_Programs_Form,"Drivers_List")
	    drivers=remd((Skill_Programs_Form->curValue) drivers)
	  )
	  drivers=sort(drivers 'alphalessp)
	  if(  drivers!=nil then
	    foreach(one drivers
          axlFormSetField(Skill_Programs_Form,"Drivers_List",one)
	    ); endforeach
	  )
	  axlFormSetField(Skill_Programs_Form,"Drivers_List",nil)
	t)


      ("SelectedD_List"
	  drivers=append1(drivers,(Skill_Programs_Form->curValue))
 	  axlFormListDeleteAll(Skill_Programs_Form,"Drivers_List")
	  drivers=sort(drivers 'alphalessp)
	  foreach(onename drivers
          axlFormSetField(Skill_Programs_Form,"Drivers_List",onename)
        ); endforeach
	  axlFormSetField(Skill_Programs_Form,"Drivers_List",nil)
	  if( seldri!=nil then
	    axlFormListDeleteAll(Skill_Programs_Form,"SelectedD_List")
	    seldri=remd((Skill_Programs_Form->curValue) seldri)
	  )
	  if( seldri!=nil then
	    foreach(one seldri
            axlFormSetField(Skill_Programs_Form,"SelectedD_List",one)
          ); endforeach
        )
	  axlFormSetField(Skill_Programs_Form,"SelectedD_List",nil)
	t)

      ("create"
	  ;for each(one in the selected
	  ;break it up into CLINES.

	  axlClearSelSet()
	  filename=outfile("detailed.rpt")
	  brief=outfile("brief.rpt")
	  line=nil
  	  fprintf(filename "\n\n")
  	  fprintf(filename "########################################################\n")
	  line="\t length    layer \n"
	  fprintf(filename line)

	  ;get the pins attatched to all the drivers selected
	  foreach(onedr seldri
	    ;select the driver
	    drs=setof(n ashFindAllComps() n->name == onedr)
	    cpins=append(cpins (car(drs)->pins))
	  )

	  shell("rm test.log")
	  foreach(onepin cpins

	   ;if this pin hasn't been visited already (remember that a net from 
	   ;one of the pins on the drivers can always go to the same driver, but
	   ;different pin.  In this case you don't want to calculate the length
	   ;in circular fashion.
	   if(exists(x remvpins x==onepin)==nil then
	    ;declare variables
	    line=""
	    ch=list()
  	    br=list()
	    cpins=list()
	    ch2=list()
  	    conns=list()
	    lens=list()
	    lays=list()
	    remvpins=list()
	    check=(onepin->xy)
	    onenet=onepin->net
	    pinthrough=nil

        ;if(equal(onenet->name "MEM_WE_L0") then
	    ;if the pin has a net name attached to it, and it is not one of the power
	    ;nets(eg. VDD, GND, etc.), then need to find distance from this pin to all
	    ;other pins it's connected to.
	    if(and((strlen(onenet->name)!=0) (exists(x powers x==onenet->name)==nil)) then
		comp=onepin->component
		num=onepin->number	;pin number
		ref=comp->name		;ref des
		line=strcat("\n" ref "." num ", net name: " onenet->name "\n")
		fprintf(filename line)
		fprintf(brief line)
		axlMsgPut("net=%s is not a power net" onenet->name)

		;get the objects (vias, paths, pins, etc.) attached to the net
		br=car(onenet->branches)	
		ch=br->children	;ch contains every allegro object(pin, clines, vias, tees)
					;connected to the net
		ch=remd(onepin ch);you don't want to go from the pin to itself. Therefore, it
					;can be deleted.

		;initialize variables
		alldone=t	;keeps track whether there are still anymore components left to
				;visit
 	      l=nil

		final_dist=0.0	;keeps track of the distance from the driver pin to the
					;component
		found=nil
	      prev_path=nil  ;keeps track whether the previous selection was a path or not
		prev_pin=nil  
		prev_tee=nil
		line=""
		firsttime=t	  ;first time through the loop
		numb=0
	  	while((alldone!=nil)
		  b=outfile("test.log" "a")
		  ;fprintf(b "alldone is not nil\n")
		  oncethrough=nil
		  alldone=nil
              while((oncethrough==nil)  ;after finding a path or pin, and moving location
						    ;to next point, you want to visit the list of
						    ;ch atleast once to find any path or pin at this
						    ;new location. If nothing is found, then check
						    ;will be moved back to previous location to find
						    ;any other paths starting from that location
						    ;(this is done to implement recursion in the
						    ;program
                oncethrough=t
		    found=nil
		    foreach(onech ch
		      fprintf(b "onech->objType is %s\n" onech->objType)

		      if(equal(onech->objType "via") then
			  fprintf(b "a via is found\n")
		        ch=remd(onech ch)
	            );endif "via"

	            if(equal(onech->objType "tee") then
			  fprintf(b "a tee is found\n")
			  loc=onech->xy
			  fprintf(b "tee loc=%L \n" loc)
			  if(equal(loc check) then
     			    prev_tee=t
			    found=t
			    conns=append1(conns onech)
		          ch=remd(onech ch)
			    oncethrough=nil
			  );end if
		      );endif "tee"

		      if(equal(onech->objType "shape") then
			  fprintf(b "a shape is found\n")
		        ch=remd(onech ch)
		      );end if "shape"

		      if(equal(onech->objType "path") then
			  fprintf(b "a path is found\n")
		        ;need to check if the start or end location 
			  ;of this path is equal to "check" (check=
			  ;location of last path/pin included in measurement)

			  ;if true, add the cline length (check for
			  ;radius and find the true length).
			  xcheck=car(check)
			  ycheck=cadr(check)
			  vfound=nil

			  ;look for via at the "check" location
			  axlClearSelSet()
  			  axlSetFindFilter(?enabled  (list "noall" "VIAS" "invisible") 
			  ?onButtons  (list "noall" "VIAS"))
			  axlOpenFindFilter()
                    axlSingleSelectPoint(xcheck:ycheck)
			  vfound=axlGetSelSet() ;via found
			  fprintf(b "onch->layer=%s\n" onech->layer)

			  ;if either it's first time going through the loop (which means it's starting from a 
			  ;pin), or a via was found at the location (ie. last location was a path as well),
			  ;or previous selection was a pin and it was either on same layer or a pinthrough 
			  ;component, then check if this path starts at the same location as checkpoint.
		        if((firsttime==t || vfound!=nil || (prev_tee=t && equal(l onech->layer)) || (prev_path==nil && (equal(l onech->layer) || pinthrough==t) )) then
			    ;if before it was a pin or a via is found at current locations
			    segs=onech->segments
			    addit=nil
			    lastseg=car(last(segs))
			    firstseg=car(segs)
			    start_firstseg=car(firstseg->startEnd)
			    end_firstseg=cadr(firstseg->startEnd)
			    start_lastseg=car(lastseg->startEnd)
			    end_lastseg=cadr(lastseg->startEnd)
			    fprintf(b "end_lastseg=%L start_firstseg=%L\n" end_lastseg start_firstseg)
			    fprintf(b "lens=%L\n" lens)
  
			    if(equal(end_lastseg check) then
			      fprintf(b "end_lastseg=%L is equal to check=%L\n" end_lastseg check)
			      addit=t
			      check=start_firstseg
			    else
			      if(equal(start_firstseg check) then
			        fprintf(b "start_firstseg=%L is equal to check=%L\n" start_firstseg check)
			        addit=t
			        check=end_lastseg
			      );endif
			    );endif

			    if(addit then
			      fprintf(b "addit is true\n")
			      dist=0
			      firsttime=nil
			      prev_pin=nil
				prev_tee=nil
			      oncethrough=nil
			      conns=append1(conns onech) ;the list of connections that we have 
								   ;included just now.
			      ch=remd(onech ch)
			      found=t	;keeps track whether a path has been found when
						;backtracking the step
			      foreach(oneseg segs
			        x1 = xCoord(car(oneseg->startEnd))
			        x2 = xCoord(cadr(oneseg->startEnd))
			        y1 = yCoord(car(oneseg->startEnd))
			        y2 = yCoord(cadr(oneseg->startEnd))
			        deltax=abs(x2-x1)
			        deltay=abs(y2-y1)
			        rad=oneseg->radius
			        len=sqrt((deltax*deltax)+(deltay*deltay))
				  ;if radius is not nil, then it's an arc and need to calculate
				  ;the length for the arc.
			        if(rad!=nil then
			          rad_sq=rad*rad
			          len_sq=len*len
			          cos_theta=(2*rad_sq-len_sq)/(2*rad_sq)
			          theta=acos(cos_theta)
			          len=theta*rad
			        );endif(rad!=nil	
			        dist=dist+len
			      );end foreach(oneseg segs
		
			      line=sprintf(line "%s %f\t" line dist)
			
			      final_dist=final_dist+dist
			      l=(onech->layer)

		            line=strcat(line l "  \n")

			      lens=append1(lens dist)
			      lens=append1(lens l)
			      lays=cons(l lays)
			      fprintf(b "dist=%s found for one of the paths\n" line)
			      fprintf(b "elements in lens are %L\n" lens)
			      prev_path=t
		          );endif(addit
		        );endif(vfound!=nil
		      );endif "path"  

		      if(equal((onech->objType) "pin") then
		        ;check if the centre of this pin is at "check"
			  ;if it is, then check if the pin is a discrete
			  ;	if it is, then get the net on the other end
			  ;	of the pin.if it's not a power pin then add
			  ;	its length and continue that until find
			  ;     a receiver
			  centrepin=(onech->xy)
			  
			  if((prev_pin==nil && equal(centrepin check)) then
			    prev_path=nil
			    prev_pin=t
			    prev_tee=nil
			    pinthrough=onech->isThrough
       	          ch=remd(onech ch)
			    comp=onech->component
			    compClass=comp->class
			    num=onech->number
			    ref=comp->name
			    ;if this pin is on one of the drivers then we
			    ;don't need to find the distance from this pin again
			    ;because it's already being accounted for.
			    if(exists(x seldri x==ref) then
				remvpins=append1(remvpins onech)
			    );end if
			    oncethrough=nil
			    line=strcat(line "\t\t\t\t" ref "." num)
			    ;fprintf(filename line)

			    if(equal(compClass "DISCRETE") then
			      fn=comp->functions
			      fprintf(b "it's a discrete\n")

				;find pin on the other side of discrete
			      con=nil
			      foreach(onefn fn
			        fnpins=onefn->pins
				  pin1=car(fnpins)->pin
				  pin2=cadr(fnpins)->pin
				  if(equal((pin1->number) (onech->number)) then
				    con=pin2
				  )
				  if(equal((pin2->number) (onech->number)) then
				    con=pin1
				  )
			      ) ;end foreach(onefn fn

			      if(con!=nil then
			        fprintf(b "con is not nil\n")
				  conns=append1(conns onech)
				  conns=append1(conns con)
				  
				  ;if pin on the other side is not power pin
			        if(exists(x powers x==con->net->name)==nil then
			          ;select the net on the other side
				    check=(con->xy)
				    found=t
				    br2=car(con->net->branches)	
		     		    ch2=br2->children ;include everything connected
							    ;to net on the other side to
							    ;the ch list
				    ch2=remd(con ch2)
				    ch=append(ch ch2)
				    ch=remd(con ch)
				    line=strcat(line "\n")				 
			        else ;if can't add the net on other side of discrete
				    fprintf(b "net on other side is a power pin\n")
				    found=t
				    oncethrough=nil
				    line2=sprintf(line2 "%s \tline dist to pin= %f\n\n" line final_dist)
				    fprintf(filename line2)
				    line2=strcat("\t" ref "." num "\t")
				    line2=sprintf(line2 "%s distance=%f\n" line2 final_dist)
				    fprintf(brief line2)
				    line=strcat(line "\n")
			
				    check=onech->xy
			        );end if (exists...
			      );endif(con!=nil
			    else ;if not a discrete
				;reached one of the endpoints

			      line=sprintf(line "%s \tline dist to pin= %f\n\n" line final_dist)
				;line=strcat(line line2)
				ch=remd(onech ch)
				fprintf(filename line)
				line=strcat("\t" ref "." num "\t")
				line=sprintf(line "%s distance=%f\n" line final_dist)
				fprintf(brief line)
				fprintf(b "line printed is %s\n" line)
				line=""
			      final_dist=0.0
			      found=t
			      oncethrough=nil	;need to go through the entire ch list and see
							;if we can find any path on this pin location.
							;Need to consider this to take care of 
							;daisy chained case.
				conns=append1(conns onech)
				fprintf(b "it's not a discrete. comp found=%s\n" ref)
				fprintf(b "elements in lens=%L\n" lens)
			      foreach(onelen lens
			        if(numberp(onelen) then
				    line=sprintf(line "%s %f\t" line onelen)
				    final_dist=final_dist+onelen
				  else
				    line= sprintf(line "%s %s\n" line onelen)
				  );end if
			      );end foreach
			    ) ;end if(eq(compclass DISCRETE...
			  );end if(eq(centrepin check) ..
		      );end if(eq(onech->objType "pin") 
		    );end foreach(onech ch
	        );end while(oncethrough!=t)

		  ;if there are no more pins left in ch, then make alldone=t
		  foreach(onech ch
		    comp=onech->component
		    if(equal(onech->objType "pin") then
		      alldone=t
		    );end if(equal
		  );end foreach(onech ch

		  ;found will be true if we find any path or pin at the check
		  ;point.  if found is nil, then we went through the entire
		  ;ch list and did not find any object at that location.  Need
		  ;to move check point to the previous location and see if
		  ;something can be found there.
		  if(found==nil then
		    lastcon=car(last(conns))
		    fprintf(b "found is nil")
		    conns=remd(lastcon conns)
                if(lastcon!=nil then
		      if(equal(lastcon->objType "path") then
			  l=car(lays)
			  fprintf(b "object found is a path\n")
			  lays=cdr(lays)
			  prev_pin=nil
			  prev_tee=nil
			  prev_path=t
			  segs=lastcon->segments
			  lastseg=car(last(segs))
	  		  firstseg=car(segs)
			  startloc=car(firstseg->startEnd)
			  endloc=cadr(lastseg->startEnd)

			  temp=list()
			  leng=length(lens)
			  for(i 1 leng-2 
			    temp=append1(temp car(lens))
			    lens=nthcdr(1 lens)
			  )
			  lens=temp
			  fprintf(b "lens=%L" lens)
		        if(equal(startloc check) then
		          check=endloc
		        else
		          check=startloc
		        );end if
		      else
			 if(equal(lastcon->objType "tee") then
				prev_pin=nil
				prev_path=nil
				prev_tee=t
			 else
		        fprintf(b "found is nil and a pin is found\n")
			  pin2=car(last(conns))
			  prev_pin=t
			  prev_path=nil
			  if(pin2->number !=nil then
			    conns=remd(pin2 conns)
			    if(equal(lastcon->xy check) then
			      check=pin2->xy
				pinthrough=pin2->isThrough
			    else
			      check=lastcon->xy
				pinthrough=lastcon->isThrough
			    );end if
			  else
			    pinthrough=lastcon->isThrough
			    check=lastcon->xy
			  );end if
			 );end if
		      );end if
		    else ;if we've traversed (back) through all the elements in 
			   ;"conns", then we should set the checkpoint back
			   ;to the driver pin from where it started.
		     fprintf(b "it went to else because no last conn left\n")
		     prev_pin=t
		     prev_path=nil
		     check=onepin->xy
		     pinthrough=onepin->isThrough
		    );end if

		    line=""
		    final_dist=0.0
		    foreach(onelen lens
		      if(numberp(onelen) then
			  line=sprintf(line "%s %f\t" line onelen)
			  final_dist=final_dist+onelen
			else
			  line= sprintf(line "%s %s\n" line onelen)
			);end if
		    );end foreach
		  );endif
		  close(b)
	      );end while( (alldone!=nil)
	    );end if(exists

         ;);end if -need to remove it later on...just selects a particular net.
	   );end if (recvpins
	  );end foreach(onepin cpins
	  close(filename)
	  close(brief)
	  axlFormClose(Skill_Programs_Form)
      t)
				
	("left"
	  axlFormListDeleteAll(Skill_Programs_Form,"Drivers_List")
	  axlFormListDeleteAll(Skill_Programs_Form,"SelectedD_List")
	  seldri=list()
	  drivers=list()
	  foreach(onecomp comps
          drivers=append1(drivers,onecomp->name)
        ); endforeach
	  ;sort(available 'alphalessp)
	  foreach(onename drivers
          axlFormSetField(Skill_Programs_Form,"Drivers_List",onename)
        ); endforeach
	  axlFormSetField(Skill_Programs_Form,"Drivers_List",nil)
	  axlFormSetField(Skill_Programs_Form,"SelectedD_List",nil)
	t)
	("right"
	  axlFormListDeleteAll(Skill_Programs_Form,"Drivers_List")
	  axlFormListDeleteAll(Skill_Programs_Form,"SelectedD_List")
	  drivers=list()
	  seldri=list()
	  foreach(onecomp comps
          seldri=append1(seldri,onecomp->name)
        ); endforeach
	  ;sort(selected 'alphalessp)
	  foreach(onename seldri
          axlFormSetField(Skill_Programs_Form,"SelectedD_List",onename)
        ); endforeach
	  axlFormSetField(Skill_Programs_Form,"Drivers_List",nil)
	  axlFormSetField(Skill_Programs_Form,"SelectedD_List",nil)
	t)
	("left2"
	  axlFormListDeleteAll(Skill_Programs_Form,"Available_List")
	  axlFormListDeleteAll(Skill_Programs_Form,"PowerNets_List")
	  available=list()
	  selected=list()
	  foreach(onenet netlist
          available=append1(available,onenet->name)
        ); endforeach
	  ;sort(available 'alphalessp)
	  foreach(onename available
          axlFormSetField(Skill_Programs_Form,"Available_List",onename)
        ); endforeach
	  axlFormSetField(Skill_Programs_Form,"Available_List",nil)
	  axlFormSetField(Skill_Programs_Form,"PowerNets_List",nil)
	t)
	("right2"
	  axlFormListDeleteAll(Skill_Programs_Form,"Available_List")
	  axlFormListDeleteAll(Skill_Programs_Form,"PowerNets_List")
	  available=list()
	  powers=list()
	  foreach(onenet netlist
          powers=append1(powers,onenet->name)
        ); endforeach
	  ;sort(selected 'alphalessp)
	  foreach(onename powers
          axlFormSetField(Skill_Programs_Form,"PowerNets_List",onename)
        ); endforeach
	  axlFormSetField(Skill_Programs_Form,"Available_List",nil)
	  axlFormSetField(Skill_Programs_Form,"PowerNets_List",nil)
	t)

      ("cancel"
        axlFormClose(Skill_Programs_Form)
        axlCancelEnterFun()
      t)
    ); end case
  ); end of skill_programs_callback
  ;shell("rm Form.form")
); end of Skill_Programs_Menu

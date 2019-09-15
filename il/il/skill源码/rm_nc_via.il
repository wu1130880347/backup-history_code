/*******************************************************************
* DISCLAIMER: The following code is provided for Cadence customers *
* to use at their own risk. The code may require modification to   *
* satisfy the requirements of any user. The code and any           *
* modifications to the code may not be compatible with current or  *
* future versions of Cadence products.                             *
* THE CODE IS PROVIDED "AS IS" AND WITH NO WARRANTIES, INCLUDING   *
* WITHOUT LIMITATION ANY EXPRESS WARRANTIES OR IMPLIED WARRANTIES  *
* OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.              *
*******************************************************************/
; File: rm_nc_via.il
;
;
; Usage: Load this file rm_nc_via.il
;  Issue the command rmv on the Allegro command line.
;
; This utility will select all vias in the design and step through
; the list deleting all vias that are not on a net.
;
;
;Author: Ron Guthrie
;	Cadence Design Systems
; Version 1.0 
; Date: June 20,2005
;
procedure( _rm_nc_vias()
	let(list(l_vias, via via_count rm_via_count l_nc_vias)
	  l_nc_vias=list()
	  via_count=0
	  rm_via_count=0
	  axlClearSelSet()
	  axlSetFindFilter(?enabled '(noall vias invisible) ?onButtons '(vias))
	  axlAddSelectAll()	;select all of the vias in the design
	  l_vias=axlGetSelSet()	;put the vias in a list
	  via_count=axlGetSelSetCount()
	  printf("%d vias found in the design.\n" via_count)
	  axlClearSelSet()
	  ;step through the via list looking for unconnected vias.
	  foreach(via l_vias
	    if(via->net->name==nil then
	      ;found an unconnected via
	      axlDeleteObject(via)	;delete the via
	      rm_via_count++		;increment the counter
	    );endif
	  );end foreach
	  printf("%d unconnected vias removed.\n" rm_via_count)
	);end let
);end proc
axlCmdRegister("rmv" '_rm_nc_vias)
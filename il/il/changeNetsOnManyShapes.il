;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ;
 ; Author --
 ;	Cadence Design Systems
 ;
 ; Function --
 ;	
 ; Copyright, 2003, Cadence Design Systems, Inc.
 ; No part of this file may be reproduced, stored in a retrieval system,
 ; or transmitted in any form or by any means --- electronic, mechanical,
 ; photocopying, recording, or otherwise --- without prior written permission
 ; of Cadence Design Systems, Inc.
 ;
 ; WARRANTY:
 ; NONE. NONE. NONE.
 ; Use all material in this code at your own risk.  Cadence Design Systems 
 ; makes no claims about any material in this archive.  These examples may
 ; not function or may only function in specific instances. We'd like to hear 
 ; what you think of our approach to this, and how we can improve it.
 ;
 ; RESTRICTIONS:
 ; All software contained within this archive is in the public domain or
 ; the author has given us permission for redistribution.  Some packages
 ; have explicit copyrights and notices concerning their redistribution.
 ; Please carefully read all documentation with any package on this tape.
 ;
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; This allows a user to assign/change nets assign to one or more
; shapes. Command started user is prompted to pick one or more
; shapes (e.g. pick or by window). User is then presented with a net
; browser to select net.

; Examples of axl function use (for more info on individual functions
;	see the documentation)
;
; This demostrates writing a simple interactive Allegro command that
;
;   axlSetFindFilter: Prompt user to find elements
;   axlControlRaise: raise a panel in options
;   axlUIPopupSet: define your own popup
;
;   axlDBTransaction<XXX>:  database transactions (conditional database changes)
;   axlDBCloak: Improve performance when making multiple etch/logic changes
;
;   axlChangeNet: change the net associated with a database object
;   
;

_ashChangeNetFinish = nil

(defun ashChangeShapeNets ()
    (let (oldPopup procFunc)
	oldPopup = axlUIPopupSet(axlUIPopupDefine(nil 
				'(("Cancel" _ashChangeNetCancel)
				  ("Done" _ashChangeNetDone))))

	procFunc = '_ashChangeNetsListCloak
	axlControlRaise('find)
	axlSetFindFilter(?enabled '("NOALL" "SHAPES" ) 
			 ?onButtons '("SHAPES"))
        mark = axlDBTransactionStart()
	while( axlSelect(?prompt "Select shapes(s) to change nets")
	     apply(procFunc list(axlGetSelSet()))
	)

	if( _ashChangeNetFinish then
            axlDBTransactionCommit(mark)
	else
	    axlUIWPrint(nil 'warn "Canceled.\n")
            axlDBTransactionRollback(mark)
	)
	axlUIPopupSet(oldPopup)
	nil
))

(defun _ashChangeNetsListCloak (shapes)
    (let (net )

    axlDBCloak( '_ashChangeNetsList(shapes) ))
)

(defun _ashChangeNetsList (shapes)
    (let (net )

    net = axlUIDataBrowse('NET '(RETRIEVE_OBJECT) "Select Net" t)
    when(net
	axlUIWPrint(nil 'info0 "Changing shapes to net %s...\n" car(net))
	net = cadr(net)
	foreach(item shapes
	    ; only change shapes already with a net
	    when(item->net
                axlChangeNet(item net))
	)
	axlUIWPrint(nil 'info0 "Completed.\n")
    )
))

(defun _ashChangeNetCancel ()
	axlCancelEnterFun()
	_ashChangeNetFinish = nil
)

(defun _ashChangeNetDone ()
	axlFinishEnterFun()
	_ashChangeNetFinish = t
)


axlCmdRegister("changeshapenet" 'ashChangeShapeNets 
			?cmdType "interactive"
			?doneCmd '_ashChangeNetDone
			?cancelCmd '_ashChangeNetCancel)

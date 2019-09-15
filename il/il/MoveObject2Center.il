/*
################################################################################
#                                                                              #
#                      Command: MoveObject2Center                                    #
#                   Skill File: MoveObject2Center.il                                 #
#                  How To Load: Command> load("MoveObject2Center.il")              #
#               How To Execute: Command> MoveObject2Center                   #
#                Creation Date: 05/27/2015                                     #
#                                                                              #
################################################################################
*/

axlCmdRegister("MoveObject2Center" 'MoveObject2Center ?cmdType "interactive"
				?doneCmd 'MO2C_Done ?cancelCmd 'MO2C_Cancel)

procedure(MoveObject2Center()
let(()
	axlDBIgnoreFixed(t)
	axlSetFindFilter(?enabled list("all") ?onButtons list("noall" "pins" "shapes"))
	MO2Cpopup = axlUIPopupDefine( nil (list
		(list "Done" 'MO2C_Done)
		(list "Undo" 'MO2C_unDo)
		(list "Cancel" 'MO2C_Cancel)
    ))
	axlUIPopupSet(MO2Cpopup)
	
	MO2C_mark = axlDBTransactionStart()
	notdone = t
	while(notdone
		if(o_Object = axlGetSelSet(axlSelect()) then
			axlClearSelSet()
			firstbBox = car(o_Object)->bBox
			minX = car(car(firstbBox))
			minY = cadr(car(firstbBox))
			maxX = car(cadr(firstbBox))
			maxY = cadr(cadr(firstbBox))
			
			foreach(Object_dbid o_Object
				ObjectbBox = Object_dbid->bBox
				minX= min(minX car(car(ObjectbBox)))
				minY= min(minY cadr(car(ObjectbBox)))
				maxX = max(maxX car(cadr(ObjectbBox)))
				maxY = max(maxY cadr(cadr(ObjectbBox)))
			)
			
			CenterX = (maxX + minX)/2
			CenterY = (maxY + minY)/2

			moveX = 0- CenterX
			moveY = 0- CenterY
			
			axlTransformObject(o_Object ?move list(moveX, moveY))

		); endif
	)
)); let and procedure

procedure(MO2C_unDo()
let( ()
  if(! axlDBTransactionOops(MO2C_mark) then
    axlUIWPrint(nil "-- Nothing Left To Undo. --")
  else
    axlUIWPrint(nil "-- Replacing Clines. --")
  ); endif
  axlClearSelSet()
)); end let procedure

procedure(MO2C_Cancel()
let( ()
  axlUIWPrint(nil "** Cancelled Program. **")
  axlDBTransactionRollback(MO2C_mark)
  notdone = nil
  axlClearSelSet()
  axlCancelEnterFun()

)); end let procedure


procedure(MO2C_Done()
 let( ()
  axlUIWPrint(nil "- Done -")
  axlDBTransactionCommit(MO2C_mark)
  notdone = nil
  axlCancelEnterFun()
   
)); end let and procedure

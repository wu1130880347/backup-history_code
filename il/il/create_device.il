/********************************************************/
/*               Title :  Add Symbol Value				*/
/*     Execute Command :  add_symbol_value		       	*/
/*              Author :  VivienLuo	                	*/
/*       Creation Date :  2013-07-16                	*/
/*       		Update :                            	*/
/********************************************************/

axlCmdRegister("create_device" 'add_value_device ?cmdType "general")
axlCmdRegister("create_device_batch" 'create_device_batch ?cmdType "general")

defun( create_device_batch ()
	libFiles = getDirFiles("./")
	draFiles = nil
	foreach(libFile libFiles
		when(car(last(parseString(libFile ".")))== "dra"
			draFiles = cons(libFile draFiles)
		)
	)
	foreach(draFile draFiles
		axlOpenDesign(?design draFile ?mode "wf")
		add_value_device()
	);end foreach	
	axlShell("unset noconfirm")
);end defun

defun( add_value_device ()
	axlShell("set noconfirm")
	designType = axlDesignType(t)
	case(designType
		("PACKAGE"
			crete_device_run(strcat(axlCurrentDesign() ".psm"))
			axlPadstackToDisk()
		)
		("MECHANICAL" 
			crete_device_run(strcat(axlCurrentDesign() ".bsm"))
		)
		("FORMAT"
			crete_device_run(strcat(axlCurrentDesign() ".osm"))
		)
		("SHAPE"
			crete_device_run(strcat(axlCurrentDesign() ".ssm"))
		)
		("FLASH"
			crete_device_run(strcat(axlCurrentDesign() ".fsm"))
		)
	)		
	axlShell("replay \"./create_device.scr\"")		
	axlSaveDesign(?design axlCurrentDesign())	
	axlShell("unset noconfirm")
);end defun

defun( crete_device_run (fileName)
	saveScript = outfile("./create_device.scr")
	fprintf(saveScript "scriptmode +i +n\n")
	fprintf(saveScript "version 15.51\n")
	fprintf(saveScript "setwindow pcb\n")
	fprintf(saveScript "trapsize 666\n")
	fprintf(saveScript "create device\n")
	fprintf(saveScript "setwindow form.crdevice\n")
	fprintf(saveScript "FORM crdevice done\n")
	fprintf(saveScript "create symbol\n")
	fprintf(saveScript "fillin %s\n" fileName)
	close(saveScript)
);end defun



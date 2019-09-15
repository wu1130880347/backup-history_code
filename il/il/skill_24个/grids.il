;;###################################################################################################################################
;;调整skill窗口长度：第50行指令 fprintf(gridsform "PORT 108 3\n") 将 108 调整至合适
;;调整skill窗口标题：滴51行指令 fprintf(gridsform "HEADER \"Grids by Benson 2010.05.14\"\n") 将 Grids by Benson 2010.05.14 改成任何非汉字都可以
;;例如：
;;     要设置grids的值，
;;     调整下面指令对应位置的数值
;;     fprintf( gridsform "FIELD %n \nFLOC  1  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/100     conv_factor/100   )
;;                              第一个数字"1"在窗口中的位置                                         grids=100/***       grids=100/***
;;
;;     (equal(evalstring(gridsform->curField) conv_factor/100   )	UC_grids_script_create(conv_factor/100)  	);
;;                                              grids=100/***                              grids=100/***
;;     以上2条指令的位置fprintf顺数=equal倒数，可参考下面的例程
;;
;;这样一个grids就设置完了。
;;
;;如果设置的grids大于100，设置如下grids=250mil：
;;      fprintf( gridsform "FIELD %n \nFLOC 101 1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor*25/10   conv_factor*25/10 )
;;      (equal(evalstring(gridsform->curField) conv_factor*25/10 ) 	UC_grids_script_create(conv_factor*25/10) );
;;
;;此grids.il设置的grids有(mil):		1		2		4		5		10		20		25		50		100		200		250
;;
;;      skill调用时输入：grids            载入为：   load（“girds.il”）       skill menu为：MENUITEM "grids", "grids"
;;可修改适合自己的girds.il
;;
;;改著玩玩       Benson        Layout       2010.05.14
;;###################################################################################################################################


;Modify the hotkey by yourself
;---------------------------------------
axlShell("alias SInsert skill grids")
;---------------------------------------

defun( grids ()

; creates conversion factors for different db units
; -------------------------------------------------
;
units = car(axlDBGetDesignUnits())
	cond(
		(equal(units "microns") 	  (conv_factor = 1) 	 );
		(equal(units "mils") 		    (conv_factor = 100)  );
		(equal(units "inches") 	   	(conv_factor = 0.1)  );
		(equal(units "millimeters") (conv_factor = 2.54) );
		(equal(units "centimeters") (conv_factor = 0.254)); 
	) ; end of cond

	drain()
	gridsform_file = "grids.form"

	gridsform = outfile(gridsform_file "w")
     fprintf(gridsform "FILE_TYPE=FORM_DEFN VERSION=2\n")
     fprintf(gridsform "FORM\n")
     fprintf(gridsform "FIXED\n")
     fprintf(gridsform "PORT 108 3\n")
     fprintf(gridsform "HEADER \"Grids by Benson 2010.05.14\"\n")
     fprintf(gridsform "\n")
     fprintf(gridsform "TILE\n")
	;-------------------------------------------------------------------------------------------------------
     fprintf( gridsform "FIELD %n \nFLOC  1  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/100     conv_factor/100   )
     fprintf( gridsform "FIELD %n \nFLOC 11  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/50      conv_factor/50    )
     fprintf( gridsform "FIELD %n \nFLOC 21  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/25      conv_factor/25    )
     fprintf( gridsform "FIELD %n \nFLOC 31  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/20      conv_factor/20    )
     fprintf( gridsform "FIELD %n \nFLOC 41  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/10      conv_factor/10    )
     fprintf( gridsform "FIELD %n \nFLOC 51  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/5       conv_factor/5     )
     fprintf( gridsform "FIELD %n \nFLOC 61  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/4       conv_factor/4     )
     fprintf( gridsform "FIELD %n \nFLOC 71  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/2       conv_factor/2     )
     fprintf( gridsform "FIELD %n \nFLOC 81  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor/1       conv_factor/1     )
     fprintf( gridsform "FIELD %n \nFLOC 91  1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor*2       conv_factor*2     )
     fprintf( gridsform "FIELD %n \nFLOC 101 1 \nCHECKLIST \"%n\" \"radioLabel\"\nENDFIELD\n\n" conv_factor*25/10   conv_factor*25/10 )
	;-------------------------------------------------------------------------------------------------------
     fprintf(gridsform "\n")
     fprintf(gridsform "ENDTILE\n")
     fprintf(gridsform "\n")
     fprintf(gridsform "ENDFORM\n")
	close( gridsform)

	gridsform = axlFormCreate( (gensym) gridsform_file nil 'grids_Action t nil)
	axlFormDisplay(gridsform)
	axlUIWPrint(gridsform getCurrentTime()) 
	deleteFile("grids.form")
); end of grids
;--------------------------------------------------------
; what to do on form actions
; --------------------------
defun( grids_Action (gridsform)
cond(
	(equal(evalstring(gridsform->curField) conv_factor*25/10 ) 	UC_grids_script_create(conv_factor*25/10) );
	(equal(evalstring(gridsform->curField) conv_factor*2     ) 	UC_grids_script_create(conv_factor*2)     );
	(equal(evalstring(gridsform->curField) conv_factor       ) 	UC_grids_script_create(conv_factor) 	    );
	(equal(evalstring(gridsform->curField) conv_factor/2     ) 	UC_grids_script_create(conv_factor/2)   	);
	(equal(evalstring(gridsform->curField) conv_factor/4     ) 	UC_grids_script_create(conv_factor/4)   	);
	(equal(evalstring(gridsform->curField) conv_factor/5     ) 	UC_grids_script_create(conv_factor/5)   	);
	(equal(evalstring(gridsform->curField) conv_factor/10    )	UC_grids_script_create(conv_factor/10)  	);
	(equal(evalstring(gridsform->curField) conv_factor/20    ) 	UC_grids_script_create(conv_factor/20)    );
	(equal(evalstring(gridsform->curField) conv_factor/25    ) 	UC_grids_script_create(conv_factor/25)   	);
	(equal(evalstring(gridsform->curField) conv_factor/50    ) 	UC_grids_script_create(conv_factor/50)   	);
	(equal(evalstring(gridsform->curField) conv_factor/100   )	UC_grids_script_create(conv_factor/100)  	);
) ; end of cond
	deleteFile("grids.scr")
); end grids_Action
;--------------------------------------------------------
defun( UC_grids_script_create ( cur_grid )
	Script_File=outfile("grids.scr" "w")
    fprintf(Script_File "scriptmode +i\n")
    fprintf(Script_File "define grid \nsetwindow form.grid\n")
		fprintf(Script_File "FORM grid non_etch non_etch_x_grids %n \n" cur_grid)
		fprintf(Script_File "FORM grid non_etch non_etch_y_grids %n \n" cur_grid)
		fprintf(Script_File "FORM grid all_etch all_etch_x_grids %n \n" cur_grid)
		fprintf(Script_File "FORM grid all_etch all_etch_y_grids %n \n" cur_grid)
		fprintf(Script_File "FORM grid done\n")
	close(Script_File)	
	axlShell("replay grids.scr")
	axlUIWPrint(gridsform units) 
); end UC_grids_script_create
;--------------------------------------------------------
; Registers grids as an 'Allegro' command
; --------------------------------------
axlCmdRegister( "grids" `grids)

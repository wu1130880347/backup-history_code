;by chen_ck
;update 2014.7.10
;text reset
;	load("text_reset.il")

axlCmdRegister("TxtRST" 'text_reset ?cmdType "general")


defun( text_reset ()
	formPath = "./txt_block.form"
	Txt_blk = outfile( formPath "w")
	fprintf(Txt_blk "FILE_TYPE=FORM_DEFN VERSION=2\n")
	fprintf(Txt_blk "FORM\n")
	fprintf(Txt_blk "FIXED\n")
	fprintf(Txt_blk "PORT 39 10\n")
	fprintf(Txt_blk "HEADER \"Change Block\"\n")
	fprintf(Txt_blk "TILE\n")
/*************************************更改pinnumber到器件中心*******************************************/	
	fprintf(Txt_blk "TEXT \"Pin_Num\265\275Pin\326\320\320\304\":\n");位号到器件中心
	fprintf(Txt_blk "FLOC 2 2\n")
	fprintf(Txt_blk "ENDTEXT\n")
	
	fprintf(Txt_blk "FIELD pinnum_rst\n")
	fprintf(Txt_blk "FLOC 27 2\n")
	fprintf(Txt_blk "MENUBUTTON \"\326\264\320\320\" 10 3\n");执行
	fprintf(Txt_blk "ENDFIELD\n")

/*************************************更改位号到器件中心*******************************************/
	fprintf(Txt_blk "GROUP \n")
	fprintf(Txt_blk "FLOC 1 4\n")
	fprintf(Txt_blk "FSIZE 38 11\n")
	fprintf(Txt_blk "ENDGROUP\n")
	
	fprintf(Txt_blk "TEXT \"\316\273\272\305\270\264\316\273\265\275\306\367\274\376\326\320\320\304\":\n");位号到器件中心
	fprintf(Txt_blk "FLOC 2 6\n")
	fprintf(Txt_blk "ENDTEXT\n")

	fprintf(Txt_blk  "FIELD area_rest\n")
	fprintf(Txt_blk  "FLOC 3 9\n")
	fprintf(Txt_blk  "CHECKLIST \"\307\370\323\362\270\264\316\273\"\n")
	fprintf(Txt_blk  "ENDFIELD\n")
	
	fprintf(Txt_blk "FIELD area_ref\n")
	fprintf(Txt_blk "FLOC 15 9\n")
	fprintf(Txt_blk "MENUBUTTON \"\307\370\323\362\321\241\324\361\" 10 3\n");区域选择
	fprintf(Txt_blk "ENDFIELD\n")
	
	fprintf(Txt_blk "FIELD refTocenter\n")
	fprintf(Txt_blk "FLOC 27 9\n")
	fprintf(Txt_blk "MENUBUTTON \"\326\264\320\320\270\264\316\273\" 10 3\n");执行复位
	fprintf(Txt_blk "ENDFIELD\n")

	fprintf(Txt_blk  "FIELD icld_silk\n")
	fprintf(Txt_blk  "FLOC 3 12\n")
	fprintf(Txt_blk  "CHECKLIST \"REF-SILK\"\n")
	fprintf(Txt_blk  "ENDFIELD\n")
	
	fprintf(Txt_blk  "FIELD icld_assem\n")
	fprintf(Txt_blk  "FLOC 23 12\n")
	fprintf(Txt_blk  "CHECKLIST \"REF-ASMB\"\n")
	fprintf(Txt_blk  "ENDFIELD\n")
	
	fprintf(Txt_blk "ENDTILE\n")
	fprintf(Txt_blk "ENDFORM\n")
	close(Txt_blk)
	
	tb_form = axlFormCreate( (gensym) "txt_block.form" `(ne inner "msglines" 2) `txtblock_Action t nil)
	axlFormDisplay( tb_form )
	deleteFile(formPath)
	axlFormSetFieldVisible(tb_form   "area_ref"    0)
	area_list = nil
);
/*************************************callback*************************************/
defun( txtblock_Action (tb_form)
	case(tb_form->curField
		("pinnum_rst"	reset_pinnumber_to_center()
		);case1
		("refTocenter" 	   reset_ref_to_symcenter()
		);case12 位号复位到器件中心
		("area_rest"     area_select_en(tb_form)
		);case14 位号复位按钮使能
		("area_ref"		area_select()
		);case15 位号复位 区域选择
	);case
);;defun
/*********************区域选择使能函数*******************/
defun( area_select_en (tb_form)
	if(axlFormGetField( tb_form "area_rest") == t
		then
			axlFormSetFieldVisible(tb_form   "area_ref"   1)
		else
			axlFormSetFieldVisible(tb_form   "area_ref"   0)	
	);if
);defun
/*********************区域选择函数*******************/
defun( area_select ()

	axlShell("done")
	area_list = nil
	axlClearSelSet()
	axlSetFindFilter(?enabled list("noall" "symbols") ?onButtons list("noall" "symbols"))
	alltexts3 = axlGetSelSet(axlAddSelectBox())
	axlDBRefreshId(nil)
	foreach(db alltexts3
		foreach(db3 db->children 
			if(car(parseString(db3->layer "/")) == "REF DES"
				then
					area_list = cons(db3 area_list)
			);if
		);foreach 选择不相关字符
	);foreach 选择不相关字符
);
/*********************器件位号复位到器件中心*******************/
defun( reset_ref_to_symcenter ()
	axlShell("done")
	if(axlFormGetField( tb_form "icld_silk")
		then
			axlVisibleLayer( "REF DES/SILKSCREEN_TOP" t )
			axlVisibleLayer( "REF DES/SILKSCREEN_BOTTOM" t )
			axlVisibleLayer( "REF DES/ASSEMBLY_TOP" nil )
			axlVisibleLayer( "REF DES/ASSEMBLY_BOTTOM" nil )
		else if(axlFormGetField( tb_form "icld_assem")
			then
				axlVisibleLayer( "REF DES/ASSEMBLY_TOP" t )
				axlVisibleLayer( "REF DES/ASSEMBLY_BOTTOM" t )
				axlVisibleLayer( "REF DES/SILKSCREEN_TOP" nil )
				axlVisibleLayer( "REF DES/SILKSCREEN_BOTTOM" nil )
			);end if
			
	);end if
	axlVisibleUpdate(t)
	
	ref_list = nil
	sym_list = nil
	sym_tmp = nil
	sym_center = nil
	
	if(listp(area_list)
		then
			ref_list = area_list
		else
			axlClearSelSet()
			axlSetFindFilter(?enabled list("noall" "text") ?onButtons list("noall" "text"))
			alltexts2 = axlGetSelSet(axlAddSelectAll())
			axlDBRefreshId(nil)
			foreach(db alltexts2
				if(car(parseString(db->layer "/")) == "REF DES"
					then
						ref_list = cons(db ref_list)
				);if
			);foreach 选择不相关字符
	);if boundp(area_list)
	foreach(db ref_list
		rotmark = 0

		ref_center = list( fix((xCoord(car(db->bBox)) + xCoord(cadr(db->bBox)))/2)
										fix((yCoord(car(db->bBox)) + yCoord(cadr(db->bBox)))/2)
						)
		foreach(db2 db->parent->children 
			if(cadr(parseString(db2->layer "_")) == "BOUND"
				then
					sym_tmp = db->parent
					sym_center = list(  fix((xCoord(car(db2->bBox)) + xCoord(cadr(db2->bBox)))/2)
										fix((yCoord(car(db2->bBox)) + yCoord(cadr(db2->bBox)))/2)
					);器件中心
			);if
		);foreach
					
		case(sym_tmp->rotation
			(0.0	cond(
						(db->rotation == 0.0 rotmark = 0.0)
						(db->rotation == 90.0 rotmark = 270.0)
						(db->rotation == 180.0 rotmark = 180.0)
						(db->rotation == 270.0 rotmark = 90.0)
					);cond
			);case
			(90.0	cond(
						(db->rotation == 0.0 rotmark = 90.0)
						(db->rotation == 90.0 rotmark = 0.0)
						(db->rotation == 180.0 rotmark = 270.0)
						(db->rotation == 270.0 rotmark = 180.0)
					);cond
			);case
			(180.0	cond(
						(db->rotation == 0.0 rotmark = 0.0)
						(db->rotation == 90.0 rotmark = 270.0)
						(db->rotation == 180.0 rotmark = 180.0)
						(db->rotation == 270.0 rotmark = 90.0)
					);cond
			);case
			(270.0	cond(
						(db->rotation == 0.0 rotmark = 90.0)
						(db->rotation == 90.0 rotmark = 0.0)
						(db->rotation == 180.0 rotmark = 270.0)
						(db->rotation == 270.0 rotmark = 180.0)
					);cond
			);case
		);case
	axlTransformObject(db, ?move mapcar('difference sym_center ref_center), ?angle rotmark);执行更改
	);foreach
	area_list = 0;框选区域变量复位
);defun
defun( reset_pinnumber_to_center ()
	axlShell("done")
 	axlVisibleLayer( "PACKAGE GEOMETRY/PIN_NUMBER" t )
	axlVisibleUpdate(t)
	
	pinnumber_list = nil
	pin_center = nil
	pinnumber_center = nil
	
	axlClearSelSet()
	axlSetFindFilter(?enabled list("noall" "text") ?onButtons list("noall" "text"))
	alltexts3 = axlGetSelSet(axlAddSelectAll())
	axlDBRefreshId(nil)
	foreach(db alltexts3
		if((db->layer == "PACKAGE GEOMETRY/PIN_NUMBER")
			then
				pinnumber_list = cons(db pinnumber_list)
		);if
	);foreach 选择不相关字符

	foreach(db pinnumber_list
		pin_center =  db->parent->xy
		pinnumber_center = db->xy
	axlTransformObject(db, ?move mapcar('difference pin_center pinnumber_center), ?angle 0.0);执行更改
	);foreach
);defun
/****************************************************/
/*               Title :  find symbol no refdes		*/
/*     Execute Command :  find_symbol_no_refdes		*/
/*              Author :  Vivien Luo                */
/*       Creation Date :  2013-09-16                */
/*       		Update :  		                	*/
/*            Web site :  www.allegro-skill.com     */
/*             E-maial :  admin@allegro-skill.com	*/
/****************************************************/
axlCmdRegister("find_symbol_no_refdes" 'find_symbol_no_refdes)

defun(find_symbol_no_refdes ()
	let((curVisible symbols logfile sym txt norefdes)
	logifile = outfile("./find_symbol_no_refdes.log")
	fprintf(logifile "------------------------------------------\n")
	fprintf(logifile "Find symbol no refdes report				\n")
	fprintf(logifile "   Author : Vivien Luo					\n")
	fprintf(logifile "   E-mail : admin@allegro-skill.com		\n")
	fprintf(logifile "Date&Time : %s							\n" getCurrentTime())
	fprintf(logifile "------------------------------------------\n")
	fprintf(logifile "Refdes\t\t\tLocation\n")
	;visible the layers
	axlVisibleLayer("PIN/TOP" t)
	axlVisibleLayer("PIN/BOTTOM" t)
	axlVisibleLayer("REF DES/SILKSCREEN_TOP" t)
	axlVisibleLayer("REF DES/SILKSCREEN_BOTTOM" t)
	axlVisibleLayer("PACKAGE GEOMETRY/SILKSCREEN_TOP" t)
	axlVisibleLayer("PACKAGE GEOMETRY/SILKSCREEN_BOTTOM" t)
	axlVisibleUpdate(t)
	symbols = axlDBGetDesign()->symbols
	foreach(sym symbols
		norefdes = t
		when(sym->refdes
			foreach(txt axlDBGetAttachedText(sym)
				when(txt->layer == "REF DES/SILKSCREEN_TOP" || txt->layer == "REF DES/SILKSCREEN_BOTTOM"
					norefdes = nil
				)
			)
			when(norefdes
				fprintf(logifile "%s\t\t\t(%P)\n" sym->refdes sym->xy)
				axlHighlightObject(sym)
			)
		)
	)
	close(logifile)
	axlUIViewFileCreate("./find_symbol_no_refdes.log" "Find Symbol No Refdes  - www.allegro-skill.com" nil)
)
)
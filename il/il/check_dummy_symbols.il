axlCmdRegister("ck_dm_sym" 'check_dummy_symbols)
defun(check_dummy_symbols ()
let((syms sym)
	syms = axlDBGetDesign()->symbols
	foreach(sym syms
		unless(sym->refdes
			axlHighlightObject(sym)
		)
	);end foreach
);end let
);end defun
/****************************************************/
/*             Title : quick switch layer           */
/*   Execute Command : s_layer                      */
/*            Author : VivienLuo                    */
/*     Creation Date : 2014-09-26                   */
/*            Update :                              */
/*           Version : 1.0                          */
/*           Website : www.allegro-skill.com        */
/*           E-maial : admin@allegro-skill.com      */
/****************************************************/

axlCmdRegister("c_layer" 'c_layer ?cmdType "interactive")
axlCmdRegister("s_layer" 's_layer ?cmdType "interactive")

defun(s_layer (@optional (layer ""))
let((allLayer countLayer)
	allLayer = axlGetParam("paramLayerGroup:ETCH")->groupMembers
	countLayer = length(allLayer)
	if(layer != "" && atoi(layer)
		then
			layer = atoi(layer)
			if( layer <= countLayer && layer > 0
				then
					foreach(l allLayer
						axlVisibleLayer(strcat("ETCH/" l) nil)
						axlVisibleLayer(strcat("PIN/" l) nil)
						axlVisibleLayer(strcat("VIA CLASS/" l) nil)    
						axlVisibleLayer(strcat("DRC ERROR CLASS/" l) nil)
					)
					axlVisibleLayer(strcat("ETCH/" nthelem(layer allLayer)) t)
					axlVisibleLayer(strcat("PIN/" nthelem(layer allLayer)) t)
					axlVisibleLayer(strcat("VIA CLASS/" nthelem(layer allLayer)) t)    
					axlVisibleLayer(strcat("DRC ERROR CLASS/" nthelem(layer allLayer)) t)
					axlVisibleUpdate(t)
					axlUIWPrint(nil "Swtich layer complet...")
				else
					axlUIWPrint(nil "Error,Wrong layer argument! ~~~~(>_<)~~~~ ")
			);end if
		else
			axlUIWPrint(nil "Please Input argument...")
	);end if 
);end let
);end defun

defun(c_layer (@optional (layer ""))
let((allLayer countLayer)
	allLayer = axlGetParam("paramLayerGroup:ETCH")->groupMembers
	countLayer = length(allLayer)
	if(layer != "" && atoi(layer)
		then
			layer = atoi(layer)
			if( layer <= countLayer && layer > 0
				then
					axlVisibleDesign(nil)
					axlVisibleLayer(strcat("ETCH/" nthelem(layer allLayer)) t)
					axlVisibleLayer(strcat("PIN/" nthelem(layer allLayer)) t)
					axlVisibleLayer(strcat("VIA CLASS/" nthelem(layer allLayer)) t)    
					axlVisibleLayer(strcat("DRC ERROR CLASS/" nthelem(layer allLayer)) t)
					axlVisibleUpdate(t)
					axlUIWPrint(nil "Swtich layer complet...")
				else
					axlUIWPrint(nil "Error,Wrong layer argument! ~~~~(>_<)~~~~ ")
			);end if
		else
			axlUIWPrint(nil "Please Input argument...")
	);end if 
);end let
);end defun
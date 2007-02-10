####################################################################################
#
# Procedures d'entree-sortie g�rant des spectres
# Auteur : Benjamin MAUCLAIRE
# Date de cr�ation : 31-01-2005
# Date de mise a jour : 21-02-2005
# Chargement en script : 
# A130 : source $audace(rep_scripts)/spcaudace/spc_io.tcl
# A140 : source [ file join $audace(rep_plugin) tool spectro spcaudace spc_io.tcl ]
#
#####################################################################################


# Remarque (par Beno�t) : il faut mettre remplacer toutes les variables textes par des variables caption(mauclaire,...)
# qui seront initialis�es dans le fichier cap_mauclaire.tcl
# et renommer ce fichier mauclaire.tcl ;-)

#global audace


####################  Liste des fonctions ###############################
#
# spc_spc2png : converti un profil de raies format fits en une image format png avec gnuplot
#
#######################################################


#######################################################
#  Procedure d'ouverture de fichiers
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 29-01-2005
# Date modification : 29-01-2005
# Arguments : nom du fichier
########################################################

proc openfile { args } {
   global conf
   global audace

 if {[llength $args] == 1} {
   set filenamespc [ lindex $args 0 ]
   catch {unset profilspc} {}
   set profilspc(initialdir) [file dirname $audace(rep_images)]
   set profilspc(initialfile) [file tail $filenamespc]
   set input [open "$audace(rep_images)/$filenamespc" r]
   close input
   return $input
 } else {
   ::console::affiche_erreur "Usage: openfile fichier.fit\n\n"
 }
}
#****************************************************************#


#######################################################
#  Procedure d'ouverture de fichiers .dat avec interface graphique
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 29-01-2005
# Date modification : 29-01-2005
# Arguments : nom du fichier
########################################################

proc openfileg { {filename ""} } {
   global conf
   global audace
   global captionspc

   ## === Interfacage de l'ouverture du fichier profil de raie ===
   if {$filename==""} {
      # set idir ./
      # set ifile *.spc
      set idir $audace(rep_images)
      set ifile $conf(extension,defaut)

      if {[info exists profilspc(initialdir)] == 1} {
         set idir "$profilspc(initialdir)"
      }
      if {[info exists profilspc(initialfile)] == 1} {
         set ifile "$profilspc(initialfile)"
      }
      set filenamespc [tk_getOpenFile -title $captionspc(loadspctxt) -filetypes [list [list "$captionspc(spc_profile)" {.dat .$conf(extension,defaut)}]] -initialdir $idir -initialfile $ifile ]
      if {[string compare $filenamespc ""] == 0 } {
         return 0
      }
   }
   return $filenamespc
}
#****************************************************************#


#######################################################
#  Procedure d'ouverture de spectre au format fits
#
# Auteur : Benjamin MAUCLAIRE
# Date cr�ation : 15-02-2005
# Date modification : 15-02-2005 / 17-12-2005 / 20-12-2005
# Arguments : nom du repertoire/fichier
# Sortie : 
#  Si calibr� : liste contenant la liste des valeurs de l'intensit�, NAXIS1, CRVAL1, CDELT1, CRPIX1, CTYPE1.
#  Si non calibre : liste contenant la liste des valeurs de l'intensit�, NAXIS1
# Remarque : fonction appel�e par spc_loadfit (spc_profil.tcl)
########################################################

proc openspc { args } {
    global conf
    global audace
    #global profilspc

 if {[llength $args] == 1} {
   set filenamespc [ lindex $args 0 ]
   catch {unset profilspc} {}
   #set profilspc(initialdir) [file dirname $audace(rep_images)]
   #set profilspc(initialfile) [file tail $filenamespc]
   #set repertoire [file dirname $audace(rep_images)]
   #- Modif bug le 051221 : FIN BUG !
   set repertoire [file dirname $filenamespc]
   set fichier [file tail $filenamespc]

   #-- Remis le 16/12/2005
   #buf$audace(bufNo) load "$audace(rep_images)/$filenamespc"
   #-- Remis le 15/02/2005
   #buf$audace(bufNo) load "$filenamespc"
   #-- Mis le 17/12/2005
   #cd $repertoire
   #loadima $filenamespc -nodisp
   #-- Mis le 20/12/2005
   buf$audace(bufNo) load $repertoire/$fichier

   # Determine si c'est un spectre etalonne ou non etalonne en longueur d'onde
   set mot ""
   set flagcalib 0
   set motsheader [buf$audace(bufNo) getkwds]
   set len [llength $motsheader]
   for {set k 0} {$k<$len} {incr k} {
       set mot [lindex $motsheader $k]
       if { [string compare $mot "CRVAL1"] == 0 } {
	   set flagcalib 1
           break
       } else {
	   set flagcalib 0
       }
   }

   if { $flagcalib == 0 } {
       ::console::affiche_resultat "Ouverture d'un spectre non calibr� $filenamespc\n"
       set spectre [openspcncal $repertoire $fichier]
   } else {
       ::console::affiche_resultat "Ouverture d'un spectre calibr� $filenamespc\n"
       set spectre [openspccal $repertoire $fichier]
   }
   return $spectre
 } else {
   ::console::affiche_erreur "Usage: openspc fichier_profil.fit\n\n"
 }
}
#****************************************************************#


#######################################################
#  Procedure d'ouverture de spectre non calibr� (lambda) au format fits
#
# Auteur : Benjamin MAUCLAIRE
# Date cr�ation : 15-02-2005
# Date modification : 20-12-2005
# Arguments : nom du r�pertoire, nom du fichier
# Sortie : liste contenant la liste des valeurs de l'intensit�, NAXIS1
########################################################

proc openspcncal { args } {
   global conf
   global audace audela

 if {[llength $args] == 2} {
   set repertoire [ lindex $args 0 ]
   set filenamespc [ lindex $args 1 ]
   #catch {unset profilspc} {}
   #set profilspc(initialdir) [file dirname $audace(rep_images)]
   #set profilspc(initialfile) [file tail $filenamespc]
   buf$audace(bufNo) load $repertoire/$filenamespc
   #buf$audace(bufNo) load $filenamespc

   set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]

   if { [regexp {1.3.0} $audela(version) match resu ] } {
       for {set k 1} {$k<=$naxis1} {incr k} {
	   # Lit la valeur des elements du fichier fit
	   lappend intensites [buf$audace(bufNo) getpix [list $k 1]]
       }
   } else {
       for {set k 1} {$k<=$naxis1} {incr k} {
	   lappend intensites [ lindex [buf$audace(bufNo) getpix [list $k 1]] 1 ]
       }
   }
   set spectre [list $intensites $naxis1]
   return $spectre
 } else {
   ::console::affiche_erreur "Usage: openspcncal r�pertoire fichier_profil.fit\n\n"
 }
}
#****************************************************************#



#######################################################
#  Procedure d'ouverture de spectre calibr� (lambda) au format fits
#
# Auteur : Benjamin MAUCLAIRE
# Date cr�ation : 15-02-2005
# Date modification : 15-02-2005
# Arguments : r�pertoire nom du fichier
# Sortie : liste contenant la liste des valeurs de l'intensit�, NAXIS1, CRVAL1, CDELT1, CRPIX1, CTYPE1.
########################################################

proc openspccal { args } {
   global conf
   global audace audela

 if {[llength $args] == 2} {
   set repertoire [ lindex $args 0 ]
   set filenamespc [ lindex $args 1 ]
   #catch {unset profilspc} {}
   #set profilspc(initialdir) [file dirname $audace(rep_images)]
   ##set profilspc(initialfile) [file tail $filenamespc]
   ##buf$audace(bufNo) load "$audace(rep_images)/$filenamespc"
   #buf$audace(bufNo) load "$filenamespc"
   buf$audace(bufNo) load $repertoire/$filenamespc

   set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
   # Valeur minimale de l'abscisse : =0 si profil non �talonn�
   set xdepart [lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1]
   # Dispersion du spectre : =1 si profil non �talonn�
   set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
   # Pixel de l'abscisse centrale
   set xcenter [lindex [buf$audace(bufNo) getkwd "CRPIX1"] 1]
   # Type de spectre : LINEAR ou NONLINEAR
   set dtype [lindex [buf$audace(bufNo) getkwd "CTYPE1"] 1]

   if { [regexp {1.3.0} $audela(version) match resu ] } {
       for {set k 1} {$k<=$naxis1} {incr k} {
	   #-- Lit la valeur des elements du fichier fit
	   # lappend intensites [buf$audace(bufNo) getpix [list $k 1]]
	   #-- Gestion des valeurs "nan" de l'intensite
	   set ival [ buf$audace(bufNo) getpix [list $k 1] ]
	   #if { $ival == "nan" } {
	   #   lappend intensites 0
	   #   ::console::affiche_resultat "Cas nan : $ival\n"
	   #} else {
	   lappend intensites $ival
	   #}
       }
   } else {
       set ival [ lindex [ buf$audace(bufNo) getpix [list $k 1] ] 1 ]
   }
   set spectre [list $intensites $naxis1 $xdepart $xincr $xcenter "$dtype"]
   return $spectre
 } else {
   ::console::affiche_erreur "Usage: openspccal r�pertoire fichier_profil.fit\n\n"
 }
}
#****************************************************************#



#######################################################
#  Procedure d'ouverture d'un profil spectral au format fits
#
# Auteur : Benjamin MAUCLAIRE
# Date cr�ation : 11-12-2005
# Date modification : 11-12-2005
# Arguments : nom du fichier profil de raies calibr�
# Sortie : liste contenant la liste des valeurs des abscisses et intensit�s
########################################################

proc spc_openspcfits { args } {

    global conf
    global audace audela

    if {[llength $args] == 1} {
	set filenamespc [ lindex $args 0 ]
	set erreur [ lindex $args 1 ]
	buf$audace(bufNo) load $audace(rep_images)/$filenamespc
	#buf$audace(bufNo) load $filenamespc
	set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	#--- Valeur minimale de l'abscisse : =0 si profil non �talonn�
	set xdepart [lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1]
	#--- Dispersion du spectre : =1 si profil non �talonn�
	set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	::console::affiche_resultat "$naxis1 points � traiter\n"
	if { [regexp {1.3.0} $audela(version) match resu ] } {	
	    #--- Une liste commence � 0 ; Un vecteur fits commence � 1
	   for {set k 0} {$k<$naxis1} {incr k} {
		#--- Donne les bonnes valeurs aux abscisses si le spectre est �talonn� en longueur d'onde
		lappend abscisses [expr $xdepart+($k)*$xincr*1.0]
		#--- Lit la valeur (intensite) des elements du fichier fit
		lappend ordonnees [buf$audace(bufNo) getpix [list [expr $k+1] 1]]
	    }
       } else {
	   for {set k 0} {$k<$naxis1} {incr k} {
		#--- Donne les bonnes valeurs aux abscisses si le spectre est �talonn� en longueur d'onde
		lappend abscisses [expr $xdepart+($k)*$xincr*1.0]
		#--- Lit la valeur (intensite) des elements du fichier fit
		lappend ordonnees [ lindex [buf$audace(bufNo) getpix [list [expr $k+1] 1]] 1 ]
	    }
       }
       set sortie [list $abscisses $ordonnees]
       return $sortie
    } else {
	::console::affiche_erreur "Usage: spc_openspcfits fichier_profil.fit\n\n"
    }
}
#****************************************************************#


###################################################################
#  Procedure de conversion de fichier profil de raies .dat en .fit
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 31-01-2005
# Date modification : 15-02-2005/27-04-2006
# Arguments : fichier .dat du profil de raie ?fichier_sortie.fit?
###################################################################

proc spc_dat2fits { args } {

    global conf
    global audace spcaudace

    #set nbunit "float"
    set nbunit "double"
    set precision 0.05
    
    if { [llength $args] <= 2 } {
	if { [llength $args] == 1 } {
	    set filenamespc [ lindex $args 0 ]
	} elseif { [llength $args] == 2 } {
	    set filenamespc [ lindex $args 0 ]
	    set filenameout [ lindex $args 1 ]
	} else {
	    ::console::affiche_erreur "Usage: spc_dat2fits fichier_profil.dat ?fichier_sortie.fit?\n\n"
	    return 0
	}
	## === Lecture du fichier de donnees du profil de raie ===
	set input [open "$audace(rep_images)/$filenamespc" r]
	set contents [split [read $input] \n]
	close $input
	## === Extraction des numeros des pixels et des intensites ===
	#::console::affiche_resultat "ICI :\n $contents.\n"
	#set profilspc(naxis1) [expr [llength $contents]-2]
	set naxis1 [ expr [llength $contents]-1 ]
	#::console::affiche_resultat "$profilspc(naxis1)\n"
	set offset 1
	# Une liste commence � 0
	for {set k -1} {$k < $naxis1} {incr k} {
	    set ligne [lindex $contents $k]
	    append pixels "[lindex $ligne 0] "
	    append intensites "[lindex $ligne 1] "
	    #incr $k
	}
	#::console::affiche_resultat "$profilspc(pixels)\n"
	
	# === On prepare les vecteurs a afficher ===
	# len : longueur du profil (NAXIS1)
	#set len [llength $profilspc(pixels)]
	set nintensites ""
	for {set k 0} {$k<=$naxis1} {incr k} {
	    append nintensites " [lindex $intensites $k]"
	}
	
	#--- Initialisation � blanc d'un fichier fits
	buf$audace(bufNo) setpixels CLASS_GRAY $naxis1 1 FORMAT_FLOAT COMPRESS_NONE 0
	buf$audace(bufNo) setkwd [ list NAXIS 1 int "" "" ]
	buf$audace(bufNo) setkwd [ list NAXIS1 $naxis1 int "" "" ]
	
	#--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
	# Une liste commence � 0 ; Un vecteur fits commence � 1
	set intensite 0
	for {set k 0} {$k<$naxis1} {incr k} {
	    append intensite [lindex $nintensites $k]
	    #::console::affiche_resultat "$intensite\n"
	    if { [regexp {([0-9]+\.*[0-9]*)} $intensite match mintensite] } {
		buf$audace(bufNo) setpix [list [expr $k+1] 1] $mintensite
		set intensite 0
	    }
	}
	
	#=============================
	
	set flag 0
	if { $flag == 1 } {
	    #--- Type de dispersion : LINEAR, NONLINEAR
	    
	    #--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
	    # Une liste commence � 0 ; Un vecteur fits commence � 1
	    set intensite 0
	    for {set k 0} {$k<$naxis1} {incr k} {
		append intensite [lindex $nintensites $k]
		#::console::affiche_resultat "$intensite\n"
		if { [regexp {([0-9]+\.*[0-9]*)} $intensite match mintensite] } {
		    buf$audace(bufNo) setpix [list [expr $k+1] 1] $mintensite
		    set intensite 0
		}
	    }
	}
	#==============================
	
	#------- Affecte une valeur aux mots cle li�s � la spectroscopie ----------
	#-- buf$audace(bufNo) setkwd [list "INSTRU" "T310" string "mon t�lescope" ""]
	#buf$audace(bufNo) setkwd [list "NAXIS1" "$len" int "" ""]
	#buf$audace(bufNo) setkwd [list "NAXIS2" "1" int "" ""]
	#--- Valeur minimale de l'abscisse (xdepart) : =0 si profil non �talonn�
	set xdepart [ expr 0.0+1.0*[lindex $pixels 0] ]
	buf$audace(bufNo) setkwd [list "CRVAL1" $xdepart $nbunit "" ""]
	
	#--- Valeur de la longueur d'onde/pixel de fin ***** naxis1ici-1=naxis1-2 A VERFIFIER ****
	set xdernier [lindex $pixels [expr $naxis1-1]]
	::console::affiche_resultat "Xdep : $xdepart ; Xfin : $xdernier\n"
	#set xcentre [expr int(0.5*($xdernier-$xdepart))]
	#buf$audace(bufNo) setkwd [list "CRPIX1" "$xcentre" double "" ""]
	
	#--- Dispersion du spectre :
	#-- Calcul dans le cas d'une dispersion non lin�aire
	set l1 [lindex $pixels 1]
	set l2 [lindex $pixels [expr int($naxis1/10)]]
	set l3 [lindex $pixels [expr int(2*$naxis1/10)]]
	set l4 [lindex $pixels [expr int(3*$naxis1/10)]]
	set dl1 [expr ($l2-$l1)/(int($naxis1/10)-1.0)]
	set dl2 [expr ($l4-$l3)/(int($naxis1/10)-1.0)]
	set dispersion [expr 0.5*($dl2+$dl1)]
	#-- Mesure de la dispersion suppos�e lin�aire
	set l1 [lindex $pixels 1]
	set l2 [lindex $pixels 2]
	set dispersion [expr 1.0*abs($l2-$l1)]
	
	#-- Meth2 : erreur si spectre de moins de 4 pixels
	#set l2 [lindex $profilspc(pixels) 4]
	#set l3 [lindex $profilspc(pixels) [expr 1+int($len/2)]]
	#set l4 [lindex $profilspc(pixels) [expr 4+int($len/2)]]
	#set dl1 [expr ($l2-$l1)/3]
	#set dl2 [expr ($l4-$l3)/3]
	#set xincr [expr 0.5*($dl2+$dl1)]
	
	#-- Ecriture du mot clef
	::console::affiche_resultat "Dispersion : $dispersion\n"
	if { $xdepart == 1.0 } {
	    buf$audace(bufNo) setkwd [list "CDELT1" $dispersion $nbunit "" "pixel"]
	} else {
	    buf$audace(bufNo) setkwd [list "CDELT1" $dispersion $nbunit "" "Angstrom/pixel"]
	}
	
	#--- Type de dispersion : LINEAR, NONLINEAR
	
	#--- Sauve le fichier fits ainsi constitu� 
	::console::affiche_resultat "$naxis1 lignes affect�es\n"
	buf$audace(bufNo) bitpix float
	if {[llength $args] == 1} {
	    set nom [ file rootname $filenamespc ]
	    buf$audace(bufNo) bitpix float
	    buf$audace(bufNo) save "$audace(rep_images)/${nom}$conf(extension,defaut)"
	    ::console::affiche_resultat "Fichier fits sauv� sous $audace(rep_images)/${nom}$conf(extension,defaut)\n"
	    return ${nom}
	} elseif {[llength $args] == 2} {
	    set nom [ file rootname $filenameout ]
	    buf$audace(bufNo) bitpix float
	    buf$audace(bufNo) save "$audace(rep_images)/${filenameout}$conf(extension,defaut)"
	    ::console::affiche_resultat "Fichier fits sauv� sous $audace(rep_images)/${filenameout}$conf(extension,defaut)\n"
	    return ${filenameout}
	}
	#buf$audace(bufNo) bitpix short
    } else {
	::console::affiche_erreur "Usage: spc_dat2fits fichier_profil.dat ?fichier_sortie.fit?\n\n"
    }
}
#****************************************************************#




####################################################################
#  Procedure de conversion de fichier profil de raie spatial .fit en .dat
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 29-01-2005
# Date modification : 16-05-2005/20-09-06
# Arguments : fichier .fit du profil de raie ?fichier_sortie.dat?\
####################################################################

proc spc_fits2dat { args } {

  global conf
  global audace spcaudace
  global audela
  #global profilspc
  # global captionspc
  global colorspc

  if {[llength $args] <= 2} {
     if  {[llength $args] == 1} {
	 set filenamespc [ lindex $args 0 ]
     } elseif {[llength $args] == 2} {
	 set filenamespc [ lindex $args 0 ]
	 set filenameout [ lindex $args 1 ]
     } else {
	 ::console::affiche_erreur "Usage: spc_fits2dat fichier_profil.fit ?fichier_sortie.dat?\n\n"
	 return 0
     }

 
     buf$audace(bufNo) load "$audace(rep_images)/$filenamespc"
     set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
     set listemotsclef [ buf$audace(bufNo) getkwds ]
     if { [ lsearch $listemotsclef "CRVAL1" ] !=-1 } {
	 set lambda0 [ lindex [ buf$audace(bufNo) getkwd "CRVAL1" ] 1 ]
     }
     if { [ lsearch $listemotsclef "CDELT1" ] !=-1 } {
	 set dispersion [ lindex [buf$audace(bufNo) getkwd "CDELT1" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_A" ] !=-1 } {
	 set spc_a [ lindex [ buf$audace(bufNo) getkwd "SPC_A" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_B" ] !=-1 } {
	 set spc_b [ lindex [ buf$audace(bufNo) getkwd "SPC_B" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_C" ] !=-1 } {
	 set spc_c [ lindex [ buf$audace(bufNo) getkwd "SPC_C" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_D" ] !=-1 } {
	 set spc_d [ lindex [ buf$audace(bufNo) getkwd "SPC_D" ] 1 ]
     } else {
	 set spc_d 0.0
     }

     #--- Type de spectre : LINEAR ou NONLINEAR (elinine les espaces dans la valeur du mot clef.
     # set dtype [string trim [lindex [buf$audace(bufNo) getkwd "CTYPE1"] 1]]
     #::console::affiche_resultat "Ici 1\n"
     #if { $dtype != "LINEAR" || $dtype == "" } {
	 #    ::console::affiche_resultat "Le spectre ne poss�de pas une disersion lin�aire. Pas de conversion possible.\n"
	 #    break
     #}
     #::console::affiche_resultat "Ici 2\n"
     set len [expr int($naxis1/$dispersion)]
     ::console::affiche_resultat "$naxis1 intensit�s � traiter\n"

     if {[llength $args] == 1} {
	 set fileetalonnespc [ file rootname $filenamespc ]
	 set fileout ${fileetalonnespc}$spcaudace(extdat)
	 set file_id [open "$audace(rep_images)/$fileout" w+]
     } elseif {[llength $args] == 2} {
	 set fileout $filenameout
	 set file_id [open "$audace(rep_images)/$fileout" w+]
     } else {
	 ::console::affiche_erreur "Usage: spc_fits2dat fichier_profil_fit ?fichier_sortie.dat?\n\n"
	 return 0
     }

     if { [regexp {1.3.0} $audela(version) match resu ] } {
	 #--- Lecture pixels Audela 130 :
	 if { $lambda0 != 1 } {
	     if { [ lsearch $listemotsclef "SPC_A" ] !=-1 } {
		 #-- Calibration non-lin�aire :
		 if { $spc_a < 0.01 } {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 #- Ancienne formulation < 070104 :
			 set lambda [expr $spc_a*$k*$k+$spc_b*$k+$spc_c ]
			 set intensite [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
			 puts $file_id "$lambda\t$intensite\r"
		     }
		 } else {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 set lambda [expr $spc_d*$k*$k*$k+$spc_c*$k*$k+$spc_b*$k+$spc_a ]
			 set intensite [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
			 puts $file_id "$lambda\t$intensite\r"
		     }
		 }
	     } else {
		 #-- Calibration lin�aire :
		 #-- Une liste commence � 0 ; Un vecteur fits commence � 1
		 for {set k 0} {$k<$naxis1} {incr k} {
		     #-- Donne les bonnes valeurs aux abscisses si le spectre est �talonn� en longueur d'onde
		     set lambda [ expr $lambda0+($k)*$dispersion*1.0 ]
		     #-- Lit la valeur des elements du fichier fit
		     set intensite [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
		     ##lappend profilspc(intensite) $intensite
		     #-- Ecrit les couples "Lambda Intensite" dans le fichier de sortie
		     puts $file_id "$lambda\t$intensite"
		 }
	     }
	 } else {
	     #-- Profil non calibr� :
	     for {set k 0} {$k<$naxis1} {incr k} {
		 set pixel [expr $k+1]
		 set intensite [buf$audace(bufNo) getpix [list [expr $k+1] 1]]
		 puts $file_id "$pixel\t$intensite\r"
	     }
	 }
     } else {
	 #--- Lecture pixels Audela 140 :
	 if { $lambda0 != 1 } {
	     if { [ lsearch $listemotsclef "SPC_A" ] !=-1 } {
		 #-- Calibration non-lin�aire :
		 if { $spc_a < 0.01 } {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 #- Ancienne formulation < 070104 :
			 set lambda [expr $spc_a*$k*$k+$spc_b*$k+$spc_c ]
			 set intensite [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
			 puts $file_id "$lambda\t$intensite\r"
		     }
		 } else {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 set lambda [expr $spc_d*$k*$k*$k+$spc_c*$k*$k+$spc_b*$k+$spc_a ]
			 set intensite [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
			 puts $file_id "$lambda\t$intensite\r"
		     }
		 }
	     } else {
		 #-- Calibration lin�aire :
		 #-- Une liste commence � 0 ; Un vecteur fits commence � 1
		 for {set k 0} {$k<$naxis1} {incr k} {
		     #-- Donne les bonnes valeurs aux abscisses si le spectre est �talonn� en longueur d'onde
		     set lambda [ expr $lambda0+($k)*$dispersion*1.0 ]
		     #-- Lit la valeur des elements du fichier fit
		     set intensite [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
		     ##lappend profilspc(intensite) $intensite
		     #-- Ecrit les couples "Lambda Intensite" dans le fichier de sortie
		     puts $file_id "$lambda\t$intensite"
		 }
	     }
	 } else {
	     #-- Profil non calibr� :
	     for {set k 0} {$k<$naxis1} {incr k} {
		 set pixel [expr $k+1]
		 set intensite [ lindex [buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
		 puts $file_id "$pixel\t$intensite\r"
	     }
	 }
     }

     close $file_id
     ::console::affiche_resultat "Fichier fits export� sous $audace(rep_images)/$fileout\n"
     #--- Renvoie le nom du fichier avec l'extension $extsp :
     return $fileout
  } else {
     ::console::affiche_erreur "Usage: spc_fits2dat fichier_profil_fit ?fichier_sortie.dat?\n\n"
  }
}
#****************************************************************#





####################################################################
# Procedure de cr�ation d'un fichier profil de raie fits � partir des donn�es x et y
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 15-12-2005
# Date modification : 15-12-2005
# Arguments : nom fichier fit de sortie, une liste de coordonn�es x puis y, unit� des donn�es
####################################################################

proc spc_data2fits { args } {
    global audace
    global conf
    set precision 0.0001
    #set nbunit "float"
    #set nbunit "double"

  if { [llength $args] <= 3 } {
    if { [llength $args] == 3 } {
	#set nom_fichier [ file rootname [lindex 0] ]
	set nom_fichier [lindex $args 0]
	set coordonnees [lindex $args 1]
	set nbunit [lindex $args 2]
    } elseif { [llength $args] == 2 } {
	set nom_fichier [lindex $args 0]
	set coordonnees [lindex $args 1]
	set nbunit "float"
    } else {
	::console::affiche_erreur "Usage: spc_data2fits nom_fichier_fits_sortie liste_coordonn�es_x_et_y unit�es_coordonn�es (float/double)\n\n"
	return 0
    }

	set abscisses [lindex $coordonnees 0]
	set intensites [lindex $coordonnees 1]
	set len [llength $abscisses]

	#--- Cr�ation du fichier fits
	buf$audace(bufNo) setpixels CLASS_GRAY $len 1 FORMAT_FLOAT COMPRESS_NONE 0
	buf$audace(bufNo) setkwd [ list "NAXIS" 1 int "" "" ]
	buf$audace(bufNo) setkwd [ list "NAXIS1" $len int "" "" ]
	# buf$audace(bufNo) setkwd [list "NAXIS2" 1 int "" ""]

	#--- Valeur minimale de l'abscisse (xdepart) : =0 si profil non �talonn�
	set xdepart [expr 1.0*[lindex $abscisses 0] ]
	buf$audace(bufNo) setkwd [list "CRVAL1" $xdepart $nbunit "" "Angstrom"]

	#--- Calcul d'une dispersion moyenne :
	set l1 [lindex $abscisses 1]
	set l2 [lindex $abscisses 2]
	set disp1 [expr 1.0*abs($l2-$l1)]
	set l1 [lindex $abscisses [expr int($len/2)] ]
	set l2 [lindex $abscisses [expr int($len/2)+1] ]
	set disp2 [expr 1.0*abs($l2-$l1)]
	if { [ expr abs($disp2- $disp1) ] <= $precision } {
	    #-- Mesure de la dispersion supos�e lin�aire
	    set dispersion $disp1
	} else {
	    #-- Mesure de la dispersion supos�e non-lin�aire
	    #set dispersion [spc_dispersion_moy $abscisses]
	    set l1 [lindex $abscisses 1]
	    set l2 [lindex $abscisses [expr int($len/10)]]
	    set l3 [lindex $abscisses [expr int(2*$len/10)]]
	    set l4 [lindex $abscisses [expr int(3*$len/10)]]
	    set dl1 [expr ($l2-$l1)/(int($len/10)-1)]
	    set dl2 [expr ($l4-$l3)/(int($len/10)-1)]
	    set dispersion [expr 0.5*($dl2+$dl1)]
	    #set dispersion $disp1
	}

	#--- Calcul de la dispersion par r�gression lin�aire :
	for {set k 1} {$k<=$len} {incr k} {
	    lappend xpos $k
	}
	set listevals [ list $abscisses $xpos ]
	set coeffsdeg1 [ spc_reglin $listevals ]
	set dispersion [ expr 1.0/[ lindex $coeffsdeg1 0 ] ]

	
	#--- Mise � jour fichier fits
	buf$audace(bufNo) setkwd [list "CDELT1" $dispersion $nbunit "" "Angstrom/pixel"]

	#--- Type de dispersion : LINEAR, NONLINEAR
	#if { [expr abs($dl2-$dl1)] <= $precision } {
	#    buf$audace(bufNo) setkwd [list "CTYPE1" "LINEAR" string "" ""]
	#} elseif { [expr abs($dl2-$dl1)] > $precision } {
	#    buf$audace(bufNo) setkwd [list "CTYPE1" "NONLINEAR" string "" ""]
	#}

	#--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
	# Une liste commence � 0 ; Un vecteur fits commence � 1
	set intensite 0
	for {set k 0} {$k<$len} {incr k} {
	    append intensite [lindex $intensites $k]
	    #::console::affiche_resultat "$intensite\n"
	    if { [regexp {([0-9]+\.*[0-9]*)} $intensite match mintensite] } {
		buf$audace(bufNo) setpix [list [expr $k+1] 1] $mintensite
		set intensite 0
	    }
	}

	#--- Sauvegarde du fichier fits ainsi cr��
	if { $nbunit == "double" || $nbunit == "float" } {
	    buf$audace(bufNo) bitpix float
	} elseif { $nbunit == "int" } {
	    buf$audace(bufNo) bitpix short
	}
	buf$audace(bufNo) save "$audace(rep_images)/$nom_fichier$conf(extension,defaut)"
	return $nom_fichier
    } else {
       ::console::affiche_erreur "Usage: spc_data2fits nom_fichier_fits_sortie liste_coordonn�es_x_et_y unit�es_coordonn�es (float/double)\n\n"
    }
}
#**********************************************************************#




####################################################################
#  Procedure de conversion de fichier profil de raie fits en une liste contenant les listes valeurs X et Y.
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 20-12-2005
# Date modification : 20-12-2005/20-09-06
# Arguments : fichier .fit du profil de raie
####################################################################

proc spc_fits2data { args } {

 global conf
 global audace audela

 if {[llength $args] == 1} {
     set filenamespc [ lindex $args 0 ]

     buf$audace(bufNo) load "$audace(rep_images)/$filenamespc"
     set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
     set listemotsclef [ buf$audace(bufNo) getkwds ]
     if { [ lsearch $listemotsclef "CRVAL1" ] !=-1 } {
	 set lambda0 [ lindex [ buf$audace(bufNo) getkwd "CRVAL1" ] 1 ]
     }
     if { [ lsearch $listemotsclef "CDELT1" ] !=-1 } {
	 set dispersion [ lindex [buf$audace(bufNo) getkwd "CDELT1" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_A" ] !=-1 } {
	 set spc_a [ lindex [ buf$audace(bufNo) getkwd "SPC_A" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_B" ] !=-1 } {
	 set spc_b [ lindex [ buf$audace(bufNo) getkwd "SPC_B" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_C" ] !=-1 } {
	 set spc_c [ lindex [ buf$audace(bufNo) getkwd "SPC_C" ] 1 ]
     }
     if { [ lsearch $listemotsclef "SPC_D" ] !=-1 } {
	 set spc_d [ lindex [ buf$audace(bufNo) getkwd "SPC_D" ] 1 ]
     } else {
	 set spc_d 0.0
     }

     #--- Valeur minimale de l'abscisse : =0 si profil non �talonn�
     ::console::affiche_resultat "$naxis1 intensit�s � traiter...\n"

     #---- Pour Audela
     if { [regexp {1.3.0} $audela(version) match resu ] } {
	 #--- Spectre calibr� en lambda
	 if { $lambda0 != 1 } {
	     #-- Calibration non-lin�aire :
	     if { [ lsearch $listemotsclef "SPC_A" ] !=-1 } {
		 if { $spc_a < 0.01 } {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 #- Ancienne formulation < 070104 :
			 #- Une liste commence � 0 ; Un vecteur fits commence � 1
			 lappend abscisses [ expr $spc_a*$k*$k+$spc_b*$k+$spc_c ]
			 lappend intensites [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
		     }
		 } else {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 #- Une liste commence � 0 ; Un vecteur fits commence � 1
			 lappend abscisses [ expr $spc_d*$k*$k*$k+$spc_c*$k*$k+$spc_b*$k+$spc_a ]
			 lappend intensites [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
		     }
		 }
		 #-- Calibration lin�aire :
	     } else {
		 for {set k 0} {$k<$naxis1} {incr k} {
		     lappend abscisses [ expr $lambda0+($k)*$dispersion*1.0 ]
		     lappend intensites [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
		 }
	     }
	 } else {
	     #--- Spectre non calibr� en lambda :	 
	     for {set k 0} {$k<$naxis1} {incr k} {
		 lappend abscisses [ expr $k+1 ]
		 lappend intensites [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
	     }
	 }
     #---- Audela 140 :
     } else {
	 #--- Spectre calibr� en lambda
	 if { $lambda0 != 1 } {
	     #-- Calibration non-lin�aire :
	     if { [ lsearch $listemotsclef "SPC_A" ] !=-1 } {
		 if { $spc_a < 0.01 } {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 #- Ancienne formulation < 070104 :
			 #- Une liste commence � 0 ; Un vecteur fits commence � 1
			 lappend abscisses [ expr $spc_a*$k*$k+$spc_b*$k+$spc_c ]
			 lappend intensites [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
		     }
		 } else {
		     for {set k 0} {$k<$naxis1} {incr k} {
			 #- Une liste commence � 0 ; Un vecteur fits commence � 1
			 lappend abscisses [ expr $spc_d*$k*$k*$k+$spc_c*$k*$k+$spc_b*$k+$spc_a ]
			 lappend intensites [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
		     }
		 }
		 #-- Calibration lin�aire :
	     } else {
		 for {set k 0} {$k<$naxis1} {incr k} {
		     lappend abscisses [ expr $lambda0+($k)*$dispersion*1.0 ]
		     lappend intensites [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
		 }
	     }
	 } else {
	     #--- Spectre non calibr� en lambda :	 
	     for {set k 0} {$k<$naxis1} {incr k} {
		 lappend abscisses [ expr $k+1 ]
		 lappend intensites [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ] 
	     }
	 }
     }
     set coordonnees [list $abscisses $intensites]
     return $coordonnees
 } else {
     ::console::affiche_erreur "Usage: spc_fits2data fichier_fits_profil.fit\n\n"
 }
}
#****************************************************************#


####################################################################
#  Procedure de conversion de fichier profil de raie fits en une liste contenant les listes valeurs X et Y.
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 24-09-2006
# Date modification : 20-12-2005/24-09-06
# Arguments : fichier .fit du profil de raie
####################################################################

proc spc_fits2datadlin { args } {

 global conf
 global audace audela

 if {[llength $args] == 1} {
     set filenamespc [ lindex $args 0 ]

     buf$audace(bufNo) load "$audace(rep_images)/$filenamespc"
     set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
     set listemotsclef [ buf$audace(bufNo) getkwds ]
     if { [ lsearch $listemotsclef "CRVAL1" ] !=-1 } {
	 set lambda0 [ lindex [ buf$audace(bufNo) getkwd "CRVAL1" ] 1 ]
     }
     if { [ lsearch $listemotsclef "CDELT1" ] !=-1 } {
	 set dispersion [ lindex [buf$audace(bufNo) getkwd "CDELT1" ] 1 ]
     }
     #--- Valeur minimale de l'abscisse : =0 si profil non �talonn�
     ::console::affiche_resultat "$naxis1 intensit�s � traiter...\n"

     #---- Audela 130 :
     if { [regexp {1.3.0} $audela(version) match resu ] } {
	 #--- Spectre calibr� en lambda de dispersion impos�e lin�aire :
	 if { $lambda0 != 1 } {
	     for {set k 0} {$k<$naxis1} {incr k} {
		 lappend abscisses [ expr $lambda0+($k)*$dispersion*1.0 ]
		 lappend intensites [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
	     }
	 } else {
	     #--- Spectre non calibr� en lambda :	 
	     for {set k 0} {$k<$naxis1} {incr k} {
		 lappend abscisses [ expr $k+1 ]
		 lappend intensites [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ]
	     }
	 }
     #---- Audela 140 :
     } else {
	 #--- Spectre calibr� en lambda de dispersion impos�e lin�aire :
	 if { $lambda0 != 1 } {
	     for {set k 0} {$k<$naxis1} {incr k} {
		 lappend abscisses [ expr $lambda0+($k)*$dispersion*1.0 ]
		 lappend intensites [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
	     }
	 } else {
	     #--- Spectre non calibr� en lambda :	 
	     for {set k 0} {$k<$naxis1} {incr k} {
		 lappend abscisses [ expr $k+1 ]
		 lappend intensites [ lindex [ buf$audace(bufNo) getpix [list [expr $k+1] 1] ] 1 ]
	     }
	 }
     }
     set coordonnees [list $abscisses $intensites]
     return $coordonnees
 } else {
     ::console::affiche_erreur "Usage: spc_fits2datadlin fichier_fits_profil.fit\n\n"
 }
}
#****************************************************************#



####################################################################
#  Procedure de conversion de fichier profil de raie .fit en .png
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 14-08-2005
# Date modification : 26-04-2006
# Arguments : fichier .fit du profil de raie, l�gende axe X, l�gende axe Y, pas
####################################################################

proc spc_fit2pngman { args } {
    global audace spcaudace
    global conf

    if { [llength $args] == 5 } {
	set fichier [ lindex $args 0 ]
	set titre [ lindex $args 1 ]
	set legendex [ lindex $args 2 ]
	set legendey [ lindex $args 3 ]
	set pas [ lindex $args 4 ]

	spc_fits2dat $fichier
	# Retire l'extension .fit du nom du fichier
	set spcfile [ file rootname $fichier ]

	#--- Prepare le script pour gnuplot
	set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
	put $file_id "call \"$spcaudace(repgp)/gp_spc.cfg\" \"${spcfile}$spcaudace(extdat)\" \"$titre\" * * * * $pas \"${spcfile}.png\" \"$legendex\" \"$legendey\" "
	close $file_id

	#--- Execute Gnuplot pour l'export en png
	if { $tcl_platform(os)=="Linux" } {
	    set answer [ catch { exec gnuplot $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	} else {
	    set answer [ catch { exec ${repertoire_gp}/gpwin32/pgnuplot.exe $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	}
	::console::affiche_resultat "Profil de raie export� sous ${spcfile}.png\n"
	return "${spcfile}.png"
   } else {
       ::console::affiche_erreur "Usage: spc_fit2pngman fichier_fits titre l�gende_axeX l�gende_axeY intervalle_graduations\n\n"
   }
}
####################################################################



####################################################################
#  Procedure de conversion de fichier profil de raie calibr� .fit en .png
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 14-08-2005
# Date modification : 26-04-2006
# Arguments : fichier .fit du profil de raie, titre, ?xdeb, xfin?
####################################################################

proc spc_fit2png { args } {
    global audace spcaudace
    global conf
    global tcl_platform

    #-- 3%=0.03
    set lpart 0

    if { [llength $args] == 4 || [llength $args] == 2 } {
	if { [llength $args] == 2 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    #set xdeb "*"
	    #set xfin "*"
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de 3% des limites pour l'esthetique
	    set largeur [ expr $lpart*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	} elseif { [llength $args] == 4 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    set xdeb [ lindex $args 2 ]
	    set xfin [ lindex $args 3 ]
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	}

	#--- Adapte la l�gende de l'abscisse
	if { $xdeb0 == 1.0 } {
	    set legendex "Position (Pixel)"
	} else {
	    set legendex "Wavelength (A)"
	}
       
	set legendey "Relative intensity"
	
	spc_fits2dat $fichier
	#-- Retire l'extension .fit du nom du fichier
	set spcfile [ file rootname $fichier ]
	
	#--- Cr��e le fichier script pour gnuplot :
	## exec echo "call \"$spcaudace(repgp)/gpx11.cfg\" \"${spcfile}$ext\" \"$titre\" * * $xdeb $xfin $pas \"${spcfile}.png\" \"$legendex\" \"$legendey\" " > $repertoire_gp/run_gp
	# exec echo "call \"$spcaudace(repgp)/gp_novisu.cfg\" \"${spcfile}$ext\" \"$titre\" * * $xdeb $xfin * \"${spcfile}.png\" \"$legendex\" \"$legendey\" " > $repertoire_gp/run_gp
	set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
	puts $file_id "call \"$spcaudace(repgp)/gp_novisu.cfg\" \"$audace(rep_images)/${spcfile}$spcaudace(extdat)\" \"$titre\" * * $xdeb $xfin * \"$audace(rep_images)/${spcfile}.png\" \"$legendex\" \"$legendey\" "
	close $file_id

	#--- D�termine le chemin de l'executable Gnuplot selon le syst�me d'exploitation :
	#exec gnuplot "$audace(rep_images)/${spcfile}.gp"
	# exec gnuplot $repertoire_gp/run_gp
	# exec rm -f $repertoire_gp/run_pg
	if { $tcl_platform(os)=="Linux" } {
	    # set gnuplotex "/usr/bin/gnuplot"
	    # catch { exec $gnuplotex "$audace(rep_images)/${spcfile}.gp" }
	    set answer [ catch { exec gnuplot $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	} else {
	    # set gnuplotex "C:\Programms Files\gnuplot\gnuplot.exe"
	    # exec $gnuplotex "$audace(rep_images)/${spcfile}.gp"
	    # exec gnuplot "$audace(rep_images)/${spcfile}.gp"
	    #-- wgnuplot et pgnuplot doivent etre dans le rep gp de spcaudace
	    set answer [ catch { exec $spcaudace(repgp)/gpwin32/pgnuplot.exe $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	}

	#--- Effacement des fichiers de batch :
	file delete "$audace(rep_images)/${spcfile}$spcaudace(extdat)"
	file delete "$audace(rep_images)/${spcfile}.gp"
	::console::affiche_resultat "Profil de raie export� sous ${spcfile}.png\n"
	return "${spcfile}.png"
    } else {
	::console::affiche_erreur "Usage: spc_fit2png fichier_fits \"Titre\" ?xd�but xfin?\n\n"
    }
}
####################################################################



####################################################################
#  Procedure de conversion d'une s�rie de fichiers profil de raie calibr� .fit en .png evec pr�cision de la l�gende
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 03-01-2007
# Date modification : 03-01-2007
# Arguments : fichier .fit du profil de raie, titre, ?legende_x legende_y?
####################################################################

proc spc_fit2pngleg { args } {
    global audace spcaudace
    global conf
    global tcl_platform
    set repertoire_gp [ file join $audace(rep_scripts) spcaudace gp ]
    #-- 3%=0.03
    set lpart 0

    if { [llength $args] == 6 || [llength $args] == 4 || [llength $args] == 2 } {
	if { [llength $args] == 2 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]

	    #-- Adapte la l�gende de l'abscisse
	    if { $xdeb0 == 1.0 && $legendex=="" } {
		set legendex "Position (Pixel)"
	    } else {
		set legendex "Wavelength (A)"
	    }
	    set legendey "Relative intensity"
	    #-- D�termine les bornes du graphique :
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de "lpart" % des limites pour l'esthetique
	    set largeur [ expr $lpart*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]	    
	} elseif { [llength $args] == 4 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    set legendex [ lindex $args 2 ]
	    set legendey [ lindex $args 3 ]

	    #-- D�termine les bornes du graphique :
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de "lpart" % des limites pour l'esthetique
	    set largeur [ expr $lpart*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	} elseif { [llength $args] == 6 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    set legendex [ lindex $args 2 ]
	    set legendey [ lindex $args 3 ]
	    set xdeb [ lindex $args 4 ]
	    set xfin [ lindex $args 5 ]
	} else {
	    ::console::affiche_erreur "Usage: spc_fit2pngleg fichier_fits \"Titre\" ?legende_x lende_y? ?xdeb xfin?\n\n"
	    return 0
	}       

	#-- spc_fits2dat renvoie un nom avec une extension : fichier.dat
	set fileout [ spc_fits2dat $fichier ]
	#-- Retire l'extension .fit du nom du fichier
	set spcfile [ file rootname $fichier ]
	
	#--- Cr��e le fichier script pour gnuplot :
	set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
	puts $file_id "call \"$spcaudace(repgp)/gp_novisu.cfg\" \"$audace(rep_images)/${spcfile}$spcaudace(extdat)\" \"$titre\" * * $xdeb $xfin * \"$audace(rep_images)/${spcfile}.png\" \"$legendex\" \"$legendey\" "
	close $file_id

	#--- D�termine le chemin de l'executable Gnuplot selon le syst�me d'exploitation :
	if { $tcl_platform(os)=="Linux" } {
	    set answer [ catch { exec gnuplot $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	} else {
	    set answer [ catch { exec $spcaudace(repgp)/gpwin32/pgnuplot.exe $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	}

	#--- Effacement des fichiers de batch :
	file delete "$audace(rep_images)/${spcfile}$spcaudace(extdat)"
	file delete "$audace(rep_images)/${spcfile}.gp"

	#--- Fin du script :
	::console::affiche_resultat "Profil de raie export� sous ${spcfile}.png\n"
	return "${spcfile}.png"
    } else {
	::console::affiche_erreur "Usage: spc_fit2pngleg fichier_fits \"Titre\" ?legende_x lende_y? ?xdeb xfin?\n\n"
    }
}
####################################################################



####################################################################
#  Procedure de conversion de fichier profil de raie calibr� .fit en .png
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 29-01-2007
# Date modification : 29-01-2007
# Arguments : fichiers .fit du profil de raie
####################################################################

proc spc_multifit2png { args } {
    global audace spcaudace
    global conf
    global tcl_platform

    #-- 3%=0.03
    set lpart 0

    if { [llength $args] != 0 } {

	set len [ llength $args ]
	#--- Creation d'une liste de fichier sans extension :
	set listefile ""
	set i 1
	foreach fichier $args {
	    #if { $i==1 } {
		#set verticaloffset [ lindex $args 0 ]
	    #} else {
		lappend listefile [ file rootname $fichier ]
	    #}
	    incr i
	}

	#--- Adapte la l�gende de l'abscisse :
	set fichier1 [ lindex $args 0 ]
	buf$audace(bufNo) load "$audace(rep_images)/$fichier1"
	set listemotsclef [ buf$audace(bufNo) getkwds ]
	if { [ lsearch $listemotsclef "CRVAL1" ] !=-1 } {
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	} else {
	    set xdeb 1.0
	}
	if { $xdeb0 == 1.0 } {
	    set legendex "Position (Pixel)"
	} else {
	    set legendex "Wavelength (A)"
	}
	set titre ""
	set legendey ""

	#--- Conversion en dat :
	set i 1
	set listedat ""
	set plotcmd ""
	foreach fichier $listefile {
	    set filedat [ spc_fits2dat $fichier ]
	    lappend listedat $filedat
	    if { $i != $len } {
		#append plotcmd "'$audace(rep_images)/$filedat' w l, "
		append plotcmd "'$filedat' w l, "
		#append plotcmd "'$filedat' using 1:($2+$i) w l, "
	    } elseif { $i==1 } {
		append plotcmd "'$filedat' w l, "
	    } else {
		#append plotcmd "'$audace(rep_images)/$filedat' w l"
		append plotcmd "'$filedat' w l"
	    }
	    incr i
	}

	#--- Construction du fichier btach de Gnuplot :
	set xdeb "*"
	set xfin "*"
	set file_id [open "$audace(rep_images)/multiplot.gp" w+]
	# puts $file_id "call \"$spcaudace(repgp)/gp_multi.cfg\" \"$plotcmd\" \"$titre\" * * $xdeb $xfin * \"$audace(rep_images)/multiplot.png\" \"$legendex\" \"$legendey\" "
	puts $file_id "call \"$spcaudace(repgp)/gp_multi.cfg\" \"$plotcmd\" \"$titre\" * * $xdeb $xfin * \"$audace(rep_images)/multiplot.png\" \"$legendex\" "
	close $file_id

	#===================
	if { 0>1 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    #set xdeb "*"
	    #set xfin "*"
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de 3% des limites pour l'esthetique
	    set largeur [ expr $lpart*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	#} elseif { [llength $args] == 4 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    set xdeb [ lindex $args 2 ]
	    set xfin [ lindex $args 3 ]
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	}
	#============================

	#--- D�termine le chemin de l'executable Gnuplot selon le syst�me d'exploitation :
	set repdflt [ bm_goodrep ]
	if { $tcl_platform(os)=="Linux" } {
	    set answer [ catch { exec gnuplot $audace(rep_images)/multiplot.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	} else {
	    set answer [ catch { exec $spcaudace(repgp)/gpwin32/pgnuplot.exe $audace(rep_images)/multiplot.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	}
	cd $repdflt

	#--- Effacement des fichiers de batch :
	file delete "$audace(rep_images)/multiplot.gp"
        foreach fichier $listedat {
	    file delete "$audace(rep_images)/$fichier"
        }
	::console::affiche_resultat "Profils de raie export� sous multiplot.png\n"
	return "multiplot.png"
    } else {
	::console::affiche_erreur "Usage: spc_multifit2png fichier_fits1 fichier_fits2 ... fichier_fitsn\n\n"
    }
}
####################################################################



####################################################################
#  Procedure de conversion de fichier profil de raie calibr� .fit en .png evec pr�cision de la l�gende
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 03-01-2007
# Date modification : 03-01-2007
# Arguments : fichier .fit du profil de raie, titre, ?legende_x legende_y? ?xdeb xfin?
####################################################################

proc spc_fit2ps { args } {
    global audace spcaudace
    global conf
    global tcl_platform

    #-- 3%=0.03
    set lpart 0

    if { [llength $args] == 6 || [llength $args] == 4 || [llength $args] == 2 } {
	if { [llength $args] == 2 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    set legendey "Relative intensity"
	    set legendex "Wavelength (\305)"
	    #--- D�termination de xdeb et xfin :
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de "lpart" % des limites pour l'esthetique
	    set largeur [ expr $lpart*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	} elseif { [llength $args] == 4 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    set legendex [ lindex $args 2 ]
	    set legendey [ lindex $args 3 ]
	    #--- D�termination de xdeb et xfin :
	    buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de "lpart" % des limites pour l'esthetique
	    set largeur [ expr $lpart*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	} elseif { [llength $args] == 6 } {
	    set fichier [ file rootname [ lindex $args 0 ] ]
	    set titre [ lindex $args 1 ]
	    set legendex [ lindex $args 2 ]
	    set legendey [ lindex $args 3 ]
	    set xdeb [ lindex $args 4 ]
	    set xfin [ lindex $args 5 ]
	} else {
	    ::console::affiche_erreur "Usage: spc_fit2ps fichier_fits \"Titre\" ?legende_x lende_y? ?xdeb xfin?\n\n"
	}
       

	#-- spc_fits2dat renvoie un nom avec une extension : fichier.dat
	set fileout [ spc_fits2dat $fichier ]
	#-- Retire l'extension .fit du nom du fichier
	set spcfile [ file rootname $fichier ]
	
	#--- Cr��e le fichier script pour gnuplot :
	set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
	puts $file_id "call \"$spcaudace(repgp)/gp_ps.cfg\" \"$audace(rep_images)/${spcfile}$spcaudace(extdat)\" \"$titre\" * * $xdeb $xfin * \"$audace(rep_images)/${spcfile}.ps\" \"$legendex\" \"$legendey\" "
	close $file_id

	#--- D�termine le chemin de l'executable Gnuplot selon le syst�me d'exploitation :
	if { $tcl_platform(os)=="Linux" } {
	    set answer [ catch { exec gnuplot $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	} else {
	    set answer [ catch { exec $spcaudace(repgp)/gpwin32/pgnuplot.exe $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "gnuplot r�sultat : $answer\n"
	}

	#--- Effacement des fichiers de batch :
	file delete "$audace(rep_images)/${spcfile}$spcaudace(extdat)"
	file delete "$audace(rep_images)/${spcfile}.gp"

	#--- Fin du script :
	::console::affiche_resultat "Profil de raie export� sous ${spcfile}.ps\n"
	return "${spcfile}.ps"
    } else {
	::console::affiche_erreur "Usage: spc_fit2ps fichier_fits \"Titre\" ?legende_x lende_y? ?xdeb xfin?\n\n"
    }
}
####################################################################




####################################################################
#  Procedure de cr�ation du fichier batch pour gnuplot afin de convertir un fichier profil de raie calibr� .fit en .png
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 03-09-2005
# Date modification : 03-09-2005
# Arguments : fichier .fit du profil de raie, titre, ?xdeb, xfin?
####################################################################

proc spc_fit2pngbat { args } {
   global audace spcaudace
   global conf
   set ecart 0.005

   if { [llength $args] == 4 || [llength $args] == 2 } {
       if { [llength $args] == 2 } {
	   set fichier [ lindex $args 0 ]
	   set titre [ lindex $args 1 ]
	   #set xdeb "*"
	   #set xfin "*"
	   buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	   set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	   #-- Demarre et fini le graphe en de�ca de 3% des limites pour l'esthetique
	   set largeur [ expr $ecart*$naxis1 ]
	   set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	   set xdeb [ expr $xdeb0+$largeur ]
	   set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	   #set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	   set xfin [ expr $naxis1*$xincr+$xdeb0-1*$largeur ]
       } elseif { [llength $args] == 4 } {
	   set fichier [ lindex $args 0 ]
	   set titre [ lindex $args 1 ]
	   set xdeb [ lindex $args 2 ]
	   set xfin [ lindex $args 3 ]
	   buf$audace(bufNo) load "$audace(rep_images)/$fichier"
	   set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
       }

       #--- Adapte la l�gende de l'abscisse
       if { $xdeb0 == 1.0 } {
	   set legendex "Position (Pixel)"
       } else {
	   set legendex "Wavelength (A)"
       }
       set legendey "Intensity (ADU)"

       set repertoire_gp [ file join $audace(rep_scripts) spcaudace gp ]
       set ext ".dat"

       spc_fits2dat $fichier
       # Retire l'extension .fit du nom du fichier
       set spcfile [ file rootname $fichier ]
       #exec echo "call \"$spcaudace(repgp)/gpx11.cfg\" \"${spcfile}$ext\" \"$titre\" * * $xdeb $xfin $pas \"${spcfile}.png\" \"$legendex\" \"$legendey\" " > $repertoire_gp/run_gp
       set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
       puts $file_id "call \"$spcaudace(repgp)/gp_visu.cfg\" \"${spcfile}$ext\" \"$titre\" * * $xdeb $xfin * \"${spcfile}.png\" \"$legendex\" \"$legendey\" "
       close $file_id
       set file_id [open "$audace(rep_images)/trace_gp.bat" w+]
       puts $file_id "gnuplot \"${spcfile}.gp\" "
       close $file_id
       # exec gnuplot $repertoire_gp/run_gp
       ::console::affiche_resultat "Ex�cuter dans un terminal : trace_gp.bat\n"
   } else {
       ::console::affiche_erreur "Usage: spc_fit2pngbat fichier_fits \"Titre\" ?xd�but xfin?\n\n"
   }
}
####################################################################



####################################################################
#  Procedure de conversion de fichier profil de raie calibr� .dat en .png
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 27-04-2006
# Date modification : 27-04-2006
# Arguments : fichier .dat du profil de raie, titre, ?xdeb, xfin?
####################################################################

proc spc_dat2png { args } {
    global audace spcaudace
    global conf
    global tcl_platform

    if { [llength $args] == 4 || [llength $args] == 2 } {
	if { [llength $args] == 2 } {
	    set fichier [ lindex $args 0 ]
	    set titre [ lindex $args 1 ]
	    #set xdeb "*"
	    #set xfin "*"
	    set fichierfit [ spc_dat2fits $fichier ${fichier}_fittmp ]
	    buf$audace(bufNo) load "$audace(rep_images)/$fichierfit"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de 3% des limites pour l'esthetique
	    set largeur [ expr 0.03*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	} elseif { [llength $args] == 4 } {
	    set fichier [ lindex $args 0 ]
	    set titre [ lindex $args 1 ]
	    set xdeb [ lindex $args 2 ]
	    set xfin [ lindex $args 3 ]
	    set fichierfit [ spc_dat2fits $fichier ${fichier}_fittmp ]
	    buf$audace(bufNo) load "$audace(rep_images)/$fichierfit"
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	}

	#--- Adapte la l�gende de l'abscisse
	if { $xdeb0 == 1.0 } {
	    set legendex "Position (Pixel)"
	} else {
	    set legendex "Wavelength (A)"
	}
	set legendey "Intensity (ADU)"
	
	#spc_fits2dat $fichier
	#-- Retire l'extension .fit du nom du fichier
	set spcfile [ file rootname $fichier ]
	
	#--- Cr��e le fichier script pour gnuplot :
	set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
	puts $file_id "call \"$spcaudace(repgp)/gp_novisu.cfg\" \"$audace(rep_images)/${spcfile}$spcaudace(extdat)\" \"$titre\" * * $xdeb $xfin * \"$audace(rep_images)/${spcfile}.png\" \"$legendex\" \"$legendey\" "
	close $file_id

	#--- D�termine le chemin de l'executable Gnuplot selon le syst�me d'exploitation :
	if { $tcl_platform(os)=="Linux" } {
	    set answer [ catch { exec gnuplot $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	} else {
	    set answer [ catch { exec $spcaudace(repgp)/gpwin32/pgnuplot.exe $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	}

	::console::affiche_resultat "Profil de raie export� sous ${spcfile}.png\n"
	return ${spcfile}.png
    } else {
	::console::affiche_erreur "Usage: spc_dat2png fichier_fits \"Titre\" ?xd�but xfin?\n\n"
    }
}
####################################################################



####################################################################
#  Procedure de conversion de fichier profil de raie .dat en .png
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 27-04-2006
# Date modification : 26-04-2006
# Arguments : fichier .dat du profil de raie, l�gende axe X, l�gende axe Y, pas
####################################################################

proc spc_dat2pngman { args } {
    global audace spcaudace
    global conf

    if { [llength $args] == 5 } {
	set fichier [ lindex $args 0 ]
	set titre [ lindex $args 1 ]
	set legendex [ lindex $args 2 ]
	set legendey [ lindex $args 3 ]
	set pas [ lindex $args 4 ]

	# Retire l'extension .fit du nom du fichier
	set spcfile [ file rootname $fichier ]

	#--- Prepare le script pour gnuplot
	set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
	put $file_id "call \"$spcaudace(repgp)/gp_spc.cfg\" \"${spcfile}$spcaudace(extdat)\" \"$titre\" * * * * $pas \"${spcfile}.png\" \"$legendex\" \"$legendey\" "
	close $file_id

	#--- Execute Gnuplot pour l'export en png
	if { $tcl_platform(os)=="Linux" } {
	    set answer [ catch { exec gnuplot $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	} else {
	    set answer [ catch { exec $spcaudace(repgp)/gpwin32/pgnuplot.exe $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	}
	::console::affiche_resultat "Profil de raie export� sous ${spcfile}.png\n"
	return ${spcfile}.png
   } else {
       ::console::affiche_erreur "Usage: spc_dat2pngman fichier_fits \"titre\" \"l�gende_axeX\" \"l�gende_axeY\" intervalle_graduations\n\n"
   }
}
####################################################################




###################################################################
#  Procedure de conversion de fichier profil de raies .spc (VisualSpec) en .fit
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 09-12-2005
# Date modification : 09-12-2005
# Arguments : fichier .spc du profil de raie
###################################################################
proc spc_spc2fits { args } {

 global conf
 global audace
 #global profilspc
 global captionspc
 global colorspc

#    set profilspc(xunit) "Position"
#    set profilspc(yunit) "ADU"

 if {[llength $args] == 1} {
   set filenamespc [ lindex $args 0 ]
   ## === Lecture du fichier de donnees du profil de raie ===
   catch {unset profilspc} {}
   set profilspc(initialdir) [file dirname $audace(rep_images)]
   set profilspc(initialfile) [file tail $filenamespc]
   set input [open "$audace(rep_images)/$filenamespc" r]

   #--- Charge le contenu du fichier et enleve l'ent�te
   #-- Retourne une cha�ne
   set contents [split [read $input] \n]
   close $input
   set profilspc(naxis1) [expr [lindex $contents 2]]
   set profilspc(exptime) [expr [lindex $contents 2]]
   set dateobs [lindex $contents 4]
   set profilspc(object) [lindex $contents 7]
   #set offset [expr [lindex $contents 1]+3]
   set offset [expr [lindex $contents 1]+15]

   ## === Extraction des numeros des pixels et des intensites ===
   for {set k 1} {$k <= $profilspc(naxis1)} {incr k} {
      set ligne [lindex $contents $offset]
      append profilspc(pixels) "[lindex $ligne 1] "
      #append profilspc(lambda) "[lindex $ligne 1] "
      append profilspc(intensite) "[lindex $ligne 2] "
      #append profilspc(argon1) "[lindex $ligne 3] "
      #append profilspc(argon2) "[lindex $ligne 4] "
      #append profilspc(noir) "[lindex $ligne 5] "
      #append profilspc(repere) "[lindex $ligne 6] "
      incr offset
   }
   #::console::affiche_resultat "ICI :\n $contents\n"
   #::console::affiche_resultat "ICI :\n $profilspc(pixels)\n"
   #::console::affiche_resultat "ICI :\n $profilspc(intensite)\n"

   # === On prepare les vecteurs a afficher ===
   # len : longueur du profil (NAXIS1)
   set len $profilspc(naxis1)

   # Initialisation � blanc d'un fichier fits
   buf$audace(bufNo) setpixels CLASS_GRAY $len 1 FORMAT_FLOAT COMPRESS_NONE 0
   buf$audace(bufNo) setkwd [ list NAXIS 1 int "" "" ]
   buf$audace(bufNo) setkwd [ list NAXIS1 $len int "" "" ]

   set intensite 0
   #--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
   # Une liste commence � 0 ; Un vecteur fits commence � 1
   for {set k 0} {$k<$len} {incr k} {
       append intensite [lindex $profilspc(intensite) $k]
       #::console::affiche_resultat "$intensite\n"
       #--- V�rifie que l'intensit� est bien un nombre
       if { [regexp {([0-9]+\.*[0-9]*)} $intensite match mintensite] } {
	   buf$audace(bufNo) setpix [list [expr $k+1] 1] $mintensite
	   set intensite 0
       }
   }

   #------- Affecte une valeur aux mots cle li�s � la spectroscopie ----------
   #- Modele :  buf$audace(bufNo) setkwd [list "INSTRU" "T310" string "mon t�lescope" ""]
   buf$audace(bufNo) setkwd [list "NAXIS1" $len int "" ""]
   buf$audace(bufNo) setkwd [list "NAXIS2" 1 int "" ""]
   buf$audace(bufNo) setkwd [list "OBJNAME" "$profilspc(object)" string "" ""]
   buf$audace(bufNo) setkwd [list "DATE-OBS" "$dateobs" string "" ""]
   #- ::console::affiche_resultat "mot : $dateobs\n"
   #--- Valeur minimale de l'abscisse (xdepart) : =0 si profil non �talonn�, DOUBLE ?
   set xdepart [expr 1.0*[lindex $profilspc(pixels) 0] ]
   buf$audace(bufNo) setkwd [list "CRVAL1" $xdepart float "" "Angstrom"]
   #--- Valeur de la longueur d'onde/pixel central(e)
   set xdernier [lindex $profilspc(pixels) [expr $profilspc(naxis1)-1]]
   ::console::affiche_resultat "Xdep : $xdepart ; Xfin : $xdernier\n"
   #set xcentre [expr int(0.5*($xdernier-$xdepart))]
   #buf$audace(bufNo) setkwd [list "CRPIX1" "$xcentre" double "" "Angstrom"]
   #--- Dispersion du spectre :
   set l1 [lindex $profilspc(pixels) 1]
   set l2 [lindex $profilspc(pixels) [expr int($len/10)]]
   set l3 [lindex $profilspc(pixels) [expr int(2*$len/10)]]
   set l4 [lindex $profilspc(pixels) [expr int(3*$len/10)]]
   set dl1 [expr ($l2-$l1)/(int($len/10)-1)]
   set dl2 [expr ($l4-$l3)/(int($len/10)-1)]
   set xincr [expr 0.5*($dl2+$dl1)]
   #--- Meth2 : erreur si spectre de moins de 4 pixels
   #set l2 [lindex $profilspc(pixels) 4]
   #set l3 [lindex $profilspc(pixels) [expr 1+int($len/2)]]
   #set l4 [lindex $profilspc(pixels) [expr 4+int($len/2)]]
   #set dl1 [expr ($l2-$l1)/3]
   #set dl2 [expr ($l4-$l3)/3]
   #set xincr [expr 0.5*($dl2+$dl1)]

   ::console::affiche_resultat "Dispersion : $xincr\n"
   buf$audace(bufNo) setkwd [list "CDELT1" $xincr float "" "Angstrom/pixel"]
   #--- Type de dispersion : LINEAR, NONLINEAR --> heuristique hasardeuse
   #if { [expr abs($dl2-$dl1)] <= 0.001 } {
   #    buf$audace(bufNo) setkwd [list "CTYPE1" "LINEAR" string "" ""]
   #} elseif { [expr abs($dl2-$dl1)] > 0.001 } {
   #    buf$audace(bufNo) setkwd [list "CTYPE1" "NONLINEAR" string "" ""]
   #}

   #--- Sauve le fichier fits ainsi constitu� 
   buf$audace(bufNo) bitpix float
   set filename [file root $filenamespc]
   buf$audace(bufNo) save "$audace(rep_images)/${filename}$conf(extension,defaut)"
   ::console::affiche_resultat "$len lignes affect�es\n"
   ::console::affiche_resultat "Fichier spc export� sous ${filename}$conf(extension,defaut)\n"
   return $filename
 } else {
   ::console::affiche_erreur "Usage: spc_spc2fits fichier_spc\n\n"
 }
}
#****************************************************************#


proc spc_spc2fits2 { args } {

 global conf
 global audace spcaudace
 global profilspc
 global captionspc
 global colorspc


 if {[llength $args] == 1} {
   set filenamespc [ lindex $args 0 ]
   ## === Lecture du fichier de donnees du profil de raie ===
   catch {unset profilspc} {}
   set profilspc(initialdir) [file dirname $audace(rep_images)]
   set profilspc(initialfile) [file tail $filenamespc]
   set input [open "$audace(rep_images)/$filenamespc" r]

   #--- Charge le contenu du fichier et enleve l'ent�te
   #-- Retourne une cha�ne
   set total_contents [read $input]
   set contents [regexp {(.+)repere\r\n(.+)$} $total_contents match]
   set liste_lignes [split $contents \n]
   close $input
   #::console::affiche_resultat "ICI :\n $contents\n"
   ## === Extraction des numeros des pixels et des intensites ===
   ##set profilspc(naxis1) [expr [llength $contents]-2]
   set profilspc(naxis1) [ expr [llength $liste_lignes]-1]
   #::console::affiche_resultat "$profilspc(naxis1)\n"
   set offset 1
   #--- Une liste commence � 0
   for {set k -1} {$k < $profilspc(naxis1)} {incr k} {
      set ligne [lindex $liste_lignes $k]
      append profilspc(pixels) "[lindex $ligne 1] "
      append profilspc(intensite) "[lindex $ligne 2] "
      #incr $k
   }
   ::console::affiche_resultat "$profilspc(pixels)\n"
   #::console::affiche_resultat "ICI :\n $profilspc(intensite)\n"

   # === On prepare les vecteurs a afficher ===
   # len : longueur du profil (NAXIS1)
   #set len [llength $profilspc(pixels)]
   set len $profilspc(naxis1)
   set intensites ""
   for {set k 0} {$k<=$len} {incr k} {
         append intensites " [lindex $profilspc(intensite) $k]"
   }

   # Initialisation � blanc d'un fichier fits
   #buf$audace(bufNo) format $len 1
   buf$audace(bufNo) setpixels CLASS_GRAY $len 1 FORMAT_FLOAT COMPRESS_NONE 0
   buf$audace(bufNo) setkwd [ list NAXIS 1 int "" "" ]
   buf$audace(bufNo) setkwd [ list NAXIS1 $len int "" "" ]

   set intensite 0
   #--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
   # Une liste commence � 0 ; Un vecteur fits commence � 1
   for {set k 0} {$k<$len} {incr k} {
       append intensite [lindex $intensites $k]
       #::console::affiche_resultat "$intensite\n"
       buf$audace(bufNo) setpix [list [expr $k+1] 1] $intensite
       set intensite 0
   }

   set profilspc(xunit) "Position"
   set profilspc(yunit) "ADU"
   #------- Affecte une valeur aux mots cle li�s � la spectroscopie ----------
   # buf$audace(bufNo) setkwd [list "INSTRU" "T310" string "mon t�lescope" ""]
   buf$audace(bufNo) setkwd [list "NAXIS1" "$len" int "" ""]
   buf$audace(bufNo) setkwd [list "NAXIS2" "1" int "" ""]
   # Valeur minimale de l'abscisse (xdepart) : =0 si profil non �talonn�
   set xdepart [ expr 1.0*[lindex $profilspc(pixels) 0] ]
   buf$audace(bufNo) setkwd [list "CRVAL1" $xdepart float "" ""]
   # Valeur de la longueur d'onde/pixel central(e)
   set xdernier [lindex $profilspc(pixels) [expr $profilspc(naxis1)-1]]
   ::console::affiche_resultat "Xdep : $xdepart ; Xfin : $xdernier\n"
   #set xcentre [expr int(0.5*($xdernier-$xdepart))]
   #buf$audace(bufNo) setkwd [list "CRPIX1" "$xcentre" int "" ""]
   # Dispersion du spectre : =1 si profil non �talonn�
   set l1 [lindex $profilspc(pixels) 1]
   set l2 [lindex $profilspc(pixels) [expr int($len/10)]]
   set l3 [lindex $profilspc(pixels) [expr int(2*$len/10)]]
   set l4 [lindex $profilspc(pixels) [expr int(3*$len/10)]]
   set dl1 [expr ($l2-$l1)/(int($len/10)-1)]
   set dl2 [expr ($l4-$l3)/(int($len/10)-1)]
   set xincr [expr 0.5*($dl2+$dl1)]
   # Meth2 : erreur si spectre de moins de 4 pixels
   #set l2 [lindex $profilspc(pixels) 4]
   #set l3 [lindex $profilspc(pixels) [expr 1+int($len/2)]]
   #set l4 [lindex $profilspc(pixels) [expr 4+int($len/2)]]
   #set dl1 [expr ($l2-$l1)/3]
   #set dl2 [expr ($l4-$l3)/3]
   #set xincr [expr 0.5*($dl2+$dl1)]

   ::console::affiche_resultat "Dispersion : $xincr\n"
   buf$audace(bufNo) setkwd [list "CDELT1" $xincr float "" ""]
   # Type de dispersion : LINEAR, NONLINEAR
   #if { [expr abs($dl2-$dl1)] <= 0.001 } {
   #    buf$audace(bufNo) setkwd [list "CTYPE1" "LINEAR" string "" ""]
   #} elseif { [expr abs($dl2-$dl1)] > 0.001 } {
   #    buf$audace(bufNo) setkwd [list "CTYPE1" "NONLINEAR" string "" ""]
   #}

   # Sauve le fichier fits ainsi constitu� 
   buf$audace(bufNo) bitpix float
   buf$audace(bufNo) save "$audace(rep_images)/${filenamespc}_fit$conf(extension,defaut)"
   ::console::affiche_resultat "$len lignes affect�es\n"
   ::console::affiche_resultat "Fichier spc export� sous ${filenamespc}_fit$conf(extension,defaut)\n"
   return ${filenamespc}_fit
 } else {
   ::console::affiche_erreur "Usage: spc_spc2fits fichier_spc\n\n"
 }
}
###########################################################################


###################################################################
#  Procedure de conversion d'une s�rie defichiers profil de raies .spc (VisualSpec) en .fit
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 09-12-2005
# Date modification : 02-04-2006
# Arguments : r�pertoire contenant les fichiers .spc
###################################################################
proc spc_spcs2fits { args } {

    global conf
    global audace spcaudace


    if {[llength $args] == 1} {
	set repertoire [ lindex $args 0 ]
	set rep_img_dflt $audace(rep_images)
	set rep_courant [ pwd ]
	set audace(rep_images) $repertoire
	cd $repertoire
	set liste_fichiers [ lsort -dictionary [ glob *$spcaudace(extvspec) ] ]
	foreach fichier $liste_fichiers {
	    ::console::affiche_resultat "$fichier\n"
	    spc_spc2fits $fichier
	    ::console::affiche_resultat "\n"
	}
	set audace(rep_images) $rep_img_dflt
	cd $rep_courant
    } else {
	::console::affiche_erreur "Usage: spc_spcs2fits chemin_du_r�pertoire\n\n"
    }
}
###########################################################################


###################################################################
# Procedure de conversion d'une s�rie de fichiers profil de raies .dat en .fit
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 02-04-2006
# Date modification : 02-04-2006
# Arguments : r�pertoire contenant les fichiers .dat
###################################################################
proc spc_dats2fits { args } {

    global conf
    global audace spcaudace

    if {[llength $args] == 1} {
	set repertoire [ lindex $args 0 ]
	set rep_img_dflt $audace(rep_images)
	set rep_courant [ pwd ]
	set audace(rep_images) $repertoire
	cd $repertoire
	set liste_fichiers [ lsort -dictionary [ glob *$spcaudace(extdat) ] ]
	foreach fichier $liste_fichiers {
	    ::console::affiche_resultat "$fichier\n"
	    spc_dat2fits $fichier
	    ::console::affiche_resultat "\n"
	}
	set audace(rep_images) $rep_img_dflt
	cd $rep_courant
    } else {
	::console::affiche_erreur "Usage: spc_dats2fits chemin_du_r�pertoire\n\n"
    }
}
#**********************************************************************************#



###################################################################
# Lecture des fichiers contenant le nom et la longueur d'onde d'especes chimiques
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 17-09-2006
# Date modification : 17-09-2006
# Arguments : aucun
###################################################################
proc spc_readchemfiles { args } {

    global conf
    global audace spcaudace
    set extdata ".txt"
    set fileelements "stellar_lines.txt"
    set fileneon "neon.txt"
    set fileeau "h2o.txt"

    if { [ llength $args ] <= 1 } {
	if { [ llength $args ] == 1 } {
	    set repertoire [ lindex $args 0 ]
	} elseif { [ llength $args ] == 0 } {
	    set repertoire ""
	} else {
	    ::console::affiche_erreur "Usage: spc_readchemfiles ?r�pertoire des catalogues chimiques?\n\n"
	}

	#--- Lecture du fichier des raies stellaires
	set input [open "$spcaudace(repchimie)/$fileelements" r]
	set contents [split [read $input] \n]
	close $input
	foreach ligne $contents {
	    set element [ lindex $ligne 0 ]
	    set lambda [ lindex $ligne 1 ]
	    append listelambdaschem "$element:$lambda "
	    # lappend listelambdaschem "$ligne"
	}

	#--- Lecture du fichier des raies de l'eau
	set input [open "$spcaudace(repchimie)/$fileeau" r]
	set contents [split [read $input] \n]
	close $input
	foreach ligne $contents {
	    set element [ lindex $ligne 0 ]
	    set lambda [ lindex $ligne 1 ]
	    append listelambdaschem "$element:$lambda "
	    # lappend listelambdaschem "$ligne"
	}

	#--- Lecture du fichier des raies du neon
	set input [open "$spcaudace(repchimie)/$fileneon" r]
	set contents [split [read $input] \n]
	close $input
	foreach ligne $contents {
	    set element [ lindex $ligne 0 ]
	    set lambda [ lindex $ligne 1 ]
	    append listelambdaschem "$element:$lambda "
	    # lappend listelambdaschem "$ligne"
	}

	#--- Fin du script :
	set listelambdaschem [ split $listelambdaschem " " ]
	return $listelambdaschem
    } else {
	::console::affiche_erreur "Usage: spc_readchemfiles ?r�pertoire des catalogues chimiques?\n\n"
    }
}
#**********************************************************************************#




###################################################################
# Lecture des fichiers contenant le nom et la longueur d'onde d'especes chimiques
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 3-01-2007
# Date modification : 3-01-2007
# Arguments : fichier_config_mat�rielle.txt profil_de_raies_�_tracer \"Nom objet\" ?xdeb xfin?
###################################################################

proc spc_autofit2png1 { args } {

    global conf
    global audace

    set labelx "Wavelength (A)"
    set labely "Relative intensity"

    if { [ llength $args ] <= 5 } {
	if { [ llength $args ] == 3 } {
	    set fichier_config [ lindex $args 0 ]
	    set spectre [ file rootname [ lindex $args 1 ] ]
	    set nom_objet [ lindex $args 2 ]
	    set xdeb "*"
	    set xfin "*"
	} elseif { [ llength $args ] == 5 } {
	    set fichier_config [ lindex $args 0 ]
	    set spectre [ file rootname [ lindex $args 1 ] ]
	    set nom_objet [ lindex $args 2 ]
	    set xdeb [ lindex $args 3 ]
	    set xfin [ lindex $args 4 ]
	} else {
	    ::console::affiche_erreur "Usage: spc_autofit2png fichier_config_mat�rielle.txt profil_de_raies_�_tracer \"Nom objet\" ?xdeb xfin?\n\n"
	    return 0
	}
 
	#--- Lecture du fichier de confiration mat�rielle :
	if { [ file exists "$audace(rep_images)/$fichier_config" ] } {
	    set input [open "$audace(rep_images)/$fichier_config" r]
	    set contents [split [read $input] \n]
	    close $input
	    #::console::affiche_resultat "Contenu : $contents\n"
	    #set contents [ split $contentsf " " ]
	    #::console::affiche_resultat "Contenu : $contents\n"
	    #-- ligne 1 : telescope
	    set telescope [ lindex $contents 0 ]
	    #-- ligne 2 : spectroscope
	    set spectroscope [ lindex $contents 1 ]
	    #-- ligne 3 : r�seau utilis�
	    set reseau [ lindex $contents 2 ]
	    #-- Une boucle pour lire toutes les lignes :
	    #foreach ligne $contents {
	    #    set telescope [ lindex $ligne 0 ]
	    #    set lambda [ lindex $ligne 1 ]
	    #    append listelambdaschem "$element:$lambda "
	    #    # lappend listelambdaschem "$ligne"
	    #}
	} else {
	    ::console::affiche_resultat "Le fichier $fichier_config n'existe pas. Utilisation des valeurs par d�faut.\n"
	    set telescope  "T�lescope"
	    set spectroscope "Spectroscope"
	    set reseau "reseau"
	}

	#--- Liste les mots de l'ent�te fits :
	buf$audace(bufNo) load "$audace(rep_images)/$spectre"
	set listemotsclef [ buf$audace(bufNo) getkwds ]

	#--- D�termination de la date de prise de vue :
	if { [ lsearch $listemotsclef "DATE-OBS" ] !=-1 } {
	    set ladate [ bm_datefrac $spectre ]
	}

	#--- D�termination des param�tres d'exposition :
	if { [ lsearch $listemotsclef "EXPOSURE" ] !=-1 } {
	    set duree_totale [ expr round([ lindex [ buf$audace(bufNo) getkwd "EXPOSURE" ] 1 ]) ]
	}
	#-- Recherche du nombre d'occurence du nombre "99" dans un des mots clefs TT :
	set nombre_poses 0
	foreach mot $listemotsclef {
	    set valeur_mot [ lindex [ buf$audace(bufNo) getkwd "$mot" ] 1 ]
	    if { [ regexp {\s99\s} $valeur_mot match resul ] } {
		set nombre_poses [ llength $valeur_mot ]
	    }
	}

	#--- R�cup�ration de la dispersion :
	if { [ lsearch $listemotsclef "CDELT1" ] !=-1 && [ lsearch $listemotsclef "CRVAL1" ] !=-1 } {
	    if { [ llength $args ] == 3 } {
		set xdeb [  lindex [ buf$audace(bufNo) getkwd "CRVAL1" ] 1 ]
	    }
	    if { $xdeb != 1. } {
		set dispersion_precise [ lindex [ buf$audace(bufNo) getkwd "CDELT1" ] 1 ]
		set dispersion [ expr round($dispersion_precise*1000.)/1000. ]
	    } else {
		set dispersion 0
	    }
	} else {
	    set dispersion 0
	}

	#--- Suppression des accents dans les variables :
	set nom_objet [ suppr_accents $nom_objet ]
	set telescope [ suppr_accents $telescope ]
	set spectroscope [ suppr_accents $spectroscope ]


	#--- �laboration du titre du graphique :
	if { $dispersion == 0.0 } {
	    if { $nombre_poses == 0 } {
		set titre_graphique "$nom_objet - $ladate - $telescope + $spectroscope $reseau - $duree_totale s"
	    } else {
		set duree_exposition [ expr int($duree_totale/$nombre_poses) ]
		set titre_graphique "$nom_objet - $ladate - $telescope + $spectroscope $reseau - ${nombre_poses}x$duree_exposition s"
	    }
	} else {
	    if { $nombre_poses == 0 } {
		set titre_graphique "$nom_objet - $ladate - $telescope + $spectroscope $reseau - $dispersion A/pixel - $duree_totale s"
	    } else {
		set duree_exposition [ expr int($duree_totale/$nombre_poses) ]
		set titre_graphique "$nom_objet - $ladate - $telescope + $spectroscope $reseau - $dispersion A/pixel - ${nombre_poses}x$duree_exposition s"
	    }
	}

	#--- Trac� du graphique :
	set fileout [ spc_fit2pngleg "$spectre" "$titre_graphique" "$labelx" "$labely" $xdeb $xfin ]

	#--- Fabrication de la date du fichier :
	set datefile [ bm_datefile $spectre ]

	#--- Fabrication du nom de fichier graphique (pas d'espace...) :
	set nom_objet_lower [ string tolower "$nom_objet" ]
	if { [ regsub {(\s)} "$nom_objet_lower" "_" resul ] } {
	    set nom_sans_espaces "$resul"
	    if { [ regsub {(\s)} "$nom_sans_espaces" "_" resul ] } {
		set nom_sans_espaces "$resul"
		if { [ regsub {(\s)} "$nom_sans_espaces" "_" resul ] } {
		    set nom_sans_espaces "$resul"
		}
	    }
	} else {
	    set nom_sans_espaces "$nom_objet_lower"
	}
	if { [ regexp {.+(\.[a-zA-Z]{3})} "fileout" match extimg ] } {
	    file rename -force "$audace(rep_images)/$fileout" "$audace(rep_images)/${nom_sans_espaces}_$datefile$extimg"
	} else {
	    set extimg ".png"
	    file rename -force "$audace(rep_images)/$fileout" "$audace(rep_images)/${nom_sans_espaces}_$datefile$extimg"
	}

	#--- Production optionnelle d'un fichier Postscript :
	#set fileout2 [ spc_fit2ps "$spectre" "$titre_graphique" "Wavelength (\305)" "$labely" $xdeb $xfin ]
	#file rename -force "$audace(rep_images)/$fileout2" "$audace(rep_images)/${nom_sans_espaces}_${datefile}.ps"

	#--- Fin du script :
	file copy -force "$audace(rep_images)/$spectre$conf(extension,defaut)" "$audace(rep_images)/${nom_sans_espaces}_$datefile$conf(extension,defaut)"
	return "${nom_sans_espaces}_$datefile$extimg"
    } else {
	::console::affiche_erreur "Usage: spc_autofit2png1 fichier_config_mat�rielle.txt profil_de_raies_�_tracer \"Nom objet\" ?xdeb xfin?\n\n"
    }
}
#**********************************************************************************#



###################################################################
# Lecture des fichiers contenant le nom et la longueur d'onde d'especes chimiques
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 3-01-2007
# Date modification : 3-01-2007
# Arguments : fichier_config_mat�rielle.txt profil_de_raies_�_tracer \"Nom objet\" ?xdeb xfin?
###################################################################

proc spc_autofit2png { args } {

    global conf
    global audace

    set labelx "Wavelength (A)"
    set labely "Relative intensity"

    if { [ llength $args ] <= 4 } {
	if { [ llength $args ] == 2 } {
	    set spectre [ file rootname [ lindex $args 0 ] ]
	    set nom_objet [ lindex $args 1 ]
	    set xdeb "*"
	    set xfin "*"
	} elseif { [ llength $args ] == 4 } {
	    set spectre [ file rootname [ lindex $args 0 ] ]
	    set nom_objet [ lindex $args 1 ]
	    set xdeb [ lindex $args 2 ]
	    set xfin [ lindex $args 3 ]
	} else {
	    ::console::affiche_erreur "Usage: spc_autofit2png profil_de_raies_�_tracer \"Nom objet\" ?xdeb xfin?\n\n"
	    return 0
	}

	#--- Liste les mots de l'ent�te fits :
	buf$audace(bufNo) load "$audace(rep_images)/$spectre"
	set listemotsclef [ buf$audace(bufNo) getkwds ]


	#--- D�termination du t�lescope :
	if { [ lsearch $listemotsclef "TELESCOP" ] !=-1 } {
	    set telescope [ lindex [ buf$audace(bufNo) getkwd "TELESCOP" ] 1 ]
	} else {
	    set telescope  "T�lescope"
	}

	#--- D�termination de l'�quipement spectroscopique :
	if { [ lsearch $listemotsclef "EQUIPMEN" ] !=-1 } {
	    set equipement [ lindex [ buf$audace(bufNo) getkwd "EQUIPMEN" ] 1 ]
	} else {
	    set equipement  "Spectrescope reseau"
	}


	#--- D�termination de la date de prise de vue :
	if { [ lsearch $listemotsclef "DATE-OBS" ] !=-1 } {
	    set ladate [ bm_datefrac $spectre ]
	}

	#--- D�termination des param�tres d'exposition :
	if { [ lsearch $listemotsclef "EXPOSURE" ] !=-1 } {
	    set duree_totale [ expr round([ lindex [ buf$audace(bufNo) getkwd "EXPOSURE" ] 1 ]) ]
	}
	#-- Recherche du nombre d'occurence du nombre "99" dans un des mots clefs TT :
	set nombre_poses 0
	foreach mot $listemotsclef {
	    set valeur_mot [ lindex [ buf$audace(bufNo) getkwd "$mot" ] 1 ]
	    if { [ regexp {\s99\s} $valeur_mot match resul ] } {
		set nombre_poses [ llength $valeur_mot ]
	    }
	}

	#--- R�cup�ration de la dispersion :
	if { [ lsearch $listemotsclef "CDELT1" ] !=-1 && [ lsearch $listemotsclef "CRVAL1" ] !=-1 } {
	    if { [ llength $args ] == 3 } {
		set xdeb [  lindex [ buf$audace(bufNo) getkwd "CRVAL1" ] 1 ]
	    }
	    if { $xdeb != 1. } {
		set dispersion_precise [ lindex [ buf$audace(bufNo) getkwd "CDELT1" ] 1 ]
		set dispersion [ expr round($dispersion_precise*1000.)/1000. ]
	    } else {
		set dispersion 0
	    }
	} else {
	    set dispersion 0
	}

	#--- Suppression des accents dans les variables :
	set nom_objet [ suppr_accents $nom_objet ]
	set telescope [ suppr_accents $telescope ]
	set equipement [ suppr_accents $equipement ]


	#--- �laboration du titre du graphique :
	if { $dispersion == 0.0 } {
	    if { $nombre_poses == 0 } {
		set titre_graphique "$nom_objet - $ladate - $telescope + $equipement - $duree_totale s"
	    } else {
		set duree_exposition [ expr int($duree_totale/$nombre_poses) ]
		set titre_graphique "$nom_objet - $ladate - $telescope + $equipement - ${nombre_poses}x$duree_exposition s"
	    }
	} else {
	    if { $nombre_poses == 0 } {
		set titre_graphique "$nom_objet - $ladate - $telescope + $equipement - $dispersion A/pixel - $duree_totale s"
	    } else {
		set duree_exposition [ expr int($duree_totale/$nombre_poses) ]
		set titre_graphique "$nom_objet - $ladate - $telescope + $equipement - $dispersion A/pixel - ${nombre_poses}x$duree_exposition s"
	    }
	}

	#--- Trac� du graphique :
	set fileout [ spc_fit2pngleg "$spectre" "$titre_graphique" "$labelx" "$labely" $xdeb $xfin ]

	#--- Fabrication de la date du fichier :
	set datefile [ bm_datefile $spectre ]

	#--- Fabrication du nom de fichier graphique (pas d'espace...) :
	set nom_objet_lower [ string tolower "$nom_objet" ]
	if { [ regsub {(\s)} "$nom_objet_lower" "_" resul ] } {
	    set nom_sans_espaces "$resul"
	    if { [ regsub {(\s)} "$nom_sans_espaces" "_" resul ] } {
		set nom_sans_espaces "$resul"
		if { [ regsub {(\s)} "$nom_sans_espaces" "_" resul ] } {
		    set nom_sans_espaces "$resul"
		}
	    }
	} else {
	    set nom_sans_espaces "$nom_objet_lower"
	}
	if { [ regexp {.+(\.[a-zA-Z]{3})} "fileout" match extimg ] } {
	    file rename -force "$audace(rep_images)/$fileout" "$audace(rep_images)/${nom_sans_espaces}_$datefile$extimg"
	} else {
	    set extimg ".png"
	    file rename -force "$audace(rep_images)/$fileout" "$audace(rep_images)/${nom_sans_espaces}_$datefile$extimg"
	}

	#--- Production optionnelle d'un fichier Postscript :
	set fileout2 [ spc_fit2ps "$spectre" "$titre_graphique" "Wavelength (\305)" "$labely" $xdeb $xfin ]
	file rename -force "$audace(rep_images)/$fileout2" "$audace(rep_images)/${nom_sans_espaces}_${datefile}.ps"

	#--- Fin du script :
	file copy -force "$audace(rep_images)/$spectre$conf(extension,defaut)" "$audace(rep_images)/${nom_sans_espaces}_$datefile$conf(extension,defaut)"
	return "${nom_sans_espaces}_$datefile$extimg"
    } else {
	::console::affiche_erreur "Usage: spc_autofit2png profil_de_raies_�_tracer \"Nom objet\" ?xdeb xfin?\n\n"
    }
}
#**********************************************************************************#



















#==============================================================================#
#==============================================================================#
#"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""#
#  Ancienne impl�mentation des fonction 
#
#"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""#
#==============================================================================#
#==============================================================================#

####################################################################
#  Procedure de conversion de fichier profil de raie calibr� .dat en .png
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 27-04-2006
# Date modification : 27-04-2006
# Arguments : fichier .dat du profil de raie, titre, ?xdeb, xfin?
####################################################################

proc spc_dat2png_27042006 { args } {
    global audace
    global conf
    global tcl_platform

    if { [llength $args] == 4 || [llength $args] == 2 } {
	if { [llength $args] == 2 } {
	    set fichier [ lindex $args 0 ]
	    set titre [ lindex $args 1 ]
	    #set xdeb "*"
	    #set xfin "*"
	    set fichierfit [ spc_dat2fits $fichier ${fichier}_fittmp ]
	    buf$audace(bufNo) load "$audace(rep_images)/$fichierfit"
	    set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
	    #-- Demarre et fini le graphe en de�ca de 3% des limites pour l'esthetique
	    set largeur [ expr 0.03*$naxis1 ]
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	    set xdeb [ expr $xdeb0+$largeur ]
	    set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
	    set xfin [ expr $naxis1*$xincr+$xdeb-2*$largeur ]
	} elseif { [llength $args] == 4 } {
	    set fichier [ lindex $args 0 ]
	    set titre [ lindex $args 1 ]
	    set xdeb [ lindex $args 2 ]
	    set xfin [ lindex $args 3 ]
	    set fichierfit [ spc_dat2fits $fichier ${fichier}_fittmp ]
	    buf$audace(bufNo) load "$audace(rep_images)/$fichierfit"
	    set xdeb0 [ lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1 ]
	}

	#--- Adapte la l�gende de l'abscisse
	if { $xdeb0 == 1.0 } {
	    set legendex "Position (Pixel)"
	} else {
	    set legendex "Wavelength (A)"
	}
	set legendey "Intensity (ADU)"

	set repertoire_gp [ file join $audace(rep_scripts) spcaudace gp ]
	set ext ".dat"
	
	#spc_fits2dat $fichier
	#-- Retire l'extension .fit du nom du fichier
	set spcfile [ file rootname $fichier ]
	
	#--- Cr��e le fichier script pour gnuplot :
	set file_id [open "$audace(rep_images)/${spcfile}.gp" w+]
	puts $file_id "call \"$spcaudace(repgp)/gp_novisu.cfg\" \"$audace(rep_images)/${spcfile}$ext\" \"$titre\" * * $xdeb $xfin * \"$audace(rep_images)/${spcfile}.png\" \"$legendex\" \"$legendey\" "
	close $file_id

	#--- D�termine le chemin de l'executable Gnuplot selon le syst�me d'exploitation :
	if { $tcl_platform(os)=="Linux" } {
	    set answer [ catch { exec gnuplot $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	} else {
	    set answer [ catch { exec $spcaudace(repgp)/gpwin32/pgnuplot.exe $audace(rep_images)/${spcfile}.gp } ]
	    ::console::affiche_resultat "$answer\n"
	}

	::console::affiche_resultat "Profil de raie export� sous ${spcfile}.png\n"
    } else {
	::console::affiche_erreur "Usage: spc_dat2png fichier_fits \"Titre\" ?xd�but xfin?\n\n"
    }
}
####################################################################



####################################################################
#  Procedure de conversion de fichier profil de raie spatial .fit en .dat
#
# Auteur : Benjamin MAUCLAIRE
# Date creation : 29-01-2005
# Date modification : 29-01-2005
# Arguments : fichier .fit du profil de raie
####################################################################

proc spc_fits2dat0 { {filenamespc ""} } {

   global conf
   global audace
   global profilspc
   #global captionspc
   global colorspc
   set extsp ".dat"

   #buf$audace(bufNo) load "$audace(rep_images)/$filenamespc"
   buf$audace(bufNo) load $filenamespc
   set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
   # Valeur minimale de l'abscisse : =0 si profil non �talonn�
   set xdepart [lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1]
   # Dispersion du spectre : =1 si profil non �talonn�
   set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
   # Pixel de l'abscisse centrale
   set xcenter [lindex [buf$audace(bufNo) getkwd "CRPIX1"] 1]
   # Type de spectre : LINEAR ou NONLINEAR (elinine les espaces dans la valeur du mot cle.
   #set dtype [string trim [lindex [buf$audace(bufNo) getkwd "CTYPE1"] 1]]
   #if { $dtype != "LINEAR" || $dtype == "" } {
   #    ::console::affiche_resultat "Le spectre ne poss�de pas une disersion lin�aire. Pas de conversion possible.\n"
   #    break
   #}
   set len [expr int($naxis1/$xincr)]
   ::console::affiche_resultat "$len intensit�s � traiter\n"

   if { $xdepart != 1 } {
       set fileetalonnespc [ file rootname $filenamespc ]
       #set filename ${fileetalonnespc}_dat$extsp
       set filename ${fileetalonnespc}$extsp
       set file_id [open "$audace(rep_images)/$filename" w+]

   ## Une liste commence � 0 ; Un vecteur fits commence � 1
   #for {set k 0} {$k<$len} {incr k} 
   #	append intensite [lindex $intensites $k]
	#::console::affiche_resultat "$intensite\n"
	# buf$audace(bufNo) setpix [list [expr $k+1] 1] $intensite
	# set intensite 0
   #

       for {set k 1} {$k<=$len} {incr k} {
	   # Donne les bonnes valeurs aux abscisses si le spectre est �talonn� en longueur d'onde
	   set lambda [expr int($xdepart+($k-1)*$xincr)]
	   ##lappend profilspc(pixels) $lambda
	   #set lambda 0

	   # Lit la valeur des elements du fichier fit
	   set intensite [buf$audace(bufNo) getpix [list $k 1]]
	   ##lappend profilspc(intensite) $intensite
	   #set intensite 0

	   # Ecrit les couples "Lambda Intensite" dans le fichier de sortie
	   ::console::affiche_resultat "$lambda\t$intensite\n"
	   puts $file_id "$lambda\t$intensite"
       }
       close $file_id
   } else {
       # Retire l'extension .dat du nom du fichier
       set filespacialspc [ file rootname $filenamespc ]
       #buf$audace(bufNo) imaseries "PROFILE filename=${filespacialspc}_dat$extsp direction=x offset=1"
       buf$audace(bufNo) imaseries "PROFILE filename=${filespacialspc}$extsp direction=x offset=1"
       ::console::affiche_resultat "Fichier fits export� sous $profilspc(initialfile)$extsp\n"
   }
   
}
#****************************************************************#



###################################################################
#  Procedure de conversion de fichier profil de raies .dat en .fit
# 
# Auteur : Benjamin MAUCLAIRE
# Date creation : 31-01-2005
# Date modification : 15-02-2005/27-04-2006
# Arguments : fichier .dat du profil de raie ?fichier_sortie.fit?
###################################################################

proc spc_dat2fits_150205 { args } {

 global conf
 global audace
 global profilspc
 #global captionspc
 global colorspc
 set extsp ".dat"
 set nbunit "float"
 #set nbunit "double"
 set precision 0.05

 if { [llength $args] <= 2 } {
     if {[llength $args] == 1} {
	 set filenamespc [ lindex $args 0 ]
     } elseif { [llength $args] == 2 } {
	 set filenamespc [ lindex $args 0 ]
	 set filenameout [ lindex $args 1 ]
     }
     ## === Lecture du fichier de donnees du profil de raie ===
     catch {unset profilspc} {}
     set profilspc(initialdir) [file dirname $audace(rep_images)]
     set profilspc(initialfile) [file tail $filenamespc]
     set input [open "$audace(rep_images)/$filenamespc" r]
     set contents [split [read $input] \n]
     close $input
     
     ## === Extraction des numeros des pixels et des intensites ===
     #::console::affiche_resultat "ICI :\n $contents.\n"
     #set profilspc(naxis1) [expr [llength $contents]-2]
     set profilspc(naxis1) [ expr [llength $contents]-1]
     #::console::affiche_resultat "$profilspc(naxis1)\n"
     set offset 1
     # Une liste commence � 0
     for {set k -1} {$k < $profilspc(naxis1)} {incr k} {
	 set ligne [lindex $contents $k]
	 append profilspc(pixels) "[lindex $ligne 0] "
	 append profilspc(intensite) "[lindex $ligne 1] "
	 #incr $k
     }
     #::console::affiche_resultat "$profilspc(pixels)\n"
     
     # === On prepare les vecteurs a afficher ===
     # len : longueur du profil (NAXIS1)
     #set len [llength $profilspc(pixels)]
     set len $profilspc(naxis1)
     set intensites ""
     for {set k 0} {$k<=$len} {incr k} {
         append intensites " [lindex $profilspc(intensite) $k]"
     }
     
     # Initialisation � blanc d'un fichier fits
     #buf$audace(bufNo) format $len 1
     buf$audace(bufNo) setpixels CLASS_GRAY $len 1 FORMAT_FLOAT COMPRESS_NONE 0
     
     set intensite 0
     #--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
     # Une liste commence � 0 ; Un vecteur fits commence � 1
     for {set k 0} {$k<$len} {incr k} {
	 append intensite [lindex $intensites $k]
	 #::console::affiche_resultat "$intensite\n"
	 if { [regexp {([0-9]+\.*[0-9]*)} $intensite match mintensite] } {
	     buf$audace(bufNo) setpix [list [expr $k+1] 1] $mintensite
	     set intensite 0
	 }
     }
     
     set profilspc(xunit) "Position"
     set profilspc(yunit) "ADU"
     #------- Affecte une valeur aux mots cle li�s � la spectroscopie ----------
     #-- buf$audace(bufNo) setkwd [list "INSTRU" "T310" string "mon t�lescope" ""]
     #buf$audace(bufNo) setkwd [list "NAXIS1" "$len" int "" ""]
     #buf$audace(bufNo) setkwd [list "NAXIS2" "1" int "" ""]
     #--- Valeur minimale de l'abscisse (xdepart) : =0 si profil non �talonn�
     set xdepart [ expr 0.0+1.0*[lindex $profilspc(pixels) 0] ]
     buf$audace(bufNo) setkwd [list "CRVAL1" $xdepart $nbunit "" ""]
     #--- Valeur de la longueur d'onde/pixel central(e)
     set xdernier [lindex $profilspc(pixels) [expr $profilspc(naxis1)-1]]
     ::console::affiche_resultat "Xdep : $xdepart ; Xfin : $xdernier\n"
     #set xcentre [expr int(0.5*($xdernier-$xdepart))]
     #buf$audace(bufNo) setkwd [list "CRPIX1" "$xcentre" double "" ""]
     #--- Dispersion du spectre :
     #-- Calcul dans le cas d'une dispersion non lin�aire
     set l1 [lindex $profilspc(pixels) 1]
     set l2 [lindex $profilspc(pixels) [expr int($len/10)]]
     set l3 [lindex $profilspc(pixels) [expr int(2*$len/10)]]
     set l4 [lindex $profilspc(pixels) [expr int(3*$len/10)]]
     set dl1 [expr ($l2-$l1)/(int($len/10)-1.0)]
     set dl2 [expr ($l4-$l3)/(int($len/10)-1.0)]
     set xincr [expr 0.5*($dl2+$dl1)]
     #-- Mesure de la dispersion suppos�e lin�aire
     set l1 [lindex $profilspc(pixels) 1]
     set l2 [lindex $profilspc(pixels) 2]
     set xincr [expr 1.0*abs($l2-$l1)]
     
     #-- Meth2 : erreur si spectre de moins de 4 pixels
     #set l2 [lindex $profilspc(pixels) 4]
     #set l3 [lindex $profilspc(pixels) [expr 1+int($len/2)]]
     #set l4 [lindex $profilspc(pixels) [expr 4+int($len/2)]]
     #set dl1 [expr ($l2-$l1)/3]
     #set dl2 [expr ($l4-$l3)/3]
     #set xincr [expr 0.5*($dl2+$dl1)]
     
     ::console::affiche_resultat "Dispersion : $xincr\n"
     buf$audace(bufNo) setkwd [list "CDELT1" $xincr $nbunit "" ""]
     #--- Type de dispersion : LINEAR, NONLINEAR
     
     #--- Sauve le fichier fits ainsi constitu� 
     ::console::affiche_resultat "$len lignes affect�es\n"
     buf$audace(bufNo) bitpix float
     if {[llength $args] == 1} {
	 set nom [ file rootname $filenamespc ]
	 buf$audace(bufNo) save "$audace(rep_images)/${nom}$conf(extension,defaut)"
	 ::console::affiche_resultat "Fichier fits sauv� sous $audace(rep_images)/${nom}$conf(extension,defaut)\n"
     } elseif {[llength $args] == 2} {
	 set nom [ file rootname $filenameout ]
	 buf$audace(bufNo) save "$audace(rep_images)/${filenameout}$conf(extension,defaut)"
	 ::console::affiche_resultat "Fichier fits sauv� sous $audace(rep_images)/${filenameout}$conf(extension,defaut)\n"
     }
 } else {
     ::console::affiche_erreur "Usage: spc_dat2fits fichier_profil.dat ?fichier_sortie.fit?\n\n"
 }
}
#****************************************************************#



proc spc_fits2dat_160505 { args } {

 global conf
 global audace
 global profilspc
 #global captionspc
 global colorspc
 set extsp ".dat"

 if {[llength $args] == 1} {
   set filenamespc [ lindex $args 0 ]
   buf$audace(bufNo) load "$audace(rep_images)/$filenamespc"
   #buf$audace(bufNo) load $filenamespc
   set naxis1 [lindex [buf$audace(bufNo) getkwd "NAXIS1"] 1]
   #--- Valeur minimale de l'abscisse : =0 si profil non �talonn�
   set xdepart [lindex [buf$audace(bufNo) getkwd "CRVAL1"] 1]
   #--- Dispersion du spectre : =1 si profil non �talonn�
   set xincr [lindex [buf$audace(bufNo) getkwd "CDELT1"] 1]
   #--- Pixel de l'abscisse centrale
   set xcenter [lindex [buf$audace(bufNo) getkwd "CRPIX1"] 1]
   #--- Type de spectre : LINEAR ou NONLINEAR (elinine les espaces dans la valeur du mot cle.
   set dtype [string trim [lindex [buf$audace(bufNo) getkwd "CTYPE1"] 1]]
   #::console::affiche_resultat "Ici 1\n"
   #if { $dtype != "LINEAR" || $dtype == "" } {
   #    ::console::affiche_resultat "Le spectre ne poss�de pas une disersion lin�aire. Pas de conversion possible.\n"
   #    break
   #}
   #::console::affiche_resultat "Ici 2\n"
   set len [expr int($naxis1/$xincr)]
   ::console::affiche_resultat "$naxis1 intensit�s � traiter\n"

   if { $xdepart != 1 } {
       set fileetalonnespc [ file rootname $filenamespc ]
       #set filename ${fileetalonnespc}_dat$extsp
       set filename ${fileetalonnespc}$extsp
       set file_id [open "$audace(rep_images)/$filename" w+]

       #--- Une liste commence � 0 ; Un vecteur fits commence � 1
       for {set k 0} {$k<$naxis1} {incr k} {
	   #-- Donne les bonnes valeurs aux abscisses si le spectre est �talonn� en longueur d'onde
	   set lambda [expr $xdepart+($k)*$xincr*1.0]
	   #-- Lit la valeur des elements du fichier fit
	   set intensite [buf$audace(bufNo) getpix [list [expr $k+1] 1]]
	   ##lappend profilspc(intensite) $intensite
	   #-- Ecrit les couples "Lambda Intensite" dans le fichier de sortie
	   puts $file_id "$lambda\t$intensite"
       }
       close $file_id
       ::console::affiche_resultat "Fichier fits export� sous $audace(rep_images)/${fileetalonnespc}$extsp\n"
   } else {
       #--- Retire l'extension .dat du nom du fichier
       set filespacialspc [ file rootname $filenamespc ]
       #buf$audace(bufNo) imaseries "PROFILE filename=${filespacialspc}_dat$extsp direction=x offset=1"
       buf$audace(bufNo) imaseries "PROFILE filename=$audace(rep_images)/${filespacialspc}$extsp direction=x offset=1"
       ::console::affiche_resultat "Fichier fits export� sous $audace(rep_images)/${filespacialspc}$extsp\n"
   }
 } else {
   ::console::affiche_erreur "Usage: spc_fits2dat fichier_profil.fit\n\n"
 }
}
#****************************************************************#



proc spc_data2fits_051219a { args } {
    global audace
    global conf
    set precision 0.05
    set nbunit "float"
    #set nbunit "double"

    if { [llength $args] == 2 } {
	#set nom_fichier [ file rootname [lindex 0] ]
	set nom_fichier [lindex $args 0]
	set coordonnees [lindex $args 1]
	set abscisses [lindex $coordonnees 0]
	set intensites [lindex $coordonnees 1]
	set len [llength $abscisses]

	#--- Cr�ation du fichier fits
	buf$audace(bufNo) setpixels CLASS_GRAY $len 1 FORMAT_FLOAT COMPRESS_NONE 0
	buf$audace(bufNo) setkwd [list "NAXIS1" $len int "" ""]
	buf$audace(bufNo) setkwd [list "NAXIS2" 1 int "" ""]
	#-- Valeur minimale de l'abscisse (xdepart) : =0 si profil non �talonn�
	set xdepart [expr 1.0*[lindex $abscisses 0] ]
	buf$audace(bufNo) setkwd [list "CRVAL1" $xdepart $nbunit "" "Angstrom"]
	#-- Dispersion
	#set dispersion [spc_dispersion_moy $abscisses]
	set l1 [lindex $abscisses 1]
	set l2 [lindex $abscisses [expr int($len/10)]]
	set l3 [lindex $abscisses [expr int(2*$len/10)]]
	set l4 [lindex $abscisses [expr int(3*$len/10)]]
	set dl1 [expr ($l2-$l1)/(int($len/10)-1)]
	set dl2 [expr ($l4-$l3)/(int($len/10)-1)]
	set dispersion [expr 0.5*($dl2+$dl1)]
	buf$audace(bufNo) setkwd [list "CDELT1" $dispersion $nbunit "" "Angstrom/pixel"]

	#--- Type de dispersion : LINEAR, NONLINEAR
	#if { [expr abs($dl2-$dl1)] <= $precision } {
	#    buf$audace(bufNo) setkwd [list "CTYPE1" "LINEAR" string "" ""]
	#} elseif { [expr abs($dl2-$dl1)] > $precision } {
	#    buf$audace(bufNo) setkwd [list "CTYPE1" "NONLINEAR" string "" ""]
	#}

	#--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
	# Une liste commence � 0 ; Un vecteur fits commence � 1
	set intensite 0
	for {set k 0} {$k<$len} {incr k} {
	    append intensite [lindex $intensites $k]
	    #::console::affiche_resultat "$intensite\n"
	    if { [regexp {([0-9]+\.*[0-9]*)} $intensite match mintensite] } {
		buf$audace(bufNo) setpix [list [expr $k+1] 1] $mintensite
		set intensite 0
	    }
	}

	#--- Sauvegarde du fichier fits ainsi cr��
	buf$audace(bufNo) bitpix float
	buf$audace(bufNo) save "$audace(rep_images)/$nom_fichier"
	return $nom_fichier
    } else {
       ::console::affiche_erreur "Usage: spc_data2fits nom_fichier_fits_sortie liste_coordonn�es_x_et_y\n\n"
    }
}
#**********************************************************************#


proc spc_data2fits_051215a { args } {
    global audace
    global conf
    set precision 0.05
    set nbunit "float"
    #set nbunit "double"

    if { [llength $args] == 2 } {
	#set nom_fichier [ file rootname [lindex 0] ]
	set nom_fichier [lindex $args 0]
	set coordonnees [lindex $args 1]
	set abscisses [lindex $coordonnees 0]
	set intensites [lindex $coordonnees 1]
	set len [llength $abscisses]

	#--- Cr�ation du fichier fits
	buf$audace(bufNo) setpixels CLASS_GRAY $len 1 FORMAT_FLOAT COMPRESS_NONE 0
	buf$audace(bufNo) setkwd [list "NAXIS1" $len int "" ""]
	buf$audace(bufNo) setkwd [list "NAXIS2" 1 int "" ""]
	#-- Valeur minimale de l'abscisse (xdepart) : =0 si profil non �talonn�
	set xdepart [ expr 1.0*[lindex $abscisses 0]]
	buf$audace(bufNo) setkwd [list "CRVAL1" $xdepart $nbunit "" "Angstrom"]
	#-- Dispersion
	#set dispersion [spc_dispersion_moy $abscisses]
	set l1 [lindex $abscisses 1]
	set l2 [lindex $abscisses [expr int($len/10)]]
	set l3 [lindex $abscisses [expr int(2*$len/10)]]
	set l4 [lindex $abscisses [expr int(3*$len/10)]]
	set dl1 [expr ($l2-$l1)/(int($len/10)-1)]
	set dl2 [expr ($l4-$l3)/(int($len/10)-1)]
	set dispersion [expr 0.5*($dl2+$dl1)]
	buf$audace(bufNo) setkwd [list "CDELT1" $dispersion $nbunit "" "Angstrom/pixel"]

	#--- Type de dispersion : LINEAR, NONLINEAR
	#if { [expr abs($dl2-$dl1)] <= $precision } {
	#    buf$audace(bufNo) setkwd [list "CTYPE1" "LINEAR" string "" ""]
	#} elseif { [expr abs($dl2-$dl1)] > $precision } {
	#    buf$audace(bufNo) setkwd [list "CTYPE1" "NONLINEAR" string "" ""]
	#}

	#--- Rempli la matrice 1D du fichier fits avec les valeurs du profil de raie ---
	# Une liste commence � 0 ; Un vecteur fits commence � 1
	set intensite 0
	for {set k 0} {$k<$len} {incr k} {
	    append intensite [lindex $intensites $k]
	    #::console::affiche_resultat "$intensite\n"
	    if { [regexp {([0-9]+\.*[0-9]*)} $intensite match mintensite] } {
		buf$audace(bufNo) setpix [list [expr $k+1] 1] $mintensite
		set intensite 0
	    }
	}

	#--- Sauvegarde du fichier fits ainsi cr��
	buf$audace(bufNo) bitpix float
	buf$audace(bufNo) save $audace(rep_images)/$nom_fichier
	return $nom_fichier
    } else {
       ::console::affiche_erreur "Usage: spc_data2fits nom_fichier_fits_sortie liste_coordonn�es_x_et_y\n\n"
    }
}
#****************************************************************#

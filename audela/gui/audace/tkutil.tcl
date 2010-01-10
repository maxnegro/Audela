#
# Fichier : tkutil.tcl
# Description : Regroupement d'utilitaires
# Auteur : Robert DELMAS
# Mise a jour $Id: tkutil.tcl,v 1.24 2010-01-10 16:05:57 robertdelmas Exp $
#

namespace eval tkutil:: {
   global audace

   #--- Chargement des captions
   source [ file join $audace(rep_caption) tkutil.cap ]
}

#
# getOpenFileType
#    Gere les differentes extensions des fichiers images, ainsi que le cas ou l'extension
#    des fichiers FITS est differente de .fit
#
proc ::tkutil::getOpenFileType { } {
   variable openFileType
   global audace caption

   #---
   set openFileType [ list ]
   #---
   if { ( [ buf$audace(bufNo) extension ] != ".fit" ) && ( [ buf$audace(bufNo) extension ] != ".fts" ) &&
      ( [ buf$audace(bufNo) extension ] != ".fits" ) } {
      lappend openFileType \
         [ list "$caption(tkutil,image_file)" [ buf$audace(bufNo) extension ] ] \
         [ list "$caption(tkutil,image_file)" [ buf$audace(bufNo) extension ].gz ] \
         [ list "$caption(tkutil,image_fits)" [ buf$audace(bufNo) extension ] ] \
         [ list "$caption(tkutil,image_fits)" [ buf$audace(bufNo) extension ].gz ]
   }

   #---
   lappend openFileType \
      [ list "$caption(tkutil,image_file)"       {.fit}                ] \
      [ list "$caption(tkutil,image_file)"       {.fit.gz}             ] \
      [ list "$caption(tkutil,image_file)"       {.fts}                ] \
      [ list "$caption(tkutil,image_file)"       {.fts.gz}             ] \
      [ list "$caption(tkutil,image_file)"       {.fits}               ] \
      [ list "$caption(tkutil,image_file)"       {.fits.gz}            ] \
      [ list "$caption(tkutil,image_file)"       {.crw .cr2 .nef .dng} ] \
      [ list "$caption(tkutil,image_file)"       {.CRW .CR2 .NEF .DNG} ] \
      [ list "$caption(tkutil,image_file)"       {.jpeg .jpg}          ] \
      [ list "$caption(tkutil,image_file)"       {.bmp}                ] \
      [ list "$caption(tkutil,image_file)"       {.gif}                ] \
      [ list "$caption(tkutil,image_file)"       {.png}                ] \
      [ list "$caption(tkutil,image_file)"       {.tiff .tif}          ] \
      [ list "$caption(tkutil,image_file)"       {.xbm}                ] \
      [ list "$caption(tkutil,image_file)"       {.xpm}                ] \
      [ list "$caption(tkutil,image_file)"       {.ps .eps}            ] \
      [ list "$caption(tkutil,image_fits)"       {.fit}                ] \
      [ list "$caption(tkutil,image_fits)"       {.fit.gz}             ] \
      [ list "$caption(tkutil,image_fits)"       {.fts}                ] \
      [ list "$caption(tkutil,image_fits)"       {.fts.gz}             ] \
      [ list "$caption(tkutil,image_fits)"       {.fits}               ] \
      [ list "$caption(tkutil,image_fits)"       {.fits.gz}            ] \
      [ list "$caption(tkutil,image_raw)"        {.crw .cr2 .nef .dng} ] \
      [ list "$caption(tkutil,image_raw)"        {.CRW .CR2 .NEF .DNG} ] \
      [ list "$caption(tkutil,image_jpeg)"       {.jpeg .jpg}          ] \
      [ list "$caption(tkutil,image_bmp)"        {.bmp}                ] \
      [ list "$caption(tkutil,image_gif)"        {.gif}                ] \
      [ list "$caption(tkutil,image_png)"        {.png}                ] \
      [ list "$caption(tkutil,image_tiff)"       {.tiff .tif}          ] \
      [ list "$caption(tkutil,image_xbm)"        {.xbm}                ] \
      [ list "$caption(tkutil,image_xpm)"        {.xpm}                ] \
      [ list "$caption(tkutil,image_postscript)" {.ps .eps}            ] \
      [ list "$caption(tkutil,image_gif)"        {}      GIFF          ] \
      [ list "$caption(tkutil,image_jpeg)"       {}      JPEG          ] \
      [ list "$caption(tkutil,image_png)"        {}      PNGF          ] \
      [ list "$caption(tkutil,image_tiff)"       {}      TIFF          ] \
      [ list "$caption(tkutil,fichier_tous)"     *                     ]
}

#
# box_load parent initialdir numero_buffer type
#    Ouvre la fenetre de selection des fichiers a proposer au chargement (hors fichiers html)
#
proc ::tkutil::box_load { { parent } { initialdir } { numero_buffer } { type } { visuNo "1" } } {
   variable openFileType
   global caption

   #--- Ouvre la fenetre de choix des fichiers
   if { $type == "1" } {
      set title "$caption(tkutil,charger_image) (visu$visuNo)"
      ::tkutil::getOpenFileType
      set filetypes "$openFileType"
   } elseif { $type == "2" } {
      set title "$caption(tkutil,editer_script)"
      set filetypes [ list [ list "$caption(tkutil,fichier_tcl)" ".tcl" ] \
         [ list "$caption(tkutil,fichier_txt)" ".txt" ] [ list "$caption(tkutil,fichier_tous)" "*" ] ]
   } elseif { $type == "3" } {
      set title "$caption(tkutil,lancer_script)"
      set filetypes [ list [ list "$caption(tkutil,fichier_tcl)" ".tcl" ] ]
   } elseif { $type == "4" } {
      set title "$caption(tkutil,editer_notice)"
      set filetypes [ list [ list "$caption(tkutil,fichier_pdf)" ".pdf" ] ]
   } elseif { $type == "5" } {
      set title "$caption(tkutil,editer_catalogue)"
      set filetypes [ list [ list "$caption(tkutil,fichier_txt)" ".txt" ] ]
   } elseif { $type == "6" } {
      set title "$caption(tkutil,editeur_script)"
      set filetypes [ list [ list "$caption(tkutil,fichier_tous)" "*" ] ]
   } elseif { $type == "7" } {
      set title "$caption(tkutil,editeur_pdf)"
      set filetypes [ list [ list "$caption(tkutil,fichier_tous)" "*" ] ]
   } elseif { $type == "8" } {
      set title "$caption(tkutil,editeur_page_web)"
      set filetypes [ list [ list "$caption(tkutil,fichier_tous)" "*" ] ]
   } elseif { $type == "9" } {
      set title "$caption(tkutil,editeur_image)"
      set filetypes [ list [ list "$caption(tkutil,fichier_tous)" "*" ] ]
   } elseif { $type == "10" } {
      set title "$caption(tkutil,editer_modpoi)"
      set filetypes [ list [ list "$caption(tkutil,fichier_txt)" ".txt" ] ]
   } elseif { $type == "11" } {
      set title "$caption(tkutil,editer_fichier)"
      set filetypes [ list [ list "$caption(tkutil,fichier_txt)" "*" ] ]
   } elseif { $type == "12" } {
      set title "$caption(tkutil,executable_java)"
      set filetypes [ list [ list "$caption(tkutil,fichier_tous)" "*" ] ]
   } elseif { $type == "13" } {
      set title "$caption(tkutil,executable_aladin)"
      if { $::tcl_platform(os) == "Linux" } {
         set filetypes [ list [ list "$caption(tkutil,fichier_jar)" ".jar" ] ]
      } else {
         set filetypes [ list [ list "$caption(tkutil,fichier_tous)" "*" ] ]
      }
   }
   set filename [ tk_getOpenFile -title $title -filetypes $filetypes -initialdir $initialdir -parent $parent ]
   #---
   catch {
      #--- Je detruis la boite de dialogue cree par tk_getOpenFile
      #--- car sous Linux la fenetre n'est pas detruite a la fin de
      #--- l'utilisation (bug de linux ?)
      destroy $parent.__tk_filedialog
   }
   #---
   return $filename
}

#
# box_load_html parent initialdir numero_buffer type
#    Ouvre la fenetre de selection des fichiers html a proposer au chargement
#
proc ::tkutil::box_load_html { { parent } { initialdir } { numero_buffer } { type } } {
   global caption

   #--- Ouvre la fenetre de choix des fichiers
   if { $type == "1" } {
      set title "$caption(tkutil,editer_site_web)"
      set filetypes [ list [ list "$caption(tkutil,fichier_html)" ".htm" ] ]
   }
   set filename [ file join file:///[ tk_getOpenFile -title $title \
      -filetypes $filetypes -initialdir $initialdir -parent $parent ] ]
   #---
   catch {
      #--- Je detruis la boite de dialogue cree par tk_getOpenFile
      #--- car sous Linux la fenetre n'est pas detruite a la fin de
      #--- l'utilisation (bug de linux ?)
      destroy $parent.__tk_filedialog
   }
   #---
   return $filename
}

#
# getSavefiletype
#    Gere les differentes extensions des fichiers images, ainsi que le cas ou l'extension
#    des fichiers FITS est differente de .fit, de .fts et de .fits
#
proc ::tkutil::getSaveFileType { } {
   variable saveFileType
   global audace caption conf

   #---
   set saveFileType [ list ]
   #---
   if { ( [ buf$audace(bufNo) extension ] != ".fit" ) && ( [ buf$audace(bufNo) extension ] != ".fts" ) &&
      ( [ buf$audace(bufNo) extension ] != ".fits" ) && ( [ buf$audace(bufNo) extension ] != ".crw" ) &&
      ( [ buf$audace(bufNo) extension ] != ".CRW" ) && ( [ buf$audace(bufNo) extension ] != ".cr2" ) &&
      ( [ buf$audace(bufNo) extension ] != ".CR2" ) && ( [ buf$audace(bufNo) extension ] != ".nef" ) &&
      ( [ buf$audace(bufNo) extension ] != ".NEF" ) && ( [ buf$audace(bufNo) extension ] != ".dng" ) &&
      ( [ buf$audace(bufNo) extension ] != ".DNG" ) } {
      set x [ list "$caption(tkutil,image_fits)"    [ buf$audace(bufNo) extension ] ]
      set y [ list "$caption(tkutil,image_fits) gz" [ buf$audace(bufNo) extension ].gz ]
   }

   #---
   set a [ list "$caption(tkutil,image_fits) "  {.fit}     ]
   set b [ list "$caption(tkutil,image_fits) 1" {.fit.gz}  ]
   set c [ list "$caption(tkutil,image_fits) 2" {.fts}     ]
   set d [ list "$caption(tkutil,image_fits) 3" {.fts.gz}  ]
   set e [ list "$caption(tkutil,image_fits) 4" {.fits}    ]
   set f [ list "$caption(tkutil,image_fits) 5" {.fits.gz} ]
   set g [ list "$caption(tkutil,image_raw) "   {.crw}     ]
   set h [ list "$caption(tkutil,image_raw) 1"  {.CRW}     ]
   set i [ list "$caption(tkutil,image_raw) 2"  {.cr2}     ]
   set j [ list "$caption(tkutil,image_raw) 3"  {.CR2}     ]
   set k [ list "$caption(tkutil,image_raw) 4"  {.nef}     ]
   set l [ list "$caption(tkutil,image_raw) 5"  {.NEF}     ]
   set m [ list "$caption(tkutil,image_raw) 6"  {.dng}     ]
   set n [ list "$caption(tkutil,image_raw) 7"  {.DNG}     ]
   #---
   set jpg [ list "$caption(tkutil,image_jpeg)" {.jpg}     ]
   set bmp [ list "$caption(tkutil,image_bmp)"  {.bmp}     ]
   set gif [ list "$caption(tkutil,image_gif)"  {.gif}     ]
   set png [ list "$caption(tkutil,image_png)"  {.png}     ]
   set tif [ list "$caption(tkutil,image_tiff)" {.tif}     ]
   set xbm [ list "$caption(tkutil,image_xbm)"  {.xbm}     ]
   set xpm [ list "$caption(tkutil,image_xpm)"  {.xpm}     ]

   if { [ buf$audace(bufNo) extension ] == ".fit" } {
      if { $conf(fichier,compres) == "0" } {
         lappend saveFileType $a $b $c $d $e $f $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      } elseif { $conf(fichier,compres) == "1" } {
         lappend saveFileType $b $a $c $d $e $f $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      }
   } elseif { [ buf$audace(bufNo) extension ] == ".fts" } {
      if { $conf(fichier,compres) == "0" } {
         lappend saveFileType $c $d $a $b $e $f $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      } elseif { $conf(fichier,compres) == "1" } {
         lappend saveFileType $d $c $a $b $e $f $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      }
   } elseif { [ buf$audace(bufNo) extension ] == ".fits" } {
      if { $conf(fichier,compres) == "0" } {
         lappend saveFileType $e $f $a $b $c $d $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      } elseif { $conf(fichier,compres) == "1" } {
         lappend saveFileType $f $e $a $b $c $d $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      }
   } elseif { [ buf$audace(bufNo) extension ] == ".crw" } {
      lappend saveFileType $g $h $i $j $k $l $m $n $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } elseif { [ buf$audace(bufNo) extension ] == ".CRW" } {
      lappend saveFileType $h $g $i $j $k $l $m $n $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } elseif { [ buf$audace(bufNo) extension ] == ".cr2" } {
      lappend saveFileType $i $j $k $l $m $n $g $h $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } elseif { [ buf$audace(bufNo) extension ] == ".CR2" } {
      lappend saveFileType $j $i $k $l $m $n $g $h $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } elseif { [ buf$audace(bufNo) extension ] == ".nef" } {
      lappend saveFileType $k $l $m $n $g $h $i $j $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } elseif { [ buf$audace(bufNo) extension ] == ".NEF" } {
      lappend saveFileType $l $k $m $n $g $h $i $j $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } elseif { [ buf$audace(bufNo) extension ] == ".dng" } {
      lappend saveFileType $m $n $g $h $i $j $k $l $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } elseif { [ buf$audace(bufNo) extension ] == ".DNG" } {
      lappend saveFileType $n $m $g $h $i $j $k $l $a $b $c $d $e $f $jpg $bmp $gif $png $tif $xbm $xpm
   } else {
      if { $conf(fichier,compres) == "0" } {
         lappend saveFileType $x $y $a $b $c $d $e $f $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      } elseif { $conf(fichier,compres) == "1" } {
         lappend saveFileType $y $x $a $b $c $d $e $f $g $h $i $j $k $l $m $n $jpg $bmp $gif $png $tif $xbm $xpm
      }
   }
}

#
# box_save parent initialdir numero_buffer type
#    Ouvre la fenetre de selection des fichiers a proposer a la sauvegarde
#
proc ::tkutil::box_save { { parent } { initialdir } { numero_buffer } { type } { visuNo "" } } {
   variable saveFileType
   global caption conf

   #--- Ouvre la fenetre de choix des fichiers
   if { $type == "1" } {
      set title "$caption(tkutil,sauver_image) (visu$visuNo)"
      ::tkutil::getSaveFileType
      set filetypes "$saveFileType"
      set filename [ tk_getSaveFile -title $title -filetypes $filetypes -initialdir $initialdir -parent $parent ]
   } elseif { $type == "2" } {
      set title "$caption(tkutil,sauver_image_jpeg) (visu1)"
      set filetypes [ list [ list "$caption(tkutil,image_jpeg)" ".jpg" ] ]
      set filename [ tk_getSaveFile -title $title -filetypes $filetypes -initialdir $initialdir -parent $parent -defaultextension ".jpg" ]
   }
   if { $filename == "" } {
      return
   }
   return $filename
}

#
# lgEntryComboBox liste
#    Fourni la largeur de l'entry d'une combobox adaptee au plus long element de la liste
#
proc ::tkutil::lgEntryComboBox { liste } {
   set a "0"
   set lgListe [ llength $liste ]
   for { set k 1 } { $k <= $lgListe} { incr k } {
      set index [ expr $k - 1 ]
      set lgElement [ string length [ lindex $liste $index ] ]
      set b $lgElement
      if { $b > $a } {
         set a $b
      }
   }
   if { $a == "0" } {
      set a "5"
   }
   set longEntryComboBox [ expr $a + 2 ]
   return $longEntryComboBox
}

#
# displayErrorInfo
#    Affiche le contenu de ::errorInfo dans la Console et dans une fenetre modale
#    avec eventuellement un message optionnel
#
proc ::tkutil::displayErrorInfo { title { messageOptionnel "" } } {
   #--- j'affiche le message complet dans la console
   ::console::affiche_erreur "$::errorInfo\n"
   #--- j'affiche le message d'erreur dasn une fenetre modale.
   tk_messageBox -icon error -title $title \
      -message "[string range $::errorInfo 0 [string first "\n    while executing" $::errorInfo]]$messageOptionnel"
}

##--------------------------------------------------------------
# validateNumber
#    verifie la valeur saisie dans un widget
#    Cette verification est activee en ajoutant les options suivantes au widget :
#    -validate all -validatecommand { ::tkutil::validateNumber %W %V %P %s <class> <minValue> <maxValue> <errrorVariable> }
#
# <br>Exemple
# <br>    -validatecommand { ::tkutil::validateNumber %W %V %P %s "numRef" integer -360 360 }
#
# @param  win      : nom tk du widget renseigne avec %W
# @param  event    : evenement (key, focusout,...) sur le widget renseigne avec %V
# @param  newValue : valeur apres l'evenement renseignee avec %P
# @param  oldValue : valeur avant l'evenement renseignee avec %s
# @param  class    : classe de la valeur attendue
#            - boolean : booleen ( 0, 1, false, true, no, yes , off , on)
#            - double  : nombre decimal
#            - integer : nombre entier
# @param  minValue      : valeur minimale du nombre
# @param  maxValue      : valeur maximale du nombre
# @param  errorVariable : nom de la variable d'erreur associee au widget
#
# @return
#   - 1 si OK
#   - 0 si erreur
# @public
#----------------------------------------------------------------------------
proc ::tkutil::validateNumber { win event newValue oldValue class minValue maxValue { errorVariable "" } } {
   variable widget

   set result 0
   if { $event == "key" || $event == "focusout" || $event == "forced" } {
      #--- je verifie la classe
      set classCheck [expr [string is $class -failindex charIndex $newValue] ]
      if {! $classCheck} {
         set fullCheck $classCheck
         if { $errorVariable != "" } {
            set $errorVariable [format $::caption(tkutil,badCharacter) "\"$newValue\"" "\"[string range $newValue $charIndex $charIndex]\"" ]
         }
         set result $classCheck
      } else {
         #--- je verifie l'ordre des bornes de l'intervalle
         if {$minValue > $maxValue} {
            set tmp $minValue; set minValue $maxValue; set maxValue $tmp
         }
         #--- je verifie la plage
         if { $errorVariable != "" } {
            if { $newValue < $minValue } {
               set $errorVariable [format $::caption(tkutil,numberTooSmall) $newValue $minValue ]
            } elseif { $newValue > $maxValue } {
               set $errorVariable [format $::caption(tkutil,numberTooGreat) $newValue $maxValue ]
            } else {
               if { [info exists $errorVariable] } {
                  unset $errorVariable
               }
            }
         }
         if { $newValue == "" } {
            set textVariable [$win cget -textvariable]
            set newValue $minValue
            if { $textVariable != "" } {
               #---
               set $textVariable $newValue
            }
         }
         set fullCheck [expr {$classCheck && ($newValue >= $minValue) && ($newValue <= $maxValue)}]
         set result $fullCheck
      }
      if { $result == 0 } {
         #--- j'affiche en inverse video
         $win configure -bg $::audace(color,entryBackColor2) -fg $::audace(color,entryTextColor)
      } else {
         #--- j'affiche normalement
         $win configure -bg $::audace(color,entryBackColor) -fg $::audace(color,entryTextColor)
      }
   } else {
      #--- je ne traite pas l'evenement
      set result 1
   }
   ####console::disp "win=$win event=$event newValue=$newValue oldValue=$oldValue result=$result\n"
   return $result
}

##--------------------------------------------------------------
# validateString
#    Verifie le caractere saisie dans un widget
#    Cette verification est activee en ajoutant les options suivantes au widget :
#    -validate all -validatecommand { ::tkutil::validateNumber %W %V %P %s <class> <minLength> <maxLength> ?<errrorVariable>? }
#
# <br>Exemple 1
# <br>    -validatecommand { ::tkutil::validateString %W %V %P %s fits 1 70 }
#
# <br>Exemple 2 : avec variable de controle
# <br>    entry .frame.yyy -validatecommand { ::tkutil::validateString %W %V %P %s fits 1 70 ::xxxx::widget($visuNo,error,yyy) }
# <br>    entry .frame.zzz -validatecommand { ::tkutil::validateString %W %V %P %s fits 1 70 ::xxxx::widget($visuNo,error,zzz) }
#
# <br> puis faire les controles dans la procedure ::xxxx::apply
#     if { [array names ::xxxx::widget $visuNo,error,* ] != "" } {
#        #--- j'affiche un message d'erreur s'il existe au moins une variable ::xxxx::widget($visuNo,error,...)
#        ...
#     }
#
# @param  win      : nom tk du widget renseigne avec %W
# @param  event    : evenement sur le widget renseigne avec %V (key, focusout,...)
# @param  newValue : valeur apres l'evenement renseignee avec %P
# @param  oldValue : valeur avant l'evenement renseignee avec %s
# @param  class    : classe de la valeur attendue
#            - alnum    : caractere alphabetique ou numerique
#            - alpha    : caractere alphabetique
#            - ascii    : caractere ASCII dont le code est inferieur ou egal a 127
#            - boolean  : booleen ( 0, 1, false, true, no, yes , off , on)
#            - fits     : caractere autorise dans un mot cle FITS
#            - wordchar : caractere alphabetique ou numerique ou underscore
#            - xdigit   : caractere hexadecimal
# @param  minLength     : longueur minimale de la chaine
# @param  maxLength     : longueur maximale de la chaine
# @param  errorVariable : nom de la variable d'erreur associee au widget
#
# @return
#   - 1 si OK
#   - 0 si erreur
# @private
#----------------------------------------------------------------------------
proc ::tkutil::validateString { win event newValue oldValue class minLength maxLength { errorVariable "" } } {
   variable widget

   set result 0
   if { $event == "key" || $event == "focusout" || $event == "forced" } {
      #--- je verifie la classe
      if { $class == "fits" } {
         set classCheck [expr [string is ascii -failindex charIndex $newValue] ]
        ### set classCheck [expr [[regexp -all {[\u0000-\u0029]|[\u007F-\u00FF]} $newValue ] != 0 ] ]
      } else {
         set classCheck [expr [string is $class -failindex charIndex $newValue] ]
      }
      if {! $classCheck} {
         set $errorVariable [format $::caption(tkutil,badCharacter) "\"$newValue\"" "\"[string range $newValue $charIndex $charIndex]\"" ]
         set result $classCheck
      } else {
         #--- je verifie l'ordre des bornes de longueur
         if {$minLength > $maxLength} {
            set tmp $minLength; set minLength $maxLength; set maxLength $tmp
         }
         #--- je verifie la longueur de la chaine
         set xLength [string length $newValue]
         if { $xLength < $minLength } {
            if { $errorVariable != "" } {
               set $errorVariable [format $::caption(tkutil,stringTooShort) "\"$newValue\"" $minLength]
            }
            set result 0
         } elseif { $xLength > $maxLength } {
            if { $errorVariable != "" } {
               set $errorVariable [format $::caption(tkutil,stringTooLarge) "\"$newValue\"" $maxLength]
            }
            set result 0
         } else {
            if { $errorVariable != "" } {
               if { [info exists $errorVariable] } {
                  unset $errorVariable
               }
            }
            set result 1
         }
      }
      if { $result == 0 } {
         #--- j'affiche en inverse video
         $win configure -bg $::audace(color,entryBackColor2) -fg $::audace(color,entryTextColor)
      } else {
         #--- j'affiche normalement
         $win configure -bg $::audace(color,entryBackColor) -fg $::audace(color,entryTextColor)
      }
   } else {
      #--- je ne traite pas l'evenement
      set result 1
   }
   return $result
}


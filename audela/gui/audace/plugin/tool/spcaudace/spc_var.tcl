####################################################################
# Spécification des variables utilisées par spcaudace
#
####################################################################

# Mise a jour $Id$


#----------------------------------------------------------------------------------#
#--- Initialisation des variables d'environnement d'SpcAudace :
global audela audace
global spcaudace

#--- Version d'SpcAudace :
set spcaudace(num_version) "3.7"
set spcaudace(version) "$spcaudace(num_version) - 25/06/2014"
# ::audace::date_sys2ut ?Date?
#set spcaudace(version) [ file mtime $spcaudace(repspc) ]
set spcaudace(flag_verifversion) 0

#--- Liste des contributeurs au développement d'SpcAudace :
set spcaudace(author) "Benjamin MAUCLAIRE"
set spcaudace(contribs) "Alain Klotz, Michel Pujol, Patrick Lailly, François Cochard"


#--- Extension des fichiers :
set spcaudace(extdat) ".dat"
set spcaudace(exttxt) ".txt"
set spcaudace(extvspec) ".spc"

#--- Répertoire d'SpcAudace :
if { [regexp {1.3.7} $audela(version) match resu ] } {
   set spcaudace(rep_spc) [ file join $audace(rep_scripts) spcaudace ]
} else {
   set spcaudace(rep_spc) [ file join $audace(rep_plugin) tool spcaudace ]
}


#--- Chemin des répertoires :
#-- Répertoire des outils : Gnuplot, Spectrum... :
set spcaudace(repgp) [ file join $spcaudace(rep_spc) gp ]
set spcaudace(spectrum) [ file join $spcaudace(rep_spc) plugins spectrum ]

#-- Répertoire des données chimiques :
set spcaudace(repchimie) [ file join $spcaudace(rep_spc) data chimie ]
set spcaudace(reptelluric) [ file join $spcaudace(rep_spc) data telluric ]
set spcaudace(filetelluric) "$spcaudace(reptelluric)/h2o_calibrage.txt"
#set spcaudace(filetelluric) "$spcaudace(reptelluric)/h2o_calibrage_140b.txt"
#-- Liste des methodes de calibrations telluriques :
#- set spcaudace(calo_meths) { 1 2 4 5 6 }
set spcaudace(calo_meths) { 1 2 }
#- 1 : spectre initial juste linearise ; toutes configs ; ne pas enlever de la liste.
#- 2 : decalage d'une valeur egale au decalage moyen mean_shift mesure ; toutes configs.
#- 3 : recallibrage de degre 3 avec les raies telluriques, puis decalage de RMS et linearisation ; que kaf1600.
#- 4 : recallibrage de degre 2 avec les raies telluriques, reechantillonne et linearisation a la volee ; que kaf400.
#- 5 : decalage d'une valeur egale au RMS mesure des raies ; toutes configs.
#- 6 : decalage d'une valeur egale au RMS*0.5 mesure des raies ; kaf1600.
#- 7 : Recalage progressif par iterations pour minimiser le RMS mesure ; kaf1600 optionnel.

#-- Effacement des profils des differentes methodes de calibration tellurique :
set spcaudace(flag_rmcalo) "o"

#-- Superflat binne et normalise :
set spcaudace(binned_flat) "n"

#-- Utilisation d'abord de la methode 1 de detection de l'angle de tilt. Par defaut, utilise que la methode 2 (valeur=n) :
set spcaudace(tilt_normal) "o"

#-- Inversion du sens de coubure de la correction du smilex (1, -1) :
set spcaudace(smilex_inv) 1

#-- Répertoire de la bibliothèque spectrale :
set spcaudace(rep_spcbib) [ file join $spcaudace(rep_spc) data bibliotheque_spectrale ]

#-- Répertoire de la calibration-chimie :
set spcaudace(rep_spccal) [ file join $spcaudace(rep_spc) data calibration_lambda ]


#--- Fichiers utilisés :
set spcaudace(sp_eau) "h2o_6500_6700.fit"


#--- Répertoire de la calibration-chimie :
#set spcaudace(motsheader) [ list "OBJNAME" "OBSERVER" "ORIGIN" "TELESCOP" "EQUIPMEN" ]
#set spcaudace(motsheaderdef) [ list "Current name of the object" "Observer name" "Origin place of FITS image" "Telescop" "System which created data via the camera" ]
set spcaudace(motsheader) [ list "OBJNAME" "TELESCOP" "EQUIPMEN" ]
set spcaudace(motsheaderdef) [ list "Current name of the object" "Telescop" "System which created data via the camera" ]


#--- Lieu de la documentation d'SpcAudACE :
set spcaudace(spcdoc) [ file join $spcaudace(rep_spc) doc liste_fonctions.html ]
set spcaudace(sitedoc) "http://wsdiscovery.free.fr/spcaudace/fonctions.html"


#--- Site de bases de données :
set spcaudace(webpage) "http://wsdiscovery.free.fr/spcaudace/"
set spcaudace(sitebess) "http://basebe.obspm.fr/basebe/"
set spcaudace(siteuves) "http://www.sc.eso.org/santiago/uvespop/interface.html"
set spcaudace(sitesimbad) "http://simbad.u-strasbg.fr/simbad/sim-fid"
set spcaudace(sitesurveys) "http://wsdiscovery.free.fr/astronomie/research/"
set spcaudace(sitebebuil) "http://astrosurf.com/buil/us/becat.htm"
set spcaudace(sitearasbeam) "http://arasbeam.free.fr/"

#--- Options d'affichage :
#-- Affichage en mode escalier, sans lissage spline de BLT :
set spcaudace(display_real) 0


#--- Options prédéfinies dans les pipelines :
# set spcaudace(methsel) "moy"
set spcaudace(methsel) "serre"
set spcaudace(methreg) "spc"
set spcaudace(methsky) "med"
#set spcaudace(methbin) "horne"
set spcaudace(methbin) "rober"


#--- Options prédéfinies (par defaut addi, options=add, moy, med, sigmakappa) :
set spcaudace(meth_somme) "addi"

#--- Constantes de calcul :
#-- Vitesse de la lumière en km/s :
set spcaudace(vlum) 299792.458

#--- Valeurs liees aux precisions de mesure :
#-- Mesure d'un photocentre en pixel :
set spcaudace(precision_centerpix) 0.2

#--- Vakeur de paramètres lés à la calibration :
#-- Degre maximum par defaut du policnoime  decalibration (pour 5 raies et plus) :
set spcaudace(degmax_cal) 4
#-- Flag délcarant que les traitements sont pour la basse résolution (affichage modèle calibration...) :
set spcaudace(br) 0
#-- Valeur limite du RMS de la qualité de la calibration au delàa de laquelle la calibration est mauvaise :
set spcaudace(rms_lim) 30
#-- Nombre maximum de pics a rechercher dans la lampe de calibration (spc_findbiglines2)
set spcaudace(nb_pics) 50
#-- seuil minimum d'un pic (exprime en centaines de % du snr) pour qu'il soit condidéré comme une raie de la lampe de calibration (spc_findbiglines2)
set spcaudace(coef_snr) 5.

#--- Valeur de paramètres des euristhiques algorithmiques :
#-- Elimine les bords nuls des profils de raies :
set spcaudace(rm_edges) "o"
#-- Calibration avec les raies telluriques par defaut :
set spcaudace(calo_serie) "n"
#-- Taux adoucissement pour l'extraction de continuum ew via piecewiselinear :
set spcaudace(taux_doucissage) 6.
#-- Valeur de la fwhm des cosmics a detecter :
set spcaudace(cosmics_fwhm) 5.0
#-- Intensitee minimale des cosmics en % du continuum :
set spcaudace(cosmics_imin) 0.15
#-- Largeur du cosmic en nombre de fwhm :
set spcaudace(cosmics_nbsigma) 2.5
#-- Valeur du parametre Kappa servant dans la somme type Kappa-Sigma :
set spcaudace(ssk_kappa) 0.8
#-- Degré du polynome pour l'extraction du continuum (5->2) :
set spcaudace(degpoly_cont) 2
#-- Fraction des bords ignorés dans certains calculs (spc_divri, pwl...) pour la détermination du Imax du profil :
set spcaudace(pourcent_bord) 0.15
set spcaudace(pourcent_bord_br) 0.05
set spcaudace(pourcent_bord_run) $spcaudace(pourcent_bord)
#-- Taux de croissance de l'intensité pour considérer que l'on passe du bord (proche de 0) au continuum :
set spcaudace(croissbord) 0.2
#-- Tolérence sur l'écart à l'intensité maximale (spc_divri) : 50% (avant 5%)
set spcaudace(imax_tolerence) 1.5
#-- Largeur spectrale considérée comme basse résolution :
set spcaudace(bande_br) 1000.0
#-- Fraction des bords ignorés dans la détermination de l'angle de TILT :
set spcaudace(pourcent_bordt) 0.10
#-- Fraction des bords mis à 0 du résultat de la division avant lissage pour la RI :
#- 0.0127 - 0.04
set spcaudace(bordsnuls) 0.04
#-- Dispersion maximale pour un spectre haute résolution (extraction continuum) :
set spcaudace(dmax) 0.5
#-- Bande spectrale considérée comme basse résolution 500 A :
set spcaudace(bp_br) 500.
#-- Hauteur max d'un spectre 2D pour ne considérer que du slant :
set spcaudace(hmax) 3.7
#-- Pourcentage de l'intensité moyenne en deça de laquelle il y a mise a 0 (spc_pwl*) :
set spcaudace(nulpcent) 0.6
#-- Epaisseur de binning en cas de sélection manuelle de raie de calibration :
set spcaudace(epaisseur_bin) 100.
#-- Nombde de coupes verticales pour les detections de profil :
#set spcaudace(nb_coupes) 5
set spcaudace(nb_coupes) 10
#-- Epaisseur de detection (verticale, latérale) pour les tranches de détection lors de spc_findtilt, spc_detect (5% de naxis2) :
set spcaudace(epaisseur_detect) 0.05
#-- Hauteur souhaitée de binning des spectres (si 0, le calcul automatique de hauteur est réalisé par spc_detectasym) :
set spcaudace(hauteur_binning) 0
#-- Coefficient multiplicateur de la FWHM pour l'epaisseur de binning :
#- Pour spc_detectasym :
set spcaudace(cafwhm_binning) 1.9
#- Pour spc_detectmoy :
set spcaudace(cmfwhm_binning) 1.7
#- Pour spc_detect :
set spcaudace(clfwhm_binning) 3.7
#-- Epaisseur de binning par défaut pour flats et détections géométriques :
set spcaudace(largeur_binning) 7
#-- Coefficient de rejection des cosmics lors du binning (0-100) :
set spcaudace(hornethreshold) 40
#-- Angle limite en degrès autorisé pour un tilt :
#set spcaudace(tilt_limit) 0.746
#set spcaudace(tilt_limit) 1.5
#set spcaudace(tilt_limit) 2.
set spcaudace(tilt_limit) 4.
#-- Rapport limit de I_moy_fond_de_ciel/I_moy pour détectivité de l'angle... : NON UTILISE
#set spcaudace(rapport_imoy) 0.97
#-- Linéarisation automatique de la loi de calibration en longueur d'onde :
set spcaudace(linear_cal) "o"
#-- Largeur du filtrage SavGol pour la recherche des raies telluriques :
set spcaudace(largeur_savgol) 28
#-- Demi-largeur (anstroms) de plage de recherche des raies telluriques (spc_calibretelluric) :
set spcaudace(dlargeur_eau) 0.5
#-- Coefficient de uncosmic
set spcaudace(uncosmic) 0.85
#-- Largeur des raies detectees (spc_findbiglines) : 10 ; 8
set spcaudace(largeur_raie_detect) 8


#----------------------------------------------------------------------------------#
# Couleurs et répertoires : (pris dans spc_menu.cap et toujours présent -> migration à terminer)
#--- Liste des couleurs disponibles pour les graphes :
set spcaudace(lgcolors) [ list "darkblue" "green" "lightblue" "red" "blue" "yellow" ]
#-- Indice de la couleur par defaut (darkblue) :
set spcaudace(gcolor) 0
#-- Liste de noms des profils affichés :
set spcaudace(gloaded) [ list ]


#--- definition of colors
#--- definition des couleurs
set colorspc(back) #123.76
#-- Ancien fond des menus : #D9D9D9
set colorspc(backmenu) #ECE9D8
set colorspc(back_infos) #FFCCDD
set colorspc(fore_infos) #000000
#-- Ancien fond du contour graphe : #CCCCCC, #DCDAD5
set colorspc(back_graphborder) #DCDAD5
set colorspc(plotbackground) #FFFFFF
set colorspc(profile) #000088


#--- definition of variables
#--- definition des variables
if { [info exists profilspc(initialfile)] == 0 } {
   set profilspc(initialfile) " "
}
if { [info exists profilspc(xunit)] == 0 } {
   set profilspc(xunit) "screen coord"
}
if { [info exists profilspc(yunit)] == 0 } {
   set profilspc(yunit) "screen coord"
}




#----------------------------------------------------------------------------------#


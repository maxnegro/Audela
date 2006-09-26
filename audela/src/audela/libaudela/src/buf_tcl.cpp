/* buf_tcl.cpp
 *
 * This file is part of the AudeLA project : <http://software.audela.free.fr>
 * Copyright (C) 1998-2004 The AudeLA Core Team
 *
 * Initial author : Denis MARCHAIS <denis.marchais@free.fr>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <stdio.h>

#include "libstd.h"
#include "cpool.h"
#include "cbuffer.h"


//------------------------------------------------------------------------------
// La variable globale est definie de maniere unique ici.
//
CPool *buf_pool;

//------------------------------------------------------------------------------
// Commande internes pour gerer les commandes d'un buffer
//
//int cmdFormat(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdType(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdClear(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdSetKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGetKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGetKwds(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdDelKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdDelKwds(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdCopyKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdImageReady(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdLoadSave(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdLoad3d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdRestoreInitialCut(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdCreate3d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdSave3d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdSave1d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdCopyTo(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGetNaxis(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGetPix(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGetPixels(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGetPixelsHeight(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGetPixelsWidth(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdSetPix(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdSetPixels(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdMergePixels(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdAstroPhot(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdPointer(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdXy2radec(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdRadec2xy(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdFwhm(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdBitpix(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdPhotom(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdBarycenter(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

int cmdTtMirrorX(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtMirrorY(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtRot(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtLog(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtSub(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtAdd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtDiv(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtOffset(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdMult(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtNGain(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtNOffset(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtUnsmear(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtOpt(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdTtStat(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdImaSeries(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

int cmdCompress(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdExtension(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdFitGauss(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdFitGauss2d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdGauss(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdHistogram(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdClipmin(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdClipmax(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdScale(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdScar(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdSaveJpg(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdSaveFitsRGB(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdUnifyBg(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdRegion(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

// Fonctions Libtt de Delphine mises ici pour cause de redimensionnement d'image
// Temporaire ?
int cmdBinX(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdBinY(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdWindow(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdA_StarList(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int cmdAutocuts(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);


static struct cmditem cmdlist[] = {
   {"add", (Tcl_CmdProc *)cmdTtAdd},
   {"autocuts", (Tcl_CmdProc *)cmdAutocuts},
   {"barycenter", (Tcl_CmdProc *)cmdBarycenter},
   {"binx", (Tcl_CmdProc *)cmdBinX},
   {"biny", (Tcl_CmdProc *)cmdBinY},
   {"bitpix", (Tcl_CmdProc *)cmdBitpix},
   {"centro", (Tcl_CmdProc *)cmdAstroPhot},
   {"clear", (Tcl_CmdProc *)cmdClear},
   {"clipmax", (Tcl_CmdProc *)cmdClipmax},
   {"clipmin", (Tcl_CmdProc *)cmdClipmin},
   {"compress", (Tcl_CmdProc *)cmdCompress},
   {"copykwd", (Tcl_CmdProc *)cmdCopyKwd},
   {"copyto", (Tcl_CmdProc *)cmdCopyTo},
   {"create3d", (Tcl_CmdProc *)cmdCreate3d},
   {"div", (Tcl_CmdProc *)cmdTtDiv},
   {"delkwd",(Tcl_CmdProc *)cmdDelKwd},
   {"delkwds",(Tcl_CmdProc *)cmdDelKwds},
   {"extension", (Tcl_CmdProc *)cmdExtension},
   {"fitgauss", (Tcl_CmdProc *)cmdFitGauss},
   {"fitgauss2d", (Tcl_CmdProc *)cmdFitGauss2d},
   {"flux", (Tcl_CmdProc *)cmdAstroPhot},
   {"fwhm", (Tcl_CmdProc *)cmdFwhm},
   {"getkwd", (Tcl_CmdProc *)cmdGetKwd},
   {"getkwds", (Tcl_CmdProc *)cmdGetKwds},
   {"getnaxis",         (Tcl_CmdProc *)cmdGetNaxis},
   {"getpix",           (Tcl_CmdProc *)cmdGetPix},
   {"getpixels",        (Tcl_CmdProc *)cmdGetPixels},
   {"getpixelsheight",  (Tcl_CmdProc *)cmdGetPixelsHeight},
   {"getpixelswidth",   (Tcl_CmdProc *)cmdGetPixelsWidth},
   {"histo", (Tcl_CmdProc *)cmdHistogram},
   {"imaseries", (Tcl_CmdProc *)cmdImaSeries},
   {"imageready", (Tcl_CmdProc *)cmdImageReady},
   {"initialcut", (Tcl_CmdProc *)cmdRestoreInitialCut},
   {"load", (Tcl_CmdProc *)cmdLoadSave},
   {"load3d", (Tcl_CmdProc *)cmdLoad3d},
   {"log", (Tcl_CmdProc *)cmdTtLog},
   {"mergepixels", (Tcl_CmdProc *)cmdMergePixels},
   {"mirrorx", (Tcl_CmdProc *)cmdTtMirrorX},
   {"mirrory", (Tcl_CmdProc *)cmdTtMirrorY},
   {"mult", (Tcl_CmdProc *)cmdMult},
   {"ngain", (Tcl_CmdProc *)cmdTtNGain},
   {"noffset", (Tcl_CmdProc *)cmdTtNOffset},
   {"offset", (Tcl_CmdProc *)cmdTtOffset},
   {"opt", (Tcl_CmdProc *)cmdTtOpt},
   {"phot", (Tcl_CmdProc *)cmdAstroPhot},
   {"photom", (Tcl_CmdProc *)cmdPhotom},
   {"pointer", (Tcl_CmdProc *)cmdPointer},
   {"radec2xy", (Tcl_CmdProc *)cmdRadec2xy},
   {"rot", (Tcl_CmdProc *)cmdTtRot},
   {"save", (Tcl_CmdProc *)cmdLoadSave},
   {"save1d", (Tcl_CmdProc *)cmdSave1d},
   {"save3d", (Tcl_CmdProc *)cmdSave3d},
   {"savefitsrgb", (Tcl_CmdProc *)cmdSaveFitsRGB},
   {"savejpeg", (Tcl_CmdProc *)cmdSaveJpg},
   {"scale", (Tcl_CmdProc *)cmdScale},
   {"scar", (Tcl_CmdProc *)cmdScar},
   {"setkwd", (Tcl_CmdProc *)cmdSetKwd},
   {"setpix", (Tcl_CmdProc *)cmdSetPix},
   {"setpixels", (Tcl_CmdProc *)cmdSetPixels},
   {"sub", (Tcl_CmdProc *)cmdTtSub},
   {"stat", (Tcl_CmdProc *)cmdTtStat},
   {"synthegauss", (Tcl_CmdProc *)cmdGauss},
   {"type", (Tcl_CmdProc *)cmdType},
   {"unsmear", (Tcl_CmdProc *)cmdTtUnsmear},
   {"xy2radec", (Tcl_CmdProc *)cmdXy2radec},
   {"window", (Tcl_CmdProc *)cmdWindow},
   {"A_starlist",(Tcl_CmdProc *)cmdA_StarList},
   {"ubg",(Tcl_CmdProc *)cmdUnifyBg},
   {"region",(Tcl_CmdProc *)cmdRegion},
   {NULL, NULL}
};

//------------------------------------------------------------------------------
// Commande unique pour acceder aux buffers crees
//
int CmdBuf(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char s[2048],ss[50];
   int retour = TCL_OK,k;
   struct cmditem *cmd;

   if(argc==1) {
      sprintf(s,"%s choose sub-command among ",argv[0]);
      k=0;
      while (cmdlist[k].cmd!=NULL) {
	      sprintf(ss," %s",cmdlist[k].cmd);
			strcat(s,ss);
			k++;
		}
      Tcl_SetResult(interp,s,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      for(cmd=cmdlist;cmd->cmd!=NULL;cmd++) {
         if(strcmp(cmd->cmd,argv[1])==0) {
            retour = (*cmd->func)(clientData, interp, argc, argv);
            break;
         }
      }
      if(cmd->cmd==NULL) {
         sprintf(s,"%s %s : sub-command not found among ",argv[0], argv[1]);
         k=0;
		   while (cmdlist[k].cmd!=NULL) {
			   sprintf(ss," %s",cmdlist[k].cmd);
			   strcat(s,ss);
			   k++;
		   }
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }
   return retour;
}

//==============================================================================
// buf$i setkwd kwd --
//   Cree ou remplace le mot cle passe en argument, dans le buffer en question.
//   Le parametre est une liste a 5 elements :
//   * le nom du mot cle
//   * la valeur du mot cle
//   * le type du mot cle : int, float, double, string
//   * le commentaire associe au mot cle
//   * l'unite de la valeur du mot cle
//
int cmdSetKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   int listArgc, retour;
   char **listArgv;
   CBuffer *buffer;
   char *ligne = (char*)calloc(1000,sizeof(char));

   if(argc!=3) {
      sprintf(ligne,"Usage: %s setkwd kwd",argv[0]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Keyword struct not valid: must be { \"keyname\" \"value\" \"type\" \"comment\" \"unit\"");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=5) {
         sprintf(ligne,"Keyword struct not valid: must be { \"keyname\" \"value\" \"type\" \"comment\" \"unit\"");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         buffer = (CBuffer*)clientData;
         try {
            buffer->SetKeyword(listArgv[0],listArgv[1],listArgv[2],listArgv[3],listArgv[4]);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s ",argv[1], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
         Tcl_Free((char*)listArgv);
      }
            
   }
   free(ligne);
   return retour;
}


//==============================================================================
// buf$i copykwd bufNo --
//   Copie les mots-cles non obligatoires depuis bufNo vers le buffer appellant.
//
int cmdCopyKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *bufferFrom, *bufferTo;
   char *ligne = new char[1000];
   int retour;
   int bufNo;


   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s bufNo",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      bufferTo = (CBuffer*)clientData;
      if(Tcl_GetInt(interp,argv[2],&bufNo)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s bufNo",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         bufferFrom = (CBuffer*)buf_pool->Chercher(bufNo);
         if(bufferFrom==NULL) {
            sprintf(ligne,"Origin buffer doesn't exist");
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            try {
               bufferTo->CopyKwdFrom(bufferFrom);
               retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            }
         }
      }
   }
   free(ligne);
   return retour;
}


//==============================================================================
// buf$i getkwd nom --
//   Recupere le contenu du mot cle 'nom', sous forme d'une liste a 5 elements.
//   Si le mot cle n'existe pas, les elements de la liste sont vides. Un lindex
//   permet d'acceder chaque champ independament.
//
int cmdGetKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   CFitsKeywords *keywords;   // Objet de gestion des mots-cles
   CFitsKeyword *kwd;
   char *ligne = (char*)calloc(1000,sizeof(char));
   int retour;

   if(argc!=3) {
      sprintf(ligne,"Usage: %s getkwd keyname",argv[0]);
      retour = TCL_ERROR; 
   } else {
      buffer = (CBuffer*)clientData;
      keywords = (CFitsKeywords*)buffer->GetKeywords();
      if(keywords==NULL) {
         strcpy(ligne,CError::message(ELIBSTD_NO_KWDS));
         retour = TCL_ERROR; 
      } else {
         kwd = keywords->FindKeyword(argv[2]);
         if(kwd==NULL) {
            sprintf(ligne,"{} {} {none} {} {}");
         } else {
            switch(kwd->GetDatatype()) {
               case TINT :
                  sprintf(ligne,"{%s} %d {%s} {%s} {%s}",kwd->GetName(),kwd->GetIntValue(),"int",kwd->GetComment(),kwd->GetUnit());
                  break;
               case TFLOAT :
                  if ((fabs(kwd->GetDoubleValue())<0.1)&&(fabs(kwd->GetDoubleValue())!=0.0)) {
                     sprintf(ligne,"{%s} %e %s {%s} {%s}",kwd->GetName(),kwd->GetDoubleValue(),"float",kwd->GetComment(),kwd->GetUnit());
                  } else {
                     sprintf(ligne,"{%s} %g %s {%s} {%s}",kwd->GetName(),kwd->GetDoubleValue(),"float",kwd->GetComment(),kwd->GetUnit());
                  }
                  break;
               case TDOUBLE :
                  sprintf(ligne,"{%s} %20.15g %s {%s} {%s}",kwd->GetName(),kwd->GetDoubleValue(),"double",kwd->GetComment(),kwd->GetUnit());
                  break;
               case TSTRING :
                  sprintf(ligne,"{%s} {%s} %s {%s} {%s}",kwd->GetName(),kwd->GetStringValue(),"string",kwd->GetComment(),kwd->GetUnit());
                  break;
               default :
                  sprintf(ligne,"{} {} {none} {} {}");
                  break;
            }
         }
         retour = TCL_OK;
      }
   }
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);
   return retour;
}


//==============================================================================
// buf$i delkwd nom --
//   Efface le mot cle 'nom' de la liste des mots cles FITS du buffer.
//
int cmdDelKwd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   int res;
   char *ligne;
   CBuffer *buffer;
   CFitsKeywords *keywords;

   if(argc!=3) {
		ligne = (char*)calloc(1000,sizeof(char));
      sprintf(ligne,"Usage: %s %s keyname",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
		free(ligne);
      return TCL_ERROR;
   }

   buffer = (CBuffer*)clientData;
   keywords = (CFitsKeywords*)buffer->GetKeywords();
   if(keywords==NULL) {
	   Tcl_SetResult(interp,CError::message(ELIBSTD_NO_KWDS),TCL_VOLATILE);
      return TCL_ERROR;
   }

   res = keywords->Delete(argv[2]);
   if(res) {
		Tcl_SetResult(interp,CError::message(res),TCL_VOLATILE);
      return TCL_ERROR;
	}
   return TCL_OK;
}

//==============================================================================
// buf$i delkwds --
//   Efface tous les mots cles FITS du buffer.
//
int cmdDelKwds(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   int res;
   char *ligne;
   CBuffer *buffer;
   CFitsKeywords *keywords;

   if(argc!=2) {
		ligne = (char*)calloc(1000,sizeof(char));
      sprintf(ligne,"Usage: %s %s",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
		free(ligne);
      return TCL_ERROR;
   }

   buffer = (CBuffer*)clientData;
   keywords = (CFitsKeywords*)buffer->GetKeywords();
   if(keywords==NULL) {
	   Tcl_SetResult(interp,CError::message(ELIBSTD_NO_KWDS),TCL_VOLATILE);
      return TCL_ERROR;
   }

   res = keywords->DeleteAll();
   if(res) {
		Tcl_SetResult(interp,CError::message(res),TCL_VOLATILE);
      return TCL_ERROR;
	}
   return TCL_OK;
}


//==============================================================================
// buf$i getkwds --
//   Renvoie la liste des mots cles FITS du buffer en question.
//
int cmdGetKwds(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   int nb_kw, retour, len;
   char *s;
   char *ligne;
   CBuffer *buffer;
   CFitsKeywords *keywords;   // Objet de gestion des mots-cles.
   CFitsKeyword *kwd;               // Mot cle parcourant la liste.

   ligne = new char[1000];

   if(argc!=2) {
      sprintf(ligne,"Usage: %s getkwds",argv[0]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      keywords = (CFitsKeywords*)buffer->GetKeywords();
      if(keywords==NULL) {
         Tcl_SetResult(interp,CError::message(ELIBSTD_NO_KWDS),TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if((nb_kw=keywords->GetKeywordNb())>0) {
         	len = 80*nb_kw;
				s = new char[len];
         	memset(s,0,len);
         	kwd = keywords->GetFirstKeyword();
	         while(kwd) {
   	         strcat(s,"\"");
      	      strcat(s,kwd->GetName());
         	   strcat(s,"\" ");
            	kwd = kwd->next;
	         }
   	      Tcl_SetResult(interp,s,TCL_VOLATILE);
      	   retour = TCL_OK;
	         delete s;
         } else {
   	      Tcl_SetResult(interp,CError::message(ELIBSTD_NO_KWDS),TCL_VOLATILE);
	         retour = TCL_ERROR;
         }
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
/**
 * buf$i imageready --
 *    Fonction permettant de savoir si une image est charg�e dans le buffer
 *
 * Parameters: 
 *    none
 * Results:
 *    returns 1 if pixels are ready, otherwise 0.
 * Side effects:
 *    none
 */

int cmdImageReady(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   ligne = new char[1000];

   if(argc!=2) {
      sprintf(ligne,"Usage: %s %s ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      try {
         if ( buffer->IsPixelsReady() == 1 ) {
               sprintf(ligne,"1");
         } else {
               sprintf(ligne,"0");
         }
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i type --
//   Fonction permettant de connaitre le type de donnees contenues dans l'image
//
int cmdType(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   TDataType dt;
   ligne = new char[1000];

   if(argc!=2) {
      sprintf(ligne,"Usage: %s %s ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      try {
         buffer->GetDataType(&dt);
         switch(dt) {
            case dt_Short :
               sprintf(ligne,"short");
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case dt_Float :
               sprintf(ligne,"float");
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_OK;
               break;
            default :
               sprintf(ligne,"unknown type (might never occur)");
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
         }
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }
   delete ligne;
   return retour;
}


//==============================================================================
// buf$i bitpix ?byte|short|long|float|double? --
//   Fixe le format d'�criture du fichier FITS sur disque. En cas de valeur
//   incorrecte, utilise des types short.
//
int cmdBitpix(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   ligne = new char[1000];

   if((argc!=2)&&(argc!=3)) {
      sprintf(ligne,"Usage: %s %s ?byte|short|ushort|long|ulong|float|double?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      if(argc==2) {
         switch(buffer->GetSavingType()) {
            case BYTE_IMG:
               Tcl_SetResult(interp,"byte",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case SHORT_IMG:
               Tcl_SetResult(interp,"short",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case USHORT_IMG:
               Tcl_SetResult(interp,"ushort",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case LONG_IMG:
               Tcl_SetResult(interp,"long",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case ULONG_IMG:
               Tcl_SetResult(interp,"ulong",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case FLOAT_IMG:
               Tcl_SetResult(interp,"float",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case DOUBLE_IMG:
               Tcl_SetResult(interp,"double",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            default:
               Tcl_SetResult(interp,"internal error",TCL_VOLATILE);
               retour = TCL_ERROR;
               break;
         }
      } else {
         if((strcmp(argv[2],"byte")==0)||(strcmp(argv[2],"8")==0)) {
            buffer->SetSavingType(BYTE_IMG);
            retour = TCL_OK;
         } else if((strcmp(argv[2],"short")==0)||(strcmp(argv[2],"16")==0)) {
            buffer->SetSavingType(SHORT_IMG);
            retour = TCL_OK;
         } else if((strcmp(argv[2],"ushort")==0)||(strcmp(argv[2],"+16")==0)) {
            buffer->SetSavingType(USHORT_IMG);
            retour = TCL_OK;
         } else if((strcmp(argv[2],"long")==0)||(strcmp(argv[2],"32")==0)) {
            buffer->SetSavingType(LONG_IMG);
            retour = TCL_OK;
         } else if((strcmp(argv[2],"ulong")==0)||(strcmp(argv[2],"+32")==0)) {
            buffer->SetSavingType(ULONG_IMG);
            retour = TCL_OK;
         } else if((strcmp(argv[2],"float")==0)||(strcmp(argv[2],"-32")==0)) {
            buffer->SetSavingType(FLOAT_IMG);
            retour = TCL_OK;
         } else if((strcmp(argv[2],"double")==0)||(strcmp(argv[2],"-64")==0)) {
            buffer->SetSavingType(DOUBLE_IMG);
            retour = TCL_OK;
         } else {
            sprintf(ligne,"Usage: %s %s ?byte|short|ushort|long|ulong|float|double?",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i compress ?method?
//   Fixe le mode de compression de la sauvegarde FITS sur disque
//   none|gzip
//   En cas de valeur incorrecte, utilise none.
//
int cmdCompress(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   ligne = new char[1000];

   if((argc!=2)&&(argc!=3)) {
      sprintf(ligne,"Usage: %s %s ?none|gzip?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      if(argc==2) {
         switch(buffer->GetCompressType()) {
            case BUFCOMPRESS_NONE:
               Tcl_SetResult(interp,"none",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            case BUFCOMPRESS_GZIP:
               Tcl_SetResult(interp,"gzip",TCL_VOLATILE);
               retour = TCL_OK;
               break;
            default:
               Tcl_SetResult(interp,"internal error",TCL_VOLATILE);
               retour = TCL_ERROR;
               break;
         }
      } else {
         if((strcmp(argv[2],"none")==0)||(strcmp(argv[2],"0")==0)) {
            buffer->SetCompressType(BUFCOMPRESS_NONE);
            retour = TCL_OK;
         } else if((strcmp(argv[2],"gzip")==0)||(strcmp(argv[2],"1")==0)) {
            buffer->SetCompressType(BUFCOMPRESS_GZIP);
            retour = TCL_OK;
         } else {
            sprintf(ligne,"Usage: %s %s ?none|gzip?",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i extension ?file_extension?
//   Fixe le nom de l'externsion par d�faut des fichiers
//   .fit par defaut
//
int cmdExtension(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   ligne = new char[1000];

   if((argc!=2)&&(argc!=3)) {
      sprintf(ligne,"Usage: %s %s ?file_extension?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      if(argc==2) {
          Tcl_SetResult(interp,buffer->GetExtension(),TCL_VOLATILE);
          retour = TCL_OK;
      } else {
          buffer->SetExtension(argv[2]);
          Tcl_SetResult(interp,buffer->GetExtension(),TCL_VOLATILE);
          retour = TCL_OK;
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i load filename --
//   Chargement d'une image FITS a partir du disque. Le contenu du buffer est
//   ecrase s'il etait non vide.
// buf$i save filename --
//   Enregistrement d'une image FITS sur disque. Si le contenu du buffer est
//   vide, rien n'est fait.
//
int cmdLoadSave(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *name     = NULL;
   char *extfits  = NULL;
   char *path2    = NULL;
   char *nom_fichier = NULL;
   char *ext      = NULL;
   char *ligne    = NULL;
   int retour;
   CBuffer *Buffer;
   
   ligne = (char*)malloc(1000);
   
   if(argc<3) {
      sprintf(ligne,"Usage: %s %s filename ?fits|gif|bmp|jpeg|tiff? ?options? ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      name = (char*)calloc(512,sizeof(char));
      extfits = (char*)calloc(128,sizeof(char));
      ext = (char*)calloc(128,sizeof(char));
      path2 = (char*)calloc(256,sizeof(char));
      nom_fichier = (char*)calloc(1000,sizeof(char));
      Buffer = (CBuffer*)clientData;
      
      // Decodage du nom de fichier : chemin, nom du fichier, etc.
      // "encoding convertfrom identity" sert a traiter coorectement les caracteres accentues
      sprintf(ligne,"file dirname [encoding convertfrom identity {%s}]",argv[2]); Tcl_Eval(interp,ligne); strcpy(path2,interp->result);
      sprintf(ligne,"file tail [encoding convertfrom identity {%s}]",argv[2]); Tcl_Eval(interp,ligne); strcpy(name,interp->result);
      sprintf(ligne,"file extension \"%s\"",argv[2]); Tcl_Eval(interp,ligne);
      if(strcmp(interp->result, "")==0) {
         // set default extension
         strcpy(extfits,Buffer->GetExtension()); 
         strcpy(ext,Buffer->GetExtension()); 
      } 
      else {
         strcpy(extfits,"");
         strcpy(ext,interp->result); 
         // je supprime le suffixe de l'extension (numero d'image d'un fichier fit en 3 dimensions
         sprintf(ligne,"string tolower [lindex [split \"%s\" \";\"] 0]",ext); 
         Tcl_Eval(interp,ligne);
         strcpy(ext,interp->result); 
      }
      sprintf(ligne,"file join {%s} {%s%s}",path2,name,extfits); 
      Tcl_Eval(interp,ligne); 
      strcpy(nom_fichier,interp->result);
      
      try {
         if( strcmp(ext, ".bmp")== 0 
            || strcmp(ext, ".gif")== 0 
            || strcmp(ext, ".jpg")== 0 
            || strcmp(ext, ".jpeg")== 0 
            || strcmp(ext, ".png")== 0 
            || strcmp(ext, ".ps")== 0 
            || strcmp(ext, ".eps")== 0 
            || strcmp(ext, ".tiff")== 0 
            || strcmp(ext, ".tif")== 0 
            || strcmp(ext, ".xbm")== 0 
            || strcmp(ext, ".xpm")== 0 ) {   
            // je verifie la pr�sence de la libriairie Img 1.3
            sprintf(ligne,"package require Img 1.3");             
            if( Tcl_Eval(interp,ligne) != TCL_OK  ) {
               throw CError("Missing library Img 1.3 for loading %s (%s)",nom_fichier,interp->result);
            }     
            
            if(strcmp(argv[1],"save")==0) {
               //--- SAVE standard format image (BMP, JPEG, GIF, PNG, PS, TIFF, XPN, XBN)
               Tk_PhotoImageBlock pib;
               char tkImgfileFormat[10];
               Tk_PhotoHandle ph ;
               
               // je cree une image tk temporaire.
               sprintf(ligne,"image create photo imageload"); 
               if ( Tcl_Eval(interp,ligne) != TCL_OK) {
                  throw CError("Error creating imageload when loading/saving %s (%)",nom_fichier, interp->result );
               }

               // je recupere le handle de l'image tk
               ph = Tk_FindPhoto(interp,"imageload");
               if(ph==NULL) throw CError( ELIBSTD_NO_TKPHOTO_HANDLE);
               
               // je convertis l'extension en minuscule
               sprintf(ligne,"string tolower \"%s\" ",ext); Tcl_Eval(interp,ligne);
               strcpy(ext, interp->result);          
               
               // j'identifie le format de la sauvegarde � partir de l'extension du fichier
               strcpy(tkImgfileFormat,"");
               if(strcmp(ext, ".bmp" )==0)   strcpy(tkImgfileFormat, "bmp");
               if(strcmp(ext, ".gif" )==0)   strcpy(tkImgfileFormat, "gif");
               if(strcmp(ext, ".jpg" )==0)   strcpy(tkImgfileFormat, "jpeg");
               if(strcmp(ext, ".jpeg")==0)   strcpy(tkImgfileFormat, "jpeg");
               if(strcmp(ext, ".png" )==0)   strcpy(tkImgfileFormat, "png");
               if(strcmp(ext, ".ps"  )==0)   strcpy(tkImgfileFormat, "ps");
               if(strcmp(ext, ".eps" )==0)   strcpy(tkImgfileFormat, "ps");
               if(strcmp(ext, ".tiff")==0)   strcpy(tkImgfileFormat, "tiff");
               if(strcmp(ext, ".tif" )==0)   strcpy(tkImgfileFormat, "tiff");
               if(strcmp(ext, ".xbm" )==0)   strcpy(tkImgfileFormat, "xbm");
               if(strcmp(ext, ".xpm" )==0)   strcpy(tkImgfileFormat, "xpm");
               
               if( strcmp(tkImgfileFormat,"" )==0) {
                  throw CError("Usage: %s %s filename  ?options? \nfile extension must be bmp|fits|gif|jpg|png|tif|xbm|xpm",argv[0],argv[1]);
               }
               
               // j"extrait les pixels du buffer                 
               Buffer->SaveNoFits(&pib.pixelSize, pib.offset, &pib.pitch, &pib.width, &pib.height, &pib.pixelPtr  );                  
               // je copie les pixels dans l'image tk temporaire
               Tk_PhotoPutBlock(ph,&pib,0,0,pib.width,pib.height);
               free(pib.pixelPtr);                  
               // j'enregistre l'image dans le fichier en utilisant la librairie tkimg
               sprintf(ligne,"imageload write \"%s\" -format %s ", nom_fichier, tkImgfileFormat); 
               if( Tcl_Eval(interp,ligne) != TCL_OK ) 
                  throw CError("Error when saving %s (%s)",nom_fichier, interp->result );
               retour = TCL_OK;               
            } else {
               //--- LOAD standard format image (BMP, JPEG, GIF, PNG, PS, TIFF, XPN, XBN)
               Tk_PhotoImageBlock pib;
               Tk_PhotoHandle ph ;

               // je cree une image tk temporaire.
               sprintf(ligne,"image create photo imageload"); 
               if ( Tcl_Eval(interp,ligne) != TCL_OK) {
                  throw CError("Error creating imageload when loading/saving %s (%)",nom_fichier, interp->result );
               }
               // je recupere le handle de l'image tk
               ph = Tk_FindPhoto(interp,"imageload");
               if(ph==NULL) throw CError( ELIBSTD_NO_TKPHOTO_HANDLE);
               
               // j'ajoute des guillements pour traiter les fichiers dont le nom contient un espace
               sprintf(ligne,"imageload configure -file \"%s\" ", nom_fichier); 
               if( Tcl_Eval(interp,ligne) != TCL_OK ) {
                  sprintf(ligne,"image delete imageload"); 
                  Tcl_Eval(interp,ligne);
                  throw CError("Error when loading %s (%s)",nom_fichier, interp->result );
               } else {
                  // je charge les pixels dans l'image temporaire avec la librairie tkImg 
                  retour = Tk_PhotoGetImage(ph,&pib);            
                  if( pib.pixelPtr == NULL) {
                     sprintf(ligne,"image delete imageload"); 
                     Tcl_Eval(interp,ligne);
                     throw CError("Error Tk_PhotoGetImage when loading %s",nom_fichier);
                  } else {
                     // je copie le pixels de l'image tk temporaire dans le buffer
                     Buffer->LoadNoFits(pib.pixelSize, pib.offset, pib.pitch ,pib.width, pib.height, pib.pixelPtr);
                  }                              
               }
            }
            // je supprime l'image tk temporaire
            sprintf(ligne,"image delete imageload"); 
            Tcl_Eval(interp,ligne);
         } else if( strcmp(ext, ".crw")== 0      // canon raw image
                    || strcmp(ext, ".nef")== 0   // nikon raw image
                    || strcmp(ext, ".cr2")== 0   // nikon raw image
                    || strcmp(ext, ".dng")== 0   // nikon raw image
                  ) {  
            //--- RAW file (crw, nef, cr2 , dng )
            if(strcmp(argv[1],"save")==0) {
               //--- SAVE RAW file
               Buffer->SaveRawFile(nom_fichier);
               retour = TCL_OK;
            } else {
               //--- LOAD RAW file
               Buffer->LoadRawFile(nom_fichier);
               retour = TCL_OK;
            }
                     
         } else {
            //--- FITS file (fit, fits, fts, fit.gz, fits.gz, fts.gz or default extension)
            if(strcmp(argv[1],"save")==0) {
               //--- save FITS file
               Buffer->SaveFits(nom_fichier);
               // compression eventuelle du fichier
               if (Buffer->GetCompressType()==BUFCOMPRESS_GZIP) {
                  sprintf(ligne,"catch {file delete %s.gz}",nom_fichier); Tcl_Eval(interp,ligne);
                  sprintf(ligne,"gzip %s",nom_fichier); 
                  Tcl_Eval(interp,ligne);
                  if( Tcl_Eval(interp,ligne) != TCL_OK  ) {
                     throw CError("gzip %s",interp->result);
                  }     

               }
               retour = TCL_OK;
            } else {
               //--- Load FITS file
               Buffer->LoadFits(nom_fichier);
               retour = TCL_OK;
            }
         }
         
         Tcl_SetResult(interp,"",TCL_VOLATILE);
         retour = TCL_OK;         
         free(name);
         free(extfits);
         free(ext);
         free(path2);
         free(nom_fichier);
         free(ligne);
      } catch (const CError& e) {
         // je supprime l'image temporaire 
         sprintf(ligne,"catch { image delete imageload }"); 
         Tcl_Eval(interp,ligne);

         // je complete le message de l'erreur
         sprintf(ligne,"%s %s \n%s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);

         // je libere les variables allou�es dynamiquement
         if (name)         free(name);
         if (extfits)      free(extfits);
         if (ext)          free(ext);
         if (path2)        free(path2);
         if (nom_fichier) free(nom_fichier);
         if (ligne)        free(ligne);
         retour = TCL_ERROR;
      }
   }
   return retour;
}


//==============================================================================
// buf$i load3d filename iaxis3 --
//   Chargement d'une image FITS a partir du disque. Le contenu du buffer est
//   ecrase s'il etait non vide.
//
int cmdLoad3d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *name, *ext, *path2, *nom_fichier;
   char *ligne;
   int retour;
   CBuffer *Buffer;
   int iaxis3;

   ligne = new char[1000];

   if(argc<=2) {
      sprintf(ligne,"Usage: %s %s filename ?iaxis3?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      name = (char*)calloc(512,sizeof(char));
      ext = (char*)calloc(128,sizeof(char));
      path2 = (char*)calloc(256,sizeof(char));
      nom_fichier = (char*)calloc(1000,sizeof(char));
      Buffer = (CBuffer*)clientData;

      // Decodage du nom de fichier : chemin, nom du fichier, etc.
      sprintf(ligne,"file dirname {%s}",argv[2]); Tcl_Eval(interp,ligne); strcpy(path2,interp->result);
      sprintf(ligne,"file tail {%s}",argv[2]); Tcl_Eval(interp,ligne); strcpy(name,interp->result);
      sprintf(ligne,"file extension \"%s\"",argv[2]); Tcl_Eval(interp,ligne);
      if(strcmp(interp->result,"")==0) strcpy(ext,Buffer->GetExtension()); else strcpy(ext,"");
      sprintf(ligne,"file join {%s} {%s%s}",path2,name,ext); Tcl_Eval(interp,ligne); strcpy(nom_fichier,interp->result);

      iaxis3=0;
      if (argc>=4) {
         iaxis3=(int)atoi(argv[3]);
      }
      try {
         Buffer->Load3d(nom_fichier,iaxis3);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }


      free(name);
      free(ext);
      free(path2);
      free(nom_fichier);
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i create3d listfilename --
//   Chargement d'images FITS a partir du disque. Le contenu du buffer est
//   ecrase s'il etait non vide.
//
int cmdCreate3d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *name, *ext, *path2, *nom_fichier;
   char *ligne;
   int retour;
   CBuffer *Buffer;
   char **argvv=NULL;
   int argcc,naxis3,k,init,errcode;
   int naxis1,naxis2;

   ligne = new char[1000];

   if(argc<=2) {
      sprintf(ligne,"Usage: %s %s listfilename",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Decodage de la liste des fichiers
      if (Tcl_SplitList(interp,argv[2],&argcc,&argvv)==TCL_ERROR) {
         sprintf(ligne,"Problem when decoding listfilename %s",argv[2]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         return TCL_ERROR;
      }

      naxis3=argcc;

      for (k=0;k<naxis3;k++) {

         name = (char*)calloc(512,sizeof(char));
         ext = (char*)calloc(128,sizeof(char));
         path2 = (char*)calloc(256,sizeof(char));
         nom_fichier = (char*)calloc(1000,sizeof(char));
         Buffer = (CBuffer*)clientData;

         // Decodage du nom de fichier : chemin, nom du fichier, etc.
         sprintf(ligne,"file dirname {%s}",argvv[k]); Tcl_Eval(interp,ligne); strcpy(path2,interp->result);
         sprintf(ligne,"file tail {%s}",argvv[k]); Tcl_Eval(interp,ligne); strcpy(name,interp->result);
         sprintf(ligne,"file extension \"%s\"",argvv[k]); Tcl_Eval(interp,ligne);
         if(strcmp(interp->result,"")==0) strcpy(ext,Buffer->GetExtension()); else strcpy(ext,"");
         sprintf(ligne,"file join {%s} {%s%s}",path2,name,ext); Tcl_Eval(interp,ligne); strcpy(nom_fichier,interp->result);

         try {
            if (k==0) {
               init=1;
               naxis1=0;
               naxis2=0;
            } else {
               init=0;
            }
            Buffer->Create3d(nom_fichier,init,naxis3,k,&naxis1,&naxis2,&errcode);
            if (errcode>0) {
               sprintf(ligne,"Error code %d ",errcode);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
               free(name);
               free(ext);
               free(path2);
               free(nom_fichier);
               break;
            } else {
               retour = TCL_OK;
            }
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
            free(name);
            free(ext);
            free(path2);
            free(nom_fichier);
            break;
         }

         free(name);
         free(ext);
         free(path2);
         free(nom_fichier);
      }
   }
   Tcl_Free((char *) argvv);

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i save1d filename ?iaxis2? --
//   Enregistrement d'une image FITS sur disque. Le contenu du buffer est
//   decoupe en un seul axe.
//
int cmdSave1d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *name, *ext, *path2, *nom_fichier;
   char *ligne;
   int retour;
   CBuffer *Buffer;
   int iaxis2;

   ligne = new char[1000];

   if(argc<=2) {
      sprintf(ligne,"Usage: %s %s filename ?iaxis2?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      name = (char*)calloc(512,sizeof(char));
      ext = (char*)calloc(128,sizeof(char));
      path2 = (char*)calloc(256,sizeof(char));
      nom_fichier = (char*)calloc(1000,sizeof(char));
      Buffer = (CBuffer*)clientData;

      // Decodage du nom de fichier : chemin, nom du fichier, etc.
      sprintf(ligne,"file dirname {%s}",argv[2]); Tcl_Eval(interp,ligne); strcpy(path2,interp->result);
      sprintf(ligne,"file tail {%s}",argv[2]); Tcl_Eval(interp,ligne); strcpy(name,interp->result);
      sprintf(ligne,"file extension \"%s\"",argv[2]); Tcl_Eval(interp,ligne);
      if(strcmp(interp->result,"")==0) strcpy(ext,Buffer->GetExtension()); else strcpy(ext,"");
      sprintf(ligne,"file join {%s} {%s%s}",path2,name,ext); Tcl_Eval(interp,ligne); strcpy(nom_fichier,interp->result);

      iaxis2=0;
      if (argc>=4) {
         iaxis2=(int)atoi(argv[3])-1;
         if (iaxis2<0) { iaxis2=0; }
      }
      try {
         Buffer->Save1d(nom_fichier,iaxis2);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }

        // compression eventuelle du fichier
        if (Buffer->GetCompressType()==BUFCOMPRESS_GZIP) {
            sprintf(ligne,"catch {file delete %s.gz}",nom_fichier); Tcl_Eval(interp,ligne);
            sprintf(ligne,"catch {gzip %s}",nom_fichier); Tcl_Eval(interp,ligne);
       	}

      free(name);
      free(ext);
      free(path2);
      free(nom_fichier);
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i save3d filename ?naxis3? ?iaxis3_beg iaxis3_end? --
//   Enregistrement d'une image FITS sur disque. Le contenu du buffer est
//   decoupe en trois axes.
//
int cmdSave3d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *name, *ext, *path2, *nom_fichier;
   char *ligne;
   int retour;
   CBuffer *Buffer;
   int naxis3,iaxis3_beg,iaxis3_end,dummy;

   ligne = new char[1000];

   if(argc<=2) {
      sprintf(ligne,"Usage: %s %s filename ?naxis3? ?iaxis3_beg iaxis3_end?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      name = (char*)calloc(512,sizeof(char));
      ext = (char*)calloc(128,sizeof(char));
      path2 = (char*)calloc(256,sizeof(char));
      nom_fichier = (char*)calloc(1000,sizeof(char));
      Buffer = (CBuffer*)clientData;

      // Decodage du nom de fichier : chemin, nom du fichier, etc.
      sprintf(ligne,"file dirname {%s}",argv[2]); Tcl_Eval(interp,ligne); strcpy(path2,interp->result);
      sprintf(ligne,"file tail {%s}",argv[2]); Tcl_Eval(interp,ligne); strcpy(name,interp->result);
      sprintf(ligne,"file extension \"%s\"",argv[2]); Tcl_Eval(interp,ligne);
      if(strcmp(interp->result,"")==0) strcpy(ext,Buffer->GetExtension()); else strcpy(ext,"");
      sprintf(ligne,"file join {%s} {%s%s}",path2,name,ext); Tcl_Eval(interp,ligne); strcpy(nom_fichier,interp->result);

      naxis3=1;
      if (argc>=4) {
         naxis3=(int)atoi(argv[3]);
         if (naxis3<0) { naxis3=1; }
      }
      iaxis3_beg=1;
      iaxis3_end=naxis3;
      if ((argc>=6)&&(naxis3>0)) {
         iaxis3_beg=(int)atoi(argv[4]);
         iaxis3_end=(int)atoi(argv[5]);
         if (iaxis3_end<iaxis3_beg) { dummy=iaxis3_beg; iaxis3_beg=iaxis3_end; iaxis3_end=dummy;}
         if (iaxis3_beg<1) { iaxis3_beg=1; }
         if (iaxis3_end>naxis3) { iaxis3_end=naxis3; }
      }
      try {
         Buffer->Save3d(nom_fichier,naxis3,iaxis3_beg,iaxis3_end);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }

        // compression eventuelle du fichier
        if (Buffer->GetCompressType()==BUFCOMPRESS_GZIP) {
            sprintf(ligne,"catch {file delete %s.gz}",nom_fichier); Tcl_Eval(interp,ligne);
            sprintf(ligne,"catch {gzip %s}",nom_fichier); Tcl_Eval(interp,ligne);
       	}

      free(name);
      free(ext);
      free(path2);
      free(nom_fichier);
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i savefitsrgb filename --
//   Enregistrement d'une image FITS RGB . Si le contenu du buffer est
//   vide, rien n'est fait.
//
int cmdSaveFitsRGB(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *name, *ext, *path2, *nom_fichier;
   char *ligne;
   int retour;
   CBuffer *Buffer;

   ligne = new char[1000];

   if ((argc<=2)||(argc>=7)) {
      sprintf(ligne,"Usage: %s %s filename ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      name = (char*)calloc(512,sizeof(char));
      ext = (char*)calloc(128,sizeof(char));
      path2 = (char*)calloc(256,sizeof(char));
      nom_fichier = (char*)calloc(1000,sizeof(char));
      Buffer = (CBuffer*)clientData;

      // Decodage du nom de fichier : chemin, nom du fichier, etc.
      sprintf(ligne,"file dirname [encoding convertfrom identity {%s}]",argv[2]); Tcl_Eval(interp,ligne); strcpy(path2,interp->result);
      sprintf(ligne,"file tail [encoding convertfrom identity {%s}]",argv[2]); Tcl_Eval(interp,ligne); strcpy(name,interp->result);
      sprintf(ligne,"file extension \"%s\"",argv[2]); Tcl_Eval(interp,ligne);
      if(strcmp(interp->result,"")==0) strcpy(ext,".jpg"); else strcpy(ext,"");
      sprintf(ligne,"file join {%s} {%s%s}",path2,name,ext); Tcl_Eval(interp,ligne); strcpy(nom_fichier,interp->result);

      try {
         Buffer->SaveFitsRGB(nom_fichier);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }

      free(name);
      free(ext);
      free(path2);
      free(nom_fichier);
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i savejpeg filename --
//   Enregistrement d'une image Jpeg sur disque. Si le contenu du buffer est
//   vide, rien n'est fait.
//
int cmdSaveJpg(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *name, *ext, *path2, *nom_fichier;
   char *ligne;
   int retour;
   CBuffer *Buffer;
   int quality,sbsh;
   double sb,sh;

   ligne = new char[1000];

   if ((argc<=2)||(argc>=7)) {
      sprintf(ligne,"Usage: %s %s filename ?quality? ?locut? ?hicut?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      name = (char*)calloc(512,sizeof(char));
      ext = (char*)calloc(128,sizeof(char));
      path2 = (char*)calloc(256,sizeof(char));
      nom_fichier = (char*)calloc(1000,sizeof(char));
      Buffer = (CBuffer*)clientData;

      // Decodage du nom de fichier : chemin, nom du fichier, etc.
      sprintf(ligne,"file dirname [encoding convertfrom identity {%s}]",argv[2]); Tcl_Eval(interp,ligne); strcpy(path2,interp->result);
      sprintf(ligne,"file tail [encoding convertfrom identity {%s}]",argv[2]); Tcl_Eval(interp,ligne); strcpy(name,interp->result);
      sprintf(ligne,"file extension \"%s\"",argv[2]); Tcl_Eval(interp,ligne);
      if(strcmp(interp->result,"")==0) strcpy(ext,".jpg"); else strcpy(ext,"");
      sprintf(ligne,"file join {%s} {%s%s}",path2,name,ext); Tcl_Eval(interp,ligne); strcpy(nom_fichier,interp->result);

      quality=75;
      if (argc>=4) {
         quality=(int)(fabs(atof(argv[3])));
      }
      sb=0.0;
      if (argc>=5) {
         sb=(double)atof(argv[4]);
      }
      sbsh=0;
      sh=1.0;
      if (argc>=6) {
         sh=(double)atof(argv[5]);
         sbsh=1;
      }
      if (quality<5) {quality=5;}
      if (quality>100) {quality=100;}

      try {
         Buffer->SaveJpg(nom_fichier,quality,sbsh,sb,sh);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }

      free(name);
      free(ext);
      free(path2);
      free(nom_fichier);
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i copyto dest
//   Fonction permettant de copier un buffer vers un autre. Il y a duplicatio
//   des pixels, et de l'entete FITS.
//
int cmdCopyTo(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   CBuffer *buffer_dest;
   char *ligne;
   int dest, retour;

   ligne = new char[1000];

   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s dest",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetInt(interp,argv[2],&dest)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s dest\ndest = must be an integer > 0",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         buffer = (CBuffer*)clientData;
         buffer_dest = (CBuffer*)buf_pool->Chercher(dest);
         if(buffer_dest==NULL) {
            sprintf(ligne,"::buf::create %d",dest);
            Tcl_Eval(interp,ligne);
            buffer_dest = (CBuffer*)buf_pool->Chercher(dest);
         }
         try {
            buffer->CopyTo(buffer_dest);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i clear --
//   Fonction permettant de supprimer le contenu du buffer, sans pour autant
//   l'effacer de la liste des buffers. L'entete et les pixels sont perdus car
//   la memoire associee est liberee.
//
int cmdClear(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;

   if(argc!=2) {
      char *ligne;
      ligne = (char*)calloc(1000,sizeof(char));
      sprintf(ligne,"Usage: %s %s",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      free(ligne);
   } else {
      buffer = (CBuffer*)clientData;
      buffer->FreeBuffer(DONT_KEEP_KEYWORDS);
   }
   return TCL_OK;
}


//==============================================================================
// buf$i getpix {x y} --
//   Fonction permettant de recuperer la valeur de l'image contenue dans le
//   buffer aux coordonnees (x,y). Ces coordonnees sont passees a la fonction
//   sous forme d'une liste a deux elements.
//
int cmdGetPix(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   int naxis1, naxis2, x, y; // Dimensions et coordonnees.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   TYPE_PIXELS val1,val2,val3; // Valeur du pixel des coordonnees specifiees.
   TDataType dt;

   ligne = (char*)calloc(1000,sizeof(char));
   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s {x y}",argv[0],argv[1]);
      retour = TCL_ERROR;
   } else {
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Position struct not valid: must be {x y}");
         retour = TCL_ERROR;
      } else if(listArgc!=2) {
         sprintf(ligne,"Position struct not valid: must be {x y}");
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x y}\nx = must be an integer",argv[0],argv[1]);
            retour = TCL_ERROR;
        } else {
            if(Tcl_GetInt(interp,listArgv[1],&y)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {x y}\ny = must be an integer",argv[0],argv[1]);
               retour = TCL_ERROR;
           } else {
               buffer = (CBuffer*)clientData;
               if(buffer==NULL) {
                  sprintf(ligne,"Buffer is NULL : abnormal error.");
                  retour = TCL_ERROR;
               } else {
                  int plane =0;
                  naxis1 = buffer->GetW();
                  naxis2 = buffer->GetH();
                  // Test sur la position du point par rapport aux coins.
                  if( buffer->GetNaxis() == 1  ) {
                     if( x<=0||x>naxis1) {
                        sprintf(ligne,"Out of limits point ((%d,%d) in (1,1),(%d,%d)).",x,y,naxis1,naxis2);
                        retour = TCL_ERROR;
                     } else {
                        x -=1;
                        y = 0;
                        retour = TCL_OK;
                     }
                  } else {
                     if(x<=0||x>naxis1||y<=0||y>naxis2) {
                        sprintf(ligne,"Out of limits point ((%d,%d) in (1,1),(%d,%d)).",x,y,naxis1,naxis2);
                        retour = TCL_ERROR;
                     } else { 
                        // Changement de repere
                        x -= 1;
                        y -= 1;
                        retour = TCL_OK;
                     }
                  }
                  
                  if ( retour == TCL_OK) {
                     try { 
                        buffer->GetPix(&plane,&val1,&val2,&val3,x,y);
                        buffer->GetDataType(&dt);
                     switch(dt) {
                        case dt_Float :
                           if( plane == 1 ) {
                              sprintf(ligne,"1 %f",val1);
                           } else  {
                              sprintf(ligne,"3 %f %f %f",val1, val2, val3);
                           }
                           break;
                        default:
                        case dt_Short :
                           if( plane == 1 ) {
                              sprintf(ligne,"1 %f",val1);
                           } else  {
                              sprintf(ligne,"3 %d %d %d", (short)val1, (short)val2, (short) val3);
                           }
                           break;
                     }
                        retour = TCL_OK;
                     } catch(const CError& e) {
                        sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
                        Tcl_SetResult(interp,ligne,TCL_VOLATILE);
                        retour = TCL_ERROR;
                     }
                  }
               }
            }
         }
         Tcl_Free((char*)listArgv);
      }
   }
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);

   return retour;
}


//==============================================================================
// buf$i setpix val {x y} --
//
int cmdSetPix(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   int naxis1, naxis2, x, y; // Dimensions et coordonnees.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour = TCL_OK;               // Code d'erreur de retour.
   double val;
   double valred, valgreen, valblue;

   ligne = (char*)calloc(1000,sizeof(char));
   if(argc!=4 && argc != 6) {
      sprintf(ligne,"Usage: %s %s {x y} [valgray | [valred valgreen valblue] ]",argv[0],argv[1]);
      retour = TCL_ERROR;
   } else {
      if( (argc == 4 && Tcl_GetDouble(interp,argv[3],&val)!=TCL_OK)
          ||
          //(argc == 6 && (Tcl_GetDouble(interp,argv[4],&valgreen)!=TCL_OK) && (Tcl_GetDouble(interp,argv[3],&valred) !=TCL_OK) && (Tcl_GetDouble(interp,argv[5],&valblue) !=TCL_OK) )
          (argc == 6 && (Tcl_GetDouble(interp,argv[4],&valgreen)!=TCL_OK))
         ) {
         sprintf(ligne,"Usage: %s %s {x y} val\nval must be a numerical value",argv[0],argv[1]);
         retour = TCL_ERROR;
      } else if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Position struct not valid: must be {x y}");
         retour = TCL_ERROR;
      } else if(listArgc!=2) {
         sprintf(ligne,"Position struct not valid: must be {x y}");
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x y} val\nx = must be an integer",argv[0],argv[1]);
            retour = TCL_ERROR;
        } else {
            if(Tcl_GetInt(interp,listArgv[1],&y)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {x y} val\ny = must be an integer",argv[0],argv[1]);
               retour = TCL_ERROR;
           } else {
               buffer = (CBuffer*)clientData;
               if(buffer==NULL) {
                  sprintf(ligne,"Buffer is NULL : abnormal error.");
                  retour = TCL_ERROR;
               } else {
                  naxis1 = buffer->GetW();
                  naxis2 = buffer->GetH();
                  // Test sur la position du point par rapport aux coins.
                  if((x<=0)||(x>naxis1)||(y<=0)||(y>naxis2)) {
                     sprintf(ligne,"Out of limits point ((%d,%d) in (1,1),(%d,%d)).",x,y,naxis1,naxis2);
                     retour = TCL_ERROR;
                  } else {
                     // Changement de repere
                     x -= 1;
                     y -= 1;
                     try {
                        if ( argc == 4 ) {
                           buffer->SetPix(PLANE_GREY, (TYPE_PIXELS)val,x,y);
                           retour = TCL_OK;
                        } else {
                           Tcl_GetDouble(interp,argv[3],&valred);
                           Tcl_GetDouble(interp,argv[5],&valblue);
                           buffer->SetPix(PLANE_R, (TYPE_PIXELS)valred,x,y);
                           buffer->SetPix(PLANE_G, (TYPE_PIXELS)valgreen,x,y);
                           buffer->SetPix(PLANE_B, (TYPE_PIXELS)valblue,x,y);
                        }                          
                     } catch(const CError& e) {
                        sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
                        Tcl_SetResult(interp,ligne,TCL_VOLATILE);
                        retour = TCL_ERROR;
                     }

                  }
               }
            }
         }
         Tcl_Free((char*)listArgv);
      }
   }
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);

   return retour;
}

//==============================================================================
// buf$i getpixels pixelsPtr 
//
// retourne une copie du tableau de pixels (format float, non compress�)
// l'appelant doit avoir r�serv� l'espace m�moire dans lequel seront retourn�s les pixels
//
int cmdGetPixels(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int pixels;
   TColorPlane colorPlane; 

   ligne = (char*)calloc(1000,sizeof(char));
   if(argc < 3  || argc > 4) {
      sprintf(ligne,"Usage: %s %s pixelsPtr [PLANE_GREY|PLANE_RGB|PLANE_R|PLANE_G|PLANE_B]",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      free(ligne);
      return TCL_ERROR;
   } else {    
      if(Tcl_GetInt(interp,argv[2],&pixels)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s pixelsPtr\nppixelsPtr must be an integer > 0",argv[0],argv[1]);
         retour =  TCL_ERROR;
      }    
      
      if((argc==4) ) {
         if( (colorPlane = CPixels::getColorPlane( argv[3] )) == PLANE_UNKNOWN ){
            sprintf(ligne,"Usage: %s %s pixelsPtr [PLANE_GREY|PLANE_RGB|PLANE_R|PLANE_G|PLANE_B]",argv[0],argv[1]);
            retour =  TCL_ERROR;
         }
      } else {
         // default plane is PLANE_GREY
         colorPlane = PLANE_GREY;
      }

      buffer = (CBuffer*)clientData;
      if(buffer==NULL) {
         sprintf(ligne,"Buffer is NULL : abnormal error.");
         retour = TCL_ERROR;
      } else {
         try {
            buffer->GetPixels( (TYPE_PIXELS*)pixels, colorPlane);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);
   
   return retour;
}

//==============================================================================
// buf$i getnaxis
//   retuns image naxis
//   
//
int cmdGetNaxis(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   ligne = new char[1000];

   if(argc!=2) {
      sprintf(ligne,"Usage: %s %s ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      sprintf(ligne,"%d",buffer->GetNaxis());
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_OK;
   }
   delete ligne;
   return retour;
}


//==============================================================================
// buf$i getpixelsheight
//   retuns image height
//   
//
int cmdGetPixelsHeight(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   ligne = new char[1000];

   if(argc!=2) {
      sprintf(ligne,"Usage: %s %s ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      sprintf(ligne,"%d",buffer->GetH());
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_OK;
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i getpixelswidth
//   retruns image width
//
//   
//
int cmdGetPixelsWidth(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour;
   ligne = new char[1000];

   if(argc!=2) {
      sprintf(ligne,"Usage: %s %s ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      sprintf(ligne,"%d",buffer->GetW());
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_OK;
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i setpixels class width height bitpix compression pixelsPtr [-keep_keywords] --

//  required parameters : 
//      class       CLASS_GRAY|CLASS_RGB|CLASS_3D|CLASS_VIDEO
//      width       columns number  
//      height      lines number     
//      format      FORMAT_BYTE|FORMAT_SHORT|FORMAT_FLOAT 
//      compression COMPRESS_NONE|COMPRESS_I420|COMPRESS_JPEG|COMPRESS_RAW
//      pixelData     pointer to pixels data 
//  optional parameters     
//      -keep_keywords  
//      -pixelSize   size of pixels data 
//      -reverseX    si "1" alors applique un miroir vertical
//      -reverseY    si "1" alors applique un miroir horizontal

int cmdSetPixels(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour=TCL_OK;               // Code d'erreur de retour.
   int width, height;
   TPixelClass pixelClass;
   TPixelFormat pixelFormat;
   TPixelCompression compression;
   int  pixelData;           // pointeur vers le le tableau de pixels
   long pixelSize;           // taille du tableau de pixels 
   int keep_keywords;
   int i;
   int reverse_x =0;
   int reverse_y =0;
   char comment[]="class width height bitpix compression ?import_pointer? ?-keep_keywords? ?-pixels_size? ?-reverse_x? ?-reverse_y?";

   ligne = (char*)calloc(1000,sizeof(char));
   if( argc < 7 ) {
      sprintf(ligne,"Usage: %s %s %s",argv[0],argv[1],comment);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      free(ligne);
      return TCL_ERROR;
   } else {
      
      // parametres obligatoires
      if( (pixelClass = CPixels::getPixelClass( argv[2] )) == CLASS_UNKNOWN ){
         sprintf(ligne,"Usage: %s %s %s\n class must be CLASS_GRAY|CLASS_RGB|CLASS_3D|CLASS_VIDEO ",argv[0],argv[1],comment);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour =  TCL_ERROR;
      }
      
      if((Tcl_GetInt(interp,argv[3],&width)!=TCL_OK)&&(retour==TCL_OK)) {
         sprintf(ligne,"Usage: %s %s %s\nwidth must be an integer > 0",argv[0],argv[1],comment);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour =  TCL_ERROR;
      } 
      
      if((Tcl_GetInt(interp,argv[4],&height)!=TCL_OK)&&(retour==TCL_OK)) {
         sprintf(ligne,"Usage: %s %s %s\nheight must be an integer > 0",argv[0],argv[1],comment);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour =  TCL_ERROR;
      } 
      
      if( ((pixelFormat = CPixels::getPixelFormat( argv[5] )) == FORMAT_UNKNOWN )&&(retour==TCL_OK)){
         sprintf(ligne,"Usage: %s %s %s\n bitpix must be FORMAT_BYTE|FORMAT_SHORT|FORMAT_FLOAT",argv[0],argv[1],comment);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour =  TCL_ERROR;
      }
      
      if( ((compression = CPixels::getPixelCompression( argv[6] )) == COMPRESS_UNKNOWN)&&(retour==TCL_OK) ){
         sprintf(ligne,"Usage: %s %s %s\n compression must be COMPRESS_NONE|COMPRESS_I420",argv[0],argv[1],comment);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour =  TCL_ERROR;
      }
      
      pixelData=0;
      if (argc>=8) {
         if((Tcl_GetInt(interp,argv[7],&pixelData)!=TCL_OK)&&(retour==TCL_OK)) {
            sprintf(ligne,"Usage: %s %s %s\nppixels must be an integer > 0",argv[0],argv[1],argv[2]);
            retour =  TCL_ERROR;
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         }
      }

      keep_keywords = DONT_KEEP_KEYWORDS;
   
      //  lecture des parametres optionels
      for (i = 8; i < argc - 1; i++) {
   	   if (strcmp(argv[i], "-keep_keywords") == 0) {
            keep_keywords = KEEP_KEYWORDS;
         } 

	      if (strcmp(argv[i], "-pixels_size") == 0) {
	         pixelSize = atol(argv[i + 1]);
	      }

	      if (strcmp(argv[i], "-reverse_x") == 0) {
            if ( strcmp(argv[i+1], "1" ) == 0) {
	            reverse_x = 1;
            } else {
	            reverse_x = 0;
            }
         }

	      if (strcmp(argv[i], "-reverse_y") == 0) {
            if ( strcmp(argv[i+1], "1" ) == 0) {
	            reverse_y = 1;
            } else {
	            reverse_y = 0;
            }
	      }
	   }
   }
    
   if( retour != TCL_ERROR ) {
      buffer = (CBuffer*)clientData;
      if(buffer==NULL) {
         sprintf(ligne,"Buffer is NULL : abnormal error.");
         retour = TCL_ERROR;
      } else {
         buffer->FreeBuffer(keep_keywords);
         try {
            if( pixelClass == CLASS_GRAY ) {
               buffer->SetPixels(PLANE_GREY, width, height, pixelFormat, compression, (void *) pixelData, pixelSize, reverse_x, reverse_y);
            } else {
               buffer->SetPixels(PLANE_RGB, width, height, pixelFormat, compression, (void *) pixelData, pixelSize, reverse_x, reverse_y);
            } 
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }
   free(ligne);
   
   return retour;
}

//==============================================================================
// buf$i mergepixels class width height bitpix compression colorplane pixelsPtr [-keep_keywords] --
//
int cmdMergePixels(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   TColorPlane colorPlane;
   int pixels;
   
   ligne = (char*)calloc(1000,sizeof(char));
   if((argc != 4)) {
      sprintf(ligne,"Usage: %s %s colorpane pixels ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      free(ligne);
      return TCL_ERROR;
   } else {
      if( (colorPlane = CPixels::getColorPlane( argv[2] )) == PLANE_UNKNOWN ){
         sprintf(ligne,"Usage: %s %s colorpane pixels\n colorplane must be PLANE_RED|PLANE_GREEN|PLANE_BLUE",argv[0],argv[1]);
         retour =  TCL_ERROR;
      }     
      
      if(Tcl_GetInt(interp,argv[3],&pixels)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s colorpane width height bitpix compression pixels\nppixels must be an integer > 0",argv[0],argv[1]);
         retour =  TCL_ERROR;
      } 

      buffer = (CBuffer*)clientData;
      if(buffer==NULL) {
         sprintf(ligne,"Buffer is NULL : abnormal error.");
         retour = TCL_ERROR;
      } else {
         try {
            buffer->MergePixels(colorPlane, pixels);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);
   
   return retour;
}

//==============================================================================
// buf$i centro {x1 y1 x2 y2} ?coef? --
//   Fonction de calcul du centroide sur une image dans la fenetre passee en
//   parametres.
//
// buf$i flux {x1 y1 x2 y2} --
//   Fonction de calcul du flux total contenu dans la fenetre.
//
// buf$i phot {x1 y1 x2 y2} ?coef? --
//   Fonction de calcul du flux uniquement au dessus de la moyenne du fond de
//   ciel.
//
int cmdAstroPhot(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   enum {CMD_CENTRO=1,CMD_FLUX,CMD_PHOT};
   int x1, y1, x2, y2;                // Position de la fenetre
   int naxis1, naxis2;                // Taille de l'image
   int retour;
   CBuffer *buffer;
   int t;                             // Variable pour swap
   //int i, j;                          // Index de parcours de l'image
   //TYPE_PIXELS *offset;
   //TYPE_PIXELS *pix;                  // Pointeur d'image
   TYPE_PIXELS seuil;                 // Valeur entiere du seuil de prise en compte
   //TYPE_PIXELS flux_pix;
   TYPE_PIXELS flux = (TYPE_PIXELS)0; // Flux total dans la fenetre
   int cmd = 0;
   double n_sigma = 3.0e0;            // Coefficient d'amplification de l'ecart-type
   TYPE_PIXELS moy;                        // Moyenne sur le contours
   int nbpix = 0;                     // Nombre de pixels pris en compte
   float sx = (float)0.;              // Somme ponderee des pixels en x
   float sy = (float)0.;              // Somme ponderee des pixels en y
   double dFlux = 0.;                 // Flux differentiel
   char s[1000];                       // Chaine pour les resultats
   int verbose = 0;                   // Mode bavard quand le sigma passe est <0
   char **listArgv;                   // Liste des argulents passes a getpix.
   int listArgc;                      // Nombre d'elements dans la liste des coordonnees.
   /* methode Howell */
   float r=(float)0.;
   TYPE_PIXELS maxi;
   //TYPE_PIXELS fond;
   int xmax=0,ymax=0;
   //int nneg,npos;
   //double *vec;
   //int xx1, yy1, xx2, yy2, sortie,rr;
   int ntot=0;

   // On recupere les parametres (et eventuellement on en met par defaut).
   if((argc!=3)&&(argc!=4)) {
      sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?",argv[0],argv[1]);
      Tcl_SetResult(interp,s,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(s,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(s,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            if(strcmp(argv[1],"centro")==0) cmd = CMD_CENTRO;
            else if(strcmp(argv[1],"flux")==0) cmd = CMD_FLUX;
            else if(strcmp(argv[1],"phot")==0) cmd = CMD_PHOT;
            // Decodage du coefficient pour sigma.
            if(argc==4) {
               Tcl_GetDouble(interp,argv[3],&n_sigma);
               if(n_sigma<0) {
                  n_sigma = -n_sigma;
                  verbose = 1;
               }
            }
            buffer = (CBuffer*)clientData;
            naxis1 = buffer->GetW();
            naxis2 = buffer->GetH();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            //pix    = buffer->pix;
            // Remise en ordre des coordonnees de la fenetre
            if(x1>x2) {
               t = x2;
               x2 = x1;
               x1 = t;
            }
            if(y1>y2) {
               t = y2;
               y2 = y1;
               y1 = t;
            }
            if(cmd==0) {
               retour = TCL_ERROR;
            } else if((x1<=0)||(x1>naxis1)||(y1<=0)||(y1>naxis2)||
                      (x2<=0)||(x2>naxis1)||(y2<=0)||(y2>naxis2)) {
               Tcl_SetResult(interp,"Cadre hors de l'image",TCL_VOLATILE);
               retour = TCL_ERROR;
            } else {
                    /*
               if((x1==x2)||(y1==y2)) {
                  if(cmd==CMD_FLUX) {
                     int plane;
                     TYPE_PIXELS val1, val2, val3;
                     buffer->GetPix(&plane, &val1, &val2, &val3, x1-1, y1-1);
                     if ( plane == 1 ) { 
                        flux = val1;
                     } else {
                         flux = (val1 + val2 + val3 ) /3;
                     }
                     sprintf(s,"%f 1",flux);
                     Tcl_SetResult(interp,s,TCL_VOLATILE);
                  } else if(cmd==CMD_PHOT) {
                     strcpy(s,"0");
                     Tcl_SetResult(interp,s,TCL_VOLATILE);
                  } else if(cmd==CMD_CENTRO) {
                     sprintf(s,"%.2f %.2f",(float)x1,(float)y1);
                     Tcl_SetResult(interp,s,TCL_VOLATILE);
                  }
                  retour = TCL_OK;
               } else {*/

                  // Petit changement de repere pour avoir des coordonnees partant de (0,0)
                  // et non de (1,1).
                   x1 -= 1; y1 -= 1; x2 -= 1; y2 -= 1;
                   
                   try {
                       // repere le pixel de valeur maximale et flux total
                       buffer->AstroFlux(x1, y1, x2, y2, &flux, &maxi, &xmax, &ymax, &moy, &seuil, &nbpix);
                       
                       // Calcul du centroide.
                       if(cmd==CMD_CENTRO) {
                           buffer->AstroCentro(x1, y1, x2, y2, xmax, ymax, seuil, &sx, &sy, &r);
                       }
                       // photometry
                       if(cmd==CMD_PHOT) {
                           buffer->AstroPhoto(x1, y1, x2, y2, xmax, ymax, moy, &dFlux, &ntot);
                       }
                       // Mise en forme et sortie
                       if(cmd==CMD_FLUX) {
                           sprintf(s,"%f %d 0",flux,nbpix);
                           Tcl_SetResult(interp,s,TCL_VOLATILE);
                       } else if(cmd==CMD_PHOT) {
                           sprintf(s,"%f %d %f",dFlux,ntot,moy);
                           Tcl_SetResult(interp,s,TCL_VOLATILE);
                       } else if(cmd==CMD_CENTRO) {
                           sprintf(s,"%.2f %.2f %.2f",sx, sy,r);
                           Tcl_SetResult(interp,s,TCL_VOLATILE);
                       }
                       retour = TCL_OK;
                   } catch(const CError& e) {
                       sprintf(s,"%s %s %s ",argv[1],argv[2], e.gets());
                       Tcl_SetResult(interp,s,TCL_VOLATILE);
                       retour = TCL_ERROR;
                   }
               //}
            }
         }
      }
      Tcl_Free((char*)listArgv);
   }
   return retour;
}

//==============================================================================
// buf$i photom window method ?args?
//   Fonction de mesure photometrique par diverses methodes
//
// method=square : carre.
//                 args= r1 r2 r3 (cotes des carres)
//                 F1=somme(r<r1)-mediane(r2<r<r3) 
//                 F2=mediane(r2<r<r3) 
//                 F3=moyenne(r2<r<r3) 
//                 F4=ecart_type(r2<r<r3) (entre 10% et 90%)
//                 F5=nb_points(r<r1)
//
// method=circle : cercle.
//                 args= r1 r2 r3 (rayons)
//                 F1=somme(r<r1)-mediane(r2<r<r3) 
//                 F2=mediane(r2<r<r3) 
//                 F3=moyenne(r2<r<r3) 
//                 F4=ecart_type(r2<r<r3) (entre 10% et 90%)
//                 F5=nb_points(r<r1)
//
int cmdPhotom(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   int naxis1, naxis2;                // Taille de l'image
   int retour;
   CBuffer *buffer;
   //int i, j;                          // Index de parcours de l'image
   //TYPE_PIXELS *offset;
   char s[1000];                       // Chaine pour les resultats

   char **listArgv;                   // Liste des argulents passes a getpix.
   int listArgc;                      // Nombre d'elements dans la liste des coordonnees.
   int method=0;
   int x1,x2,y1,y2, xx1, yy1;
   int n1;
   //int n23,n,xxx1,xxx2,yyy1,yyy2,n23d,n23f;
   double r1,r2,r3;
   //double r11,r22,r33;
   double f23;
   //double xc=0.,yc=0.,mini,maxi,flux_pix,sx,sy,*vec,f1;
   //double dx,dy,dx2,dy2,d2;
   double sigma, fmoy, flux;

   // On recupere les parametres (et eventuellement on en met par defaut).
   if (argc<4) {
      sprintf(s,"Usage: %s %s {x1 y1 x2 y2} square ?args?",argv[0],argv[1]);
      Tcl_SetResult(interp,s,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      naxis1 = buffer->GetW();
      naxis2 = buffer->GetH();
      //pix    = buffer->pix;
      // --- decode la fenetre
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(s,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         Tcl_Free((char*)listArgv);
         return TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(s,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         return TCL_ERROR;
      } else {
         retour=TCL_OK;
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} ?coef?\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
         Tcl_Free((char*)listArgv);
         if (retour==TCL_ERROR) {
            return retour;
         }
      }
      x1 -= 1; y1 -= 1; x2 -= 1; y2 -= 1;
      if (x2<x1) { xx1=x1; x1=x2; x2=xx1; }
      if (y2<y1) { yy1=y1; y1=y2; y2=yy1; }
      if((x1<0)||(x1>=naxis1)||(y1<0)||(y1>=naxis2)||(x2<0)||(x2>=naxis1)||(y2<0)||(y2>=naxis2)) {
         Tcl_SetResult(interp,"Cadre hors de l'image",TCL_VOLATILE);
         return TCL_ERROR;
      }
      // --- decode la methode
      strcpy(s,argv[3]);
      if (strcmp(s,"square")==0) method=0;
      if (strcmp(s,"circle")==0) method=1;
      if ((method==0)||(method==1)) {
         // =================================================================
         // ==== Photomotre carre ou circulaire =============================
         // =================================================================
         if (argc>=7) {
            r1=atof(argv[4]);
            r2=atof(argv[5]);
            r3=atof(argv[6]);
            
            try {
               buffer->AstroPhotom(x1, y1, x2, y2, method, r1, r2, r3, &flux, &f23, &fmoy, &sigma,&n1);
               // - sortie des resultats
               sprintf(s,"%f %f %f %f %d",flux,f23,fmoy,sigma,n1);
               Tcl_SetResult(interp,s,TCL_VOLATILE);
               retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(s,"%s %s %s ",argv[1],argv[2], e.gets());
               Tcl_SetResult(interp,s,TCL_VOLATILE);
               retour = TCL_ERROR;
            }

         } else {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2} square r1 r2 r3",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour=TCL_ERROR;
         }
      } else {
         sprintf(s,"Usage: %s %s {x1 y1 x2 y2} square ?args?",argv[0],argv[1]);
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         retour=TCL_ERROR;
      }
   }
   return retour;
}

//==============================================================================
// buf$i barycenter
int cmdBarycenter(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   int naxis1, naxis2;                // Taille de l'image
   int retour;
   CBuffer *buffer;
   //int i, j;                          // Index de parcours de l'image
   //TYPE_PIXELS *pix;                  // Pointeur d'image
   //TYPE_PIXELS *offset;
   char s[1000];                       // Chaine pour les resultats
   char **listArgv;                   // Liste des argulents passes a getpix.
   int listArgc;                      // Nombre d'elements dans la liste des coordonnees.
   double xc, yc;
   //double flux_pix,flux,sx,sy;
   int x1,x2,y1,y2;

   // On recupere les parametres (et eventuellement on en met par defaut).
   if (argc<3) {
      sprintf(s,"Usage: %s %s {x1 y1 x2 y2}",argv[0],argv[1]);
      Tcl_SetResult(interp,s,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      naxis1 = buffer->GetW();
      naxis2 = buffer->GetH();
      //pix    = buffer->pix;
      // --- decode la fenetre
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(s,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         Tcl_Free((char*)listArgv);
         return TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(s,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,s,TCL_VOLATILE);
         return TCL_ERROR;
      } else {
         retour=TCL_OK;
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2}\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2}\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2}\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(s,"Usage: %s %s {x1 y1 x2 y2}\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,s,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
         Tcl_Free((char*)listArgv);
         if (retour==TCL_ERROR) {
            return retour;
         }
      }
      x1 -= 1; y1 -= 1; x2 -= 1; y2 -= 1;
      if((x1<0)||(x1>=naxis1)||(y1<0)||(y1>=naxis2)||(x2<0)||(x2>=naxis1)||(y2<0)||(y2>=naxis2)) {
         Tcl_SetResult(interp,"Cadre hors de l'image",TCL_VOLATILE);
         return TCL_ERROR;
      }
      // --- barycentre
      buffer->AstroBaricenter(x1, y1, x2, y2, &xc, &yc);

      // - sortie des resultats
      sprintf(s,"%f %f",xc,yc);
      Tcl_SetResult(interp,s,TCL_VOLATILE);
      retour=TCL_OK;
   }
   return retour;
}

//==============================================================================
// buf$i offset val --
//   Decale le contenu du buffer d'une certaine valeur.
//   Equivelent de QMiPS : OFFSET
//
int cmdTtOffset(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double offset;
   char *ligne;
   int retour = TCL_OK;


   ligne = new char[1000];
   if(argc!=3) { // Usage
      sprintf(ligne,"Usage: %s %s offs",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[2],&offset)!=TCL_OK) { // Decadage des arguments
      sprintf(ligne,"Usage: %s %s offs\noffs = must be a	 numerical value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->Offset((float)offset);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i sub filename offset --
//   Soustrait une image du buffer, avec une constante de decalage.
//   Equivelent de QMiPS : SUB
//
int cmdTtSub(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double offset;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=4) { // Usage
      sprintf(ligne,"Usage: %s %s filename offs",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[3],&offset)!=TCL_OK) { // Decadage des arguments
      sprintf(ligne,"Usage: %s %s filename offs\noffs = must be a numerical value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->Sub(argv[2],(float)offset);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i add filename offset --
//   Ajoute une image au buffer, avec une constante de decalage.
//   Equivelent de QMiPS : ADD
//
int cmdTtAdd(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double offset;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=4) { // Usage
      sprintf(ligne,"Usage: %s %s filename offs",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[3],&offset)!=TCL_OK) {  // Decadage des arguments
      sprintf(ligne,"Usage: %s %s filename offs\noffs = must be a numerical value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->Add(argv[2],(float)offset);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i div filename constant --
//   Divise le buffer par une image, avec une constante de gain.
//   Equivelent de QMiPS : DIV
//
int cmdTtDiv(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double gain;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=4) { // Usage
      sprintf(ligne,"Usage: %s %s filename const",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[3],&gain)!=TCL_OK) {  // Decadage des arguments
      sprintf(ligne,"Usage: %s %s filename const\nconst = must be a numerical value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->Div(argv[2],(float)gain);
         retour = TCL_OK;

      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i ngain val --
//   Normalisation du gain du buffer.
//   Equivelent de QMiPS : NGAIN
//
int cmdTtNGain(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double gain;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=3) { // Usage
      sprintf(ligne,"Usage: %s %s gain_value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[2],&gain)!=TCL_OK) { // Decadage des arguments
      sprintf(ligne,"Usage: %s %s gain_value\ngain_value = must be a numerical value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->NGain((float)gain);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i noffset val --
//   Normalisation de l'offset du buffer.
//   Equivelent de QMiPS : NOFFSET
//
int cmdTtNOffset(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double offset;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=3) { // Usage
      sprintf(ligne,"Usage: %s %s offset_value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[2],&offset)!=TCL_OK) { // Decadage des arguments
      sprintf(ligne,"Usage: %s %s offset_value\noffset_value = must be a numerical value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->NOffset((float)offset);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i unsmear val --
//   Desmearing du buffer avec le coefficient.
//   Equivelent de QMiPS : deconvflat
//
int cmdTtUnsmear(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double coef;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=3) { // Usage
      sprintf(ligne,"Usage: %s %s coef",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[2],&coef)!=TCL_OK) { // Decadage des arguments
      sprintf(ligne,"Usage: %s %s coef\ncoef = must be a numerical value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->Unsmear((float)coef);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i opt dark offset --
//   Soustraction du noir avec optimisation.
//   Equivelent de QMiPS : -
//
int cmdTtOpt(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];

   if(argc!=4) { // Usage
      sprintf(ligne,"Usage: %s %s dark offset",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->Opt(argv[2],argv[3]);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i stat --
//   Calcul des statistiques de l'image
//   Equivalent de QMiPS : STAT
//
int cmdTtStat(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;
   float locut, hicut, maxi, mini, mean, sigma, bgmean, bgsigma, contrast;
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   int x1, y1, x2, y2;      // Coordonnees de la fenetre.
   int naxis1,naxis2,temp;

   ligne = new char[1000];

   if((argc!=2)&&(argc!=3)) {
      sprintf(ligne,"Usage: %s %s ?{x1 y1 x2 y2}?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      if (argc==3) {
         if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
            sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(listArgc<4) {
            sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx1 must be an integer",argv[0],argv[1]);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny1 must be an integer",argv[0],argv[1]);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx2 must be an integer",argv[0],argv[1]);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny2 must be an integer",argv[0],argv[1]);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            }
            naxis1 = buffer->GetW();
            naxis2 = buffer->GetH();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
         }
      } else {
         // on va traiter toute l'image
         x1=0;
         y1=0;
         x2=0;
         y2=0;
      }
      // Appel a la methode du buffer
      try {
         buffer->Stat(x1-1,y1-1,x2-1,y2-1,&locut,&hicut,&maxi, &mini, &mean, &sigma,
                         &bgmean, &bgsigma, &contrast);
         sprintf(ligne,"%d %d %d %d %f %f %f %f %f",(int)hicut,(int)locut,(int)maxi,
                 (int)mini,mean,sigma,bgmean,bgsigma,contrast);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i histogram --
//   Calcul de l'histogramme de l'image
//   Equivalent de QMiPS : HISTO
//
// returns :
// ListHisto      (n elements)
// ListMeanAdus   (n elements)
// ListAdus       (n+1 elements)
//
int cmdHistogram(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   int n;
   char *ligne =NULL;
   char *s = NULL;
   char ss[20];
   int retour = TCL_OK;
   long *histo;
   float *adus,*meanadus,mini=(float)0.,maxi=(float)1.;
   int ismini,ismaxi,nmax,k;
   double minid=0.,maxid=0.;

   ligne = new char[1000];

   if(argc>5) {
      sprintf(ligne,"Usage: %s %s ?NbBins? ?min? ?max?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      ismini=0;
      if (argc>=4) {
         if(Tcl_GetDouble(interp,argv[3],&minid)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s ?NbBins? ?min? ?max?\nmin must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
            delete ligne;
            return retour;
         }
         mini=(float)minid;
         ismini=1;
      }
      ismaxi=0;
      if (argc>=5) {
         if(Tcl_GetDouble(interp,argv[4],&maxid)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s ?NbBins? ?min? ?max?\nmax must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
            delete ligne;
            return retour;
         }
         maxi=(float)maxid;
         ismaxi=1;
      }
      n=10;
      if (argc>=3) {
         if(Tcl_GetInt(interp,argv[2],&n)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s ?NbBins?\nNbBins must be an integer between 1 and 10000",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
            delete ligne;
            return retour;
         }
         if (n>10000) {
            n=10000;
         }
         if (n<=0) {
            n=1;
         }
      }
      if ((histo=(long*)calloc(n,sizeof(long)))==NULL) {
         strcpy(ligne,"Memory management problem");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
         delete ligne;
         return retour;
      }
      if ((meanadus=(float*)calloc(n,sizeof(float)))==NULL) {
         strcpy(ligne,"Memory management problem");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
         free(histo);
         delete ligne;
         return retour;
      }
      if ((adus=(float*)calloc(n+1,sizeof(float)))==NULL) {
         strcpy(ligne,"Memory management problem");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
         free(meanadus);
         free(histo);
         delete ligne;
         return retour;
      }
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
      buffer->Histogram(n,adus,meanadus,histo,ismini,mini,ismaxi,maxi);
      nmax=20*2*n;
      if ((s=(char*)calloc(nmax,sizeof(char)))==NULL) {
         strcpy(ligne,"Memory management problem");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
         delete ligne;
         free(meanadus);
         free(histo);
         free(adus);
         return retour;
      }
      strcpy(s,"{");
      for (k=0;k<n;k++) {
         if ((int)strlen(s)<nmax-20) {
            sprintf(ss,"%ld ",histo[k]);
            strcat(s,ss);
         }
      }
      strcat(s,"} {");
      for (k=0;k<n;k++) {
         if ((int)strlen(s)<nmax-20) {
            sprintf(ss,"%f ",meanadus[k]);
            strcat(s,ss);
         }
      }
      strcat(s,"} {");
      for (k=0;k<=n;k++) {
         if ((int)strlen(s)<nmax-20) {
            sprintf(ss,"%f ",adus[k]);
            strcat(s,ss);
         }
      }
      strcat(s,"}");
      Tcl_SetResult(interp,s,TCL_VOLATILE);

      free(s);
      free(histo);
      free(adus);
      free(meanadus);
      } catch(const CError& e) {
         if (s) delete s;
         if(histo) free(histo);
         if(adus) free(adus);
         if(meanadus) free(meanadus);

         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i clipmin --
//   Ecretage de l'image
//   Equivalent de QMiPS : CLIPMIN
//
int cmdClipmin(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;
   double value;

   ligne = new char[1000];

   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetDouble(interp,argv[2],&value)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s value\nValue must be a float",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
         delete ligne;
         return retour;
      }
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
      buffer->Clipmin(value);
      Tcl_SetResult(interp,"",TCL_VOLATILE);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i clipmax --
//   Ecretage de l'image
//   Equivalent de QMiPS : CLIPMAX
//
int cmdClipmax(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;
   double value;

   ligne = new char[1000];

   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s value",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetDouble(interp,argv[2],&value)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s value\nValue must be a float",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
         delete ligne;
         return retour;
      }
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
      buffer->Clipmax(value);
      Tcl_SetResult(interp,"",TCL_VOLATILE);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }
   delete ligne;
   return retour;
}

//==============================================================================
// buf$i rot x0 y0 angle --
//   Rotation de l'image, de centre (x0,y0) (commencent � 1) et d'un angle de
//   'angle' degr�s (dans le sens trigo).
//
int cmdTtRot(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double x0, y0, angle;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=5) { // Usage
      sprintf(ligne,"Usage: %s %s x0 y0 angle",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[2],&x0)!=TCL_OK) {
      sprintf(ligne,"Usage: %s %s x0 y0 angle\nx0 : (float) rotation center abscissa",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[3],&y0)!=TCL_OK) {
      sprintf(ligne,"Usage: %s %s x0 y0 angle\ny0 : (float) Rotation center ordinate",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else if(Tcl_GetDouble(interp,argv[4],&angle)!=TCL_OK) {
      sprintf(ligne,"Usage: %s %s x0 y0 angle\nangle : (float) Rotation angle in degrees",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->Rot((float)x0,(float)y0,(float)angle);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i log coef ?offset? --
//
int cmdTtLog(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   double coef=1.;
   double offset=0.;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if((argc!=4)&&(argc!=3)) {
      sprintf(ligne,"Usage: %s %s coef ?offset?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetDouble(interp,argv[2],&coef)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s coef ?offset?\ncoef : (float) gain of the log",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if((argc==4)&&(Tcl_GetDouble(interp,argv[3],&offset)!=TCL_OK)) {
         sprintf(ligne,"Usage: %s %s coef ?offset?\noffset : (float) offset applied to image before log",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         buffer = (CBuffer*)clientData;
         try {
            buffer->Log((float)coef,(float)offset);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i pointer --
//   Retourne le pointeur de pixels sous la forme d'un entier.
//   Pour usage dans les extensions de LAPIN.
//   Fonction obsol�te . Replac�e par setPixels/getPixels/setPix/getPix

int cmdPointer(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=2) { // Usage
      sprintf(ligne,"Usage: %s %s  , Deprecated command. Replaced by setPixels/getPixels",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      TYPE_PIXELS  * pixelsPointer;
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      buffer->GetPixelsPointer(&pixelsPointer);
      sprintf(ligne,"%d",(int) pixelsPointer);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i mirrorx --
//   Retourne l'image suivant l'axe X (les pixels ne changent pas de ligne).
//
int cmdTtMirrorX(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=2) {
      // Usage
      sprintf(ligne,"Usage: %s %s",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->MirX();
         ligne[0] = 0;
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s ",argv[1], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i mirrory --
//   Retourne l'image suivant l'axe Y (les pixels ne changent pas de colonne).
//
int cmdTtMirrorY(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];
   if(argc!=2) { // Usage
      sprintf(ligne,"Usage: %s %s",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
      buffer->MirY();
         ligne[0] = 0;
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s ",argv[1], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}


//==============================================================================
// buf$i radec2xy {ra dec} --
//   Calcule les coordonnees sur l'image des coordonnees celestes (ra,dec),
//   pourvu qu'il y ait un nombre suffisant de parametres astrometriques. Les
//   coordonnees (ra,dec) peuvent etre des reels, mais pas des chaines de type
//   hms et dms. Il faudra les transformer avec des fonctions idoine.
//
int cmdRadec2xy(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   double ra, dec;           // Coordonnees celestes.
   double x, y;              // Coordonnees image.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int order=1;

   ligne = (char*)calloc(1000,sizeof(char));
   if(argc<3) {
      sprintf(ligne,"Usage: %s %s {ra dec} ?order?",argv[0],argv[1]);
      retour = TCL_ERROR;
   } else {      
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Position struct not valid (not a list?) : must be {ra dec}");
         retour = TCL_ERROR;
      } else if(listArgc!=2) {
         if(listArgc>2) {
            sprintf(ligne,"Position struct not valid (too many args) : must be {ra dec}");
         } else {
            sprintf(ligne,"Position struct not valid (too much args) : must be {ra dec}");
         }
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetDouble(interp,listArgv[0],&ra)!=TCL_OK) {
           sprintf(ligne,"Usage: %s %s {ra dec}\nra must be a numerical value",argv[0],argv[1]);
            retour = TCL_ERROR;
        } else {
            if(Tcl_GetDouble(interp,listArgv[1],&dec)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {ra dec}\ndec must a numerical value",argv[0],argv[1]);
               retour = TCL_ERROR;
           } else {
               if (argc>=4) {
                  order=(int)atof(argv[3]);
                  if (order!=2) { order=1; }
               }
               buffer = (CBuffer*)clientData;
               if(buffer==NULL) {
                  sprintf(ligne,"Buffer is NULL : abnormal error.");
                  retour = TCL_ERROR;
               } else {
                  try {
                     buffer->radec2xy(ra,dec,&x,&y,order);
                     sprintf(ligne,"%f %f",x+1.,y+1.);
                     Tcl_SetResult(interp,ligne,TCL_VOLATILE);
                     retour = TCL_OK;
                  } catch(const CError& ) {
                     strcpy(ligne,"Can't transform coordinates.");
                     Tcl_SetResult(interp,ligne,TCL_VOLATILE);
                     retour = TCL_ERROR;
                  }
               }
            }
         }
         Tcl_Free((char*)listArgv);
      }
   }
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);

   return retour;
}


//==============================================================================
// buf$i xy2radec {x y} --
//   Calcule les coordonnees celestes du point (x,y) a condition qu'il y ait
//   un nombre suffisant de parametres astrometriques. Les coordonnees (x,y)
//   peuvent etre des reels. Les coordonnees celestes sont donnees en degres
//   decimaux, qu'il faudra ensuite convertir en hms et dms eventuellement.
//
int cmdXy2radec(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   double ra, dec;           // Coordonnees celestes.
   double x, y;              // Coordonnees image.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int order=1;

   ligne = (char*)calloc(1000,sizeof(char));
   if(argc<3) {
      sprintf(ligne,"Usage: %s %s {x y} ?order?",argv[0],argv[1]);
      retour = TCL_ERROR;
   } else {
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Position struct not valid (not a list?) : must be {x y}");
         retour = TCL_ERROR;
      } else if(listArgc!=2) {
         if(listArgc>2) {
            sprintf(ligne,"Position struct not valid (too many args) : must be {x y}");
         } else {
            sprintf(ligne,"Position struct not valid (too much args) : must be {x y}");
         }
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetDouble(interp,listArgv[0],&x)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x y}\nx must be real",argv[0],argv[1]);
            retour = TCL_ERROR;
        } else {
            if(Tcl_GetDouble(interp,listArgv[1],&y)!=TCL_OK) {
               sprintf(ligne,"Usage: %s %s {x y}\ny must be real",argv[0],argv[1]);
               retour = TCL_ERROR;
           } else {
               if (argc>=4) {
                  order=(int)atof(argv[3]);
                  if (order!=2) { order=1; }
               }
               buffer = (CBuffer*)clientData;
               if(buffer==NULL) {
                  sprintf(ligne,"Buffer is NULL : abnormal error.");
                  retour = TCL_ERROR;
               } else {
                  try {
                     buffer->xy2radec(x-1.,y-1.,&ra,&dec,order);
                     sprintf(ligne,"%f %f",ra,dec);
                     Tcl_SetResult(interp,ligne,TCL_VOLATILE);
                     retour = TCL_OK;
                  } catch(const CError& e) {
                     sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
                     Tcl_SetResult(interp,ligne,TCL_VOLATILE);
                     retour = TCL_ERROR;
                  }
               }
            }
         }
         Tcl_Free((char*)listArgv);
      }
   }
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);

   return retour;
}


int cmdFwhm(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int x1, y1, x2, y2;      // Coordonnees de la fenetre.
   double maxx, maxy;        // Valeur des maximas en x et y.
   double posx, posy;        // Position en x et y du photocentre.
   double fwhmx, fwhmy;      // Fwhm dans les deux axes de la gaussienne.
   double fondx, fondy;      // Fonds en x et y.
   double errx, erry;        // Erreurs sur les modelisations.
   int naxis1,naxis2,temp;

   ligne = new char[1000];

   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            buffer = (CBuffer*)clientData;
            naxis1 = buffer->GetW();
            naxis2 = buffer->GetH();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
            try {
               buffer->Fwhm(x1-1,y1-1,x2-1,y2-1,&maxx,&posx,&fwhmx,&fondx,&errx,
                  &maxy,&posy,&fwhmy,&fondy,&erry,0.,0.);
               sprintf(ligne,"%.2f %.2f",fwhmx, fwhmy);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            }
         }
         Tcl_Free((char*)listArgv);
      }
   }

   delete ligne;
   return retour;
}

int cmdScar(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
// Cicatrisation d'une zone par interpolation lineaire
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int x1, y1, x2, y2;      // Coordonnees de la fenetre.
   int naxis1,naxis2,temp;

   ligne = new char[1000];

   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            buffer = (CBuffer*)clientData;
            naxis1 = buffer->GetW();
            naxis2 = buffer->GetH();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
            try {
            buffer->Scar(x1-1,y1-1,x2-1,y2-1);
            retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            }
         }
         Tcl_Free((char*)listArgv);
      }
   }

   delete ligne;
   return retour;
}

int cmdFitGauss(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int x1, y1, x2, y2;      // Coordonnees de la fenetre.
   double maxx, maxy;        // Valeur des maximas en x et y.
   double posx, posy;        // Position en x et y du photocentre.
   double fwhmx, fwhmy;      // Fwhm dans les deux axes de la gaussienne.
   double fondx, fondy;      // Fonds en x et y.
   double errx, erry;        // Erreurs sur les modelisations.
   double fluxx,fluxy;
   double pi=3.14159265359;
   int sub,k;
   double fwhmx0=0., fwhmy0=0.; // Fwhm contrainte dans les deux axes de la gaussienne.
   int naxis1,naxis2,temp;
   ligne = new char[1000];

   if(argc<3) {
      sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      sub=0;
      if (argc>=4) {
         for (k=3;k<argc;k++) {
            if (strcmp(argv[k],"-sub")==0) {
               sub=1;
            }
            if (strcmp(argv[k],"-fwhmx")==0) {
               if ((k+1)<argc) {
                  fwhmx0=(double)atof(argv[k+1]);
               }
            }
            if (strcmp(argv[k],"-fwhmy")==0) {
               if ((k+1)<argc) {
                  fwhmy0=(double)atof(argv[k+1]);
               }
            }
         }
      }
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            buffer = (CBuffer*)clientData;
            naxis1 = buffer->GetW();
            naxis2 = buffer->GetH();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
            try {
               buffer->Fwhm(x1-1,y1-1,x2-1,y2-1,&maxx,&posx,&fwhmx,&fondx,&errx,
                  &maxy,&posy,&fwhmy,&fondy,&erry,fwhmx0,fwhmy0);
               fluxx=maxx*0.601*fwhmx*sqrt(pi);
               if (x1<x2) {
                  posx=posx+x1;
               } else {
                  posx=posx+x2;
               }
               if (y2!=y1) {
                  maxx=fluxx/(fwhmx*fwhmy*0.601*0.601*pi);
                  fondx=fondx/(1.+fabs((double)(y2-y1)));
                  errx=errx/(1.+fabs((double)(y2-y1)));
               }
               fluxy=maxy*0.601*fwhmy*sqrt(pi);
               if (y1<y2) {
                  posy=posy+y1;
               } else {
                  posy=posy+y2;
               }
               if (x2!=x1) {
                  maxy=fluxy/(fwhmx*fwhmy*0.601*0.601*pi);
                  fondy=fondy/(1.+fabs((double)(x2-x1)));
                  erry=erry/(1.+fabs((double)(x2-x1)));
               }
               sprintf(ligne,"%f %f %f %f %f %f %f %f",maxx,posx,fwhmx,fondx,maxy,posy,fwhmy,fondy);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_OK;
               if (sub==1) {
                  buffer->SyntheGauss(posx-1.,posy-1.,-maxx, -maxy,fwhmx,fwhmy,0.);
               }
               retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_ERROR;
            }
         Tcl_Free((char*)listArgv);
         }
      }
   }

   delete ligne;
   return retour;
}

int cmdGauss(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   ligne = new char[1000];
   double xc,yc,imax,fwhmx,fwhmy,limitadu=0.;

   if ((argc<3)||(argc>=5)) {
      sprintf(ligne,"Usage: %s %s {xc yc i0 fwhmx fwhmy} ?LimitAdu?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if (argc==4) {
         if(Tcl_GetDouble(interp,argv[3],&limitadu)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {xc yc i0 fwhmx fwhmy} ?LimitAdu?\nLimitAdu must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
            delete ligne;
            return retour;
         }
      }
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {xc yc i0 fwhmx fwhmy}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=5) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {xc yc i0 fwhmx fwhmy}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetDouble(interp,listArgv[0],&xc)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {xc yc i0 fwhmx fwhmy} ?LimitAdu?\nxc must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetDouble(interp,listArgv[1],&yc)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {xc yc i0 fwhmx fwhmy} ?LimitAdu?\nyc must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetDouble(interp,listArgv[2],&imax)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {xc yc i0 fwhmx fwhmy} ?LimitAdu?\ni0 must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetDouble(interp,listArgv[3],&fwhmx)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {xc yc i0 fwhmx fwhmy} ?LimitAdu?\nfwhmx must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetDouble(interp,listArgv[4],&fwhmy)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {xc yc i0 fwhmx fwhmy} ?LimitAdu?\nfwhmy must be a float",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            buffer = (CBuffer*)clientData;
            buffer->SyntheGauss(xc-1.,yc-1.,imax,imax,fwhmx,fwhmy,limitadu);
            Tcl_SetResult(interp,"",TCL_VOLATILE);
            retour = TCL_OK;
         }
         Tcl_Free((char*)listArgv);
      }
   }

   delete ligne;
   return retour;
}

int cmdImaSeries(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.

   ligne = (char*)calloc(1000,sizeof(char));
   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s string",argv[0],argv[1]);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      if(buffer==NULL) {
         sprintf(ligne,"Buffer is NULL : abnormal error.");
         retour = TCL_ERROR;
      } else {
         // Appel a la methode du buffer
         try {
            buffer->TtImaSeries(argv[2]);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }

   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);
   return retour;
}

int cmdScale(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   char *s;
   int retour;               // Code d'erreur de retour.
   double scalex,scaley;
   char **listArgv;
   int listArgc;

   ligne = (char*)calloc(1000,sizeof(char));
   if ((argc<3)||(argc>4)) {
      sprintf(ligne,"Usage: %s %s ListOfTwoScalingFactors ?NormaFlux?",argv[0],argv[1]);
      retour = TCL_ERROR;
   } else {
      buffer = (CBuffer*)clientData;
      if(buffer==NULL) {
         sprintf(ligne,"Buffer is NULL : abnormal error.");
         retour = TCL_ERROR;
      } else {
         if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
            Tcl_SetResult(interp,"",TCL_VOLATILE);
            return TCL_ERROR;
         }
         scalex=atof(listArgv[0]);
         if(listArgc==2) {
            scaley=atof(listArgv[1]);
         } else {
            scaley=scalex;
         }
         if (scalex==0.) {scalex=1.;}
         if (scaley==0.) {scaley=1.;}
         s = new char[256];
         if (argc==3) {
            sprintf(s,"RESAMPLE \"paramresample=%f 0 0 0 %f 0\" ",scalex,scaley);
         }
         if (argc==4) {
            sprintf(s,"RESAMPLE \"paramresample=%f 0 0 0 %f 0\" normaflux=%f",scalex,scaley,atof(argv[3]));
         }
         // Appel a la methode du buffer
         try {
            buffer->TtImaSeries(s);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
         delete s;
      }
   }

   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);
   return retour;
}

int cmdBinX(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int x1, x2;               // Coordonnees de la fenetre.

   int width = 20;           // Largeur par d�faut de l'image de sortie
   ligne = new char[1000];
   int naxis1,temp;

   if((argc!=4)&&(argc!=5)) {
      sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetInt(interp,argv[2],&x1)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(Tcl_GetInt(interp,argv[3],&x2)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if((argc==5)&&(Tcl_GetInt(interp,argv[4],&width)!=TCL_OK)) {
         sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         buffer = (CBuffer*)clientData;
         try {
            naxis1 = buffer->GetW();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            buffer->BinX(x1-1,x2-1,width);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }

   delete ligne;
   return retour;
}

int cmdBinY(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int y1, y2;               // Coordonnees de la fenetre.
   int naxis2,temp;

   int height = 20;          // Hauteur par d�faut de l'image de sortie
   ligne = new char[1000];

   if((argc!=4)&&(argc!=5)) {
      sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetInt(interp,argv[2],&y1)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(Tcl_GetInt(interp,argv[3],&y2)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if((argc==5)&&(Tcl_GetInt(interp,argv[4],&height)!=TCL_OK)) {
         sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         buffer = (CBuffer*)clientData;
         try {
            naxis2 = buffer->GetH();
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
            buffer->BinY(y1-1,y2-1,height);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }

   delete ligne;
   return retour;
}

int cmdMedX(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int x1, x2;               // Coordonnees de la fenetre.
   int naxis1,temp;

   int width = 20;           // Largeur par d�faut de l'image de sortie
   ligne = new char[1000];

   if((argc!=4)&&(argc!=5)) {
      sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetInt(interp,argv[2],&x1)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(Tcl_GetInt(interp,argv[3],&x2)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if((argc==5)&&(Tcl_GetInt(interp,argv[4],&width)!=TCL_OK)) {
         sprintf(ligne,"Usage: %s %s x1 x2 ?width?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         buffer = (CBuffer*)clientData;
         try {
            naxis1 = buffer->GetW();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            buffer->MedX(x1-1,x2-1,width);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }

   delete ligne;
   return retour;
}

int cmdMedY(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int y1, y2;               // Coordonnees de la fenetre.
   int naxis2,temp;

   int height = 20;          // Hauteur par d�faut de l'image de sortie
   ligne = new char[1000];

   if((argc!=4)&&(argc!=5)) {
      sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_GetInt(interp,argv[2],&y1)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(Tcl_GetInt(interp,argv[3],&y2)!=TCL_OK) {
         sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if((argc==5)&&(Tcl_GetInt(interp,argv[4],&height)!=TCL_OK)) {
         sprintf(ligne,"Usage: %s %s y1 y2 ?height?",argv[0],argv[1]);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         buffer = (CBuffer*)clientData;
         try {
            naxis2 = buffer->GetH();
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
            buffer->MedY(y1-1,y2-1,height);
            retour = TCL_OK;
         } catch(const CError& e) {
            sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         }
      }
   }

   delete ligne;
   return retour;
}

int cmdWindow(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   char *s;
   int retour;               // Code d'erreur de retour.
   int x1,x2,y1,y2,temp,naxis1,naxis2;
   char **listArgv;
   int listArgc;

   ligne = new char[1000];

   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2}\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            buffer = (CBuffer*)clientData;
            // Collecte de renseignements pour la suite
            naxis1 = buffer->GetW();
            naxis2 = buffer->GetH();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
            s = new char[1000];
            sprintf(s,"WINDOW x1=%d x2=%d y1=%d y2=%d ",x1,x2,y1,y2);
            // Appel a la methode du buffer
            try {
               buffer->TtImaSeries(s);
               strcpy(ligne,"");
               retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
               retour = TCL_ERROR;
            }
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            delete s;
         }
      }
   }

   delete ligne;
   return retour;
}

int cmdMult(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   char *s;
   int retour;               // Code d'erreur de retour.
   double cste;

   ligne = new char[1000];

   if(argc!=3) {
      sprintf(ligne,"Usage: %s %s cste",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      cste=(double)atof(argv[2]);
      buffer = (CBuffer*)clientData;
      s = new char[1000];
      sprintf(s,"MULT constant=%f ",cste);
      // Appel a la methode du buffer
      try {
         buffer->TtImaSeries(s);
         strcpy(ligne,"");               
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         retour = TCL_ERROR;
      }
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      delete s;
   }

   delete ligne;
   return retour;
}

int cmdAutocuts(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
/**************************************************************************/
{

   double hicut,locut,mode;
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;


   ligne = (char*)calloc(1000,sizeof(char));
   buffer = (CBuffer*)clientData;
   try {
      buffer->Autocut(&hicut, &locut, &mode);
      sprintf(ligne,"%f %f %f",hicut,locut,mode);
      retour = TCL_OK;
   } catch(const CError& e) {
      sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
      retour = TCL_ERROR;
   }

   // retour des resultats
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      free(ligne);
   return retour;
}

int cmdRestoreInitialCut(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
/**************************************************************************/
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;

   ligne = (char*)calloc(1000,sizeof(char));
   buffer = (CBuffer*)clientData;
   try {
      buffer->RestoreInitialCut();
      ligne[0] = 0;
      retour = TCL_OK;
   } catch(const CError& e) {
      sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
      retour = TCL_ERROR;
   }

   // retour des resultats
   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
   free(ligne);
   return retour;
}

int cmdA_StarList(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
   {
   CBuffer *buffer;
   char *ligne,*filename;
   int msg,retour=TCL_OK;
   double threshin=0;

   double fwhm = 3;
   int radius = 4;
   double threshold = 40 ;
   int border = 20 ;
   int after_gauss = 0;

   char usage[]="Usage: %s %s threshin ?filename? ?after_gauss? ?fwhm? ?radius? ?border? ?threshold?\n%s";


   /*
   A_starlist - returns number of stars on image and save stars-list to file

   Parameters:

   threshin - pixels above threshin are taken by gauss filter,
         suggested:
		   threshin = (total average on the image) + 3*(total standard deviation of the image)

   filename - where save the star list - ?optional?

   after_gauss - ?optional?, copy to buffer image after gauss filter, y or n - default n
                 
   fwhm - ?optional?, default 3.0, best betwen 2.0 and 4.0

   radius - ?optional?, default 4, "radius" of gauss matrix  - size is (2*radius+1) x (2*radius+1)

   border - ?optional?, default 20, should be set to more or equal to radius

   threshold - ?optional?, default 40.0, best betwen 30.0 and 50.0, is used after gauss filter
              when procerure is looking for stars, pixels below threshold are not taken
   */

   filename = NULL;

   ligne = new char[1024];
   buffer = (CBuffer*)clientData;

   if(argc<3 || argc>9) {
      sprintf(ligne,usage,argv[0],argv[1],"bad number of arguments");
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   }
   else
   {	   
	   if(Tcl_GetDouble(interp,argv[2],&threshin)!=TCL_OK)
	   {
		   sprintf(ligne,usage,argv[0],argv[1],"threshin is not double");
		   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
		   retour = TCL_ERROR;
	   }
	   else
	   {
		   if(argc>=4) filename = argv[3];

		   if(argc>=5)
			   if(strlen(argv[4])==1 && (argv[4][0]=='y' || argv[4][0]=='n'))
			   {
				   if(argv[4][0]=='y')
					   after_gauss = 1;
			   }
			   else
			   {
				   sprintf(ligne,usage,argv[0],argv[1],"after_gauss is not y or n");
				   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
				   retour = TCL_ERROR;
			   }
		   if(argc>=6 && retour == TCL_OK)			   
			   if(Tcl_GetDouble(interp,argv[5],&fwhm)!=TCL_OK)
			   {
				   sprintf(ligne,usage,argv[0],argv[1],"fwhm is not double");
				   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
				   retour = TCL_ERROR;
			   }
		   if(argc>=7 && retour == TCL_OK)
			   if(Tcl_GetInt(interp,argv[6],&radius)!=TCL_OK)
			   {
				   sprintf(ligne,usage,argv[0],argv[1],"radius is not int");
				   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
				   retour = TCL_ERROR;
			   }
		   if(argc>=8 && retour == TCL_OK)
			   if(Tcl_GetInt(interp,argv[7],&border)!=TCL_OK)
			   {
				   sprintf(ligne,usage,argv[0],argv[1],"border is not int");
				   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
				   retour = TCL_ERROR;
			   }
		   if(argc==9 && retour == TCL_OK)
			   if(Tcl_GetDouble(interp,argv[8],&threshold)!=TCL_OK)
			   {
				   sprintf(ligne,usage,argv[0],argv[1],"threshold is not double");
				   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
				   retour = TCL_ERROR;
			   }

		   if(retour == TCL_OK)
		   {
            try {
			   msg = buffer->A_StarList(threshin,filename,fwhm,radius,border,threshold,after_gauss);
				   sprintf(ligne,"%d",msg);
               retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
				   retour = TCL_ERROR;
			   } 
				   Tcl_SetResult(interp,ligne,TCL_VOLATILE);
		   }
	   }
   }
   
   delete ligne;
   return retour;
}
   
int cmdFitGauss2d(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;          // Buffer de travail pour cette fonction.
   char **listArgv;          // Liste des argulents passes a getpix.
   int listArgc;             // Nombre d'elements dans la liste des coordonnees.
   char *ligne;              // Ligne affectee dans le resultat de la commande TCL.
   int retour;               // Code d'erreur de retour.
   int x1, y1, x2, y2;      // Coordonnees de la fenetre.
   double maxx, maxy;        // Valeur des maximas en x et y.
   double posx, posy;        // Position en x et y du photocentre.
   double fwhmx, fwhmy;      // Fwhm dans les deux axes de la gaussienne.
   double fondx, fondy;      // Fonds en x et y.
   double errx, erry;        // Erreurs sur les modelisations.
   int temp,naxis1,naxis2;
   int sub,k;
   double fwhmx0=0., fwhmy0=0.; // Fwhm contrainte dans les deux axes de la gaussienne.
   ligne = new char[1000];

   if(argc<3) {
      sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      sub=0;
      if (argc>=4) {
         for (k=3;k<argc;k++) {
            if (strcmp(argv[k],"-sub")==0) {
               sub=1;
            }
            if (strcmp(argv[k],"-fwhmx")==0) {
               if ((k+1)<argc) {
                  fwhmx0=(double)atof(argv[k+1]);
               }
            }
            if (strcmp(argv[k],"-fwhmy")==0) {
               if ((k+1)<argc) {
                  fwhmy0=(double)atof(argv[k+1]);
               }
            }
         }
      }
      if(Tcl_SplitList(interp,argv[2],&listArgc,&listArgv)!=TCL_OK) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else if(listArgc!=4) {
         sprintf(ligne,"Window struct not valid (not a list?) : must be {x1 y1 x2 y2}");
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      } else {
         if(Tcl_GetInt(interp,listArgv[0],&x1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\nx1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[1],&y1)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\ny1 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[2],&x2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\nx2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else if(Tcl_GetInt(interp,listArgv[3],&y2)!=TCL_OK) {
            sprintf(ligne,"Usage: %s %s {x1 y1 x2 y2} ?-sub? ?-fwhmx value? ?-fwhmy value?\ny2 must be an integer",argv[0],argv[1]);
            Tcl_SetResult(interp,ligne,TCL_VOLATILE);
            retour = TCL_ERROR;
         } else {
            buffer = (CBuffer*)clientData;
            naxis1 = buffer->GetW();
            naxis2 = buffer->GetH();
            if (x1<1) {x1=1;}
            if (x2<1) {x2=1;}
            if (y1<1) {y1=1;}
            if (y2<1) {y2=1;}
            if (x1>naxis1) {x1=naxis1;}
            if (x2>naxis1) {x2=naxis1;}
            if (y1>naxis2) {y1=naxis2;}
            if (y2>naxis2) {y2=naxis2;}
            if (x1 > x2) {
               temp = x1;
               x1 = x2;
               x2 = temp;
            }
            if (y1 > y2) {
               temp = y1;
               y1 = y2;
               y2 = temp;
            }
            try {
               buffer->Fwhm2d(x1-1,y1-1,x2-1,y2-1,&maxx,&posx,&fwhmx,&fondx,&errx,
                  &maxy,&posy,&fwhmy,&fondy,&erry,fwhmx0,fwhmy0);
               if (x1<x2) {
                  posx=posx+x1;
               } else {
                  posx=posx+x2;
               }
               if (y1<y2) {
                  posy=posy+y1;
               } else {
                  posy=posy+y2;
               }
               sprintf(ligne,"%f %f %f %f %f %f %f %f",maxx,posx,fwhmx,fondx,maxy,posy,fwhmy,fondy);
               Tcl_SetResult(interp,ligne,TCL_VOLATILE);
               retour = TCL_OK;
               if (sub==1) {
                  buffer->SyntheGauss(posx-1.,posy-1.,-maxx,-maxy,fwhmx,fwhmy,0.);
               }
               retour = TCL_OK;
            } catch(const CError& e) {
               sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
               retour = TCL_ERROR;
            }
         }
         Tcl_Free((char*)listArgv);
      }
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i stat --
//   Uniformisation du fond du ciel
//   Equivalent de QMiPS : STAT
//
int cmdUnifyBg(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   CBuffer *buffer;
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];

   if((argc!=2)) {
      sprintf(ligne,"Usage: %s %s ",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      // Appel a la methode du buffer
      buffer = (CBuffer*)clientData;
      try {
         buffer->UnifyBg();
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s ",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
   }

   delete ligne;
   return retour;
}

//==============================================================================
// buf$i region --
//   calcul si un point (x y) est dans un polygone (x1 y1 x2 y2 .... )
//   
//
int cmdRegion(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
   char *ligne;
   int retour = TCL_OK;

   ligne = new char[1000];

#ifdef WIN32
   char **pointArgv;          // Liste des argulents passes a getpix.
   int pointArgc;             // Nombre d'elements dans la liste des coordonnees.
   char **regionArgv;          // Liste des argulents passes a getpix.
   int regionArgc;             // Nombre d'elements dans la liste des coordonnees.

   if((argc != 4)) {
      sprintf(ligne,"Usage: %s %s { x1 y1 x2 y2 ... } { x y }",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
   } else {
      POINT * ppoints =  NULL;

      try {
         int nbPoints;
         int x, y ;
         HRGN region;
         BOOL isInRegion;
         double value;

         if(Tcl_SplitList(interp,argv[2],&regionArgc,&regionArgv)!=TCL_OK) {
            throw CError("Region struct not valid (not a list?) : must be {x1 y1 x2 y2 ... }");
         } 
         if(Tcl_SplitList(interp,argv[3],&pointArgc,&pointArgv)!=TCL_OK) {
            throw CError("Region struct not valid (not a list?) : must be { x y ... }");
         } 
         
         nbPoints = regionArgc/2;
         ppoints = (POINT *) malloc(nbPoints * sizeof(POINT));
         for(int i =0; i< nbPoints; i++ ) {
            if(Tcl_GetDouble(interp,regionArgv[i*2], &value)!=TCL_OK) {
               throw CError("Region not valid : point[%d].x=%s is not integer", i/2, regionArgv[i*2]);
            } 
            ppoints[i].x = (long) value;
            if(Tcl_GetDouble(interp,regionArgv[i*2+1],&value)!=TCL_OK) {
               throw CError("Region struct not valid : point[%d].y=%s is not integer", i/2, regionArgv[i*2+1]);
            } 
            ppoints[i].y = (long) value;
         }

         if(Tcl_GetDouble(interp,pointArgv[0],&value)!=TCL_OK) {
            throw CError("Point not valid : point.x=%s is not integer", i, pointArgv[0]);
         } 
         x = (int) value;
         if(Tcl_GetDouble(interp,pointArgv[1],&value)!=TCL_OK) {
            throw CError("Point not valid : y=%s is not integer", pointArgv[1]);
         } 
         y = (int) value;


         region = CreatePolygonRgn( ppoints,  nbPoints, WINDING);
         isInRegion = PtInRegion(region, x, y);
         DeleteObject(region);
      
	      sprintf(ligne,"%d",isInRegion);
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_OK;
      } catch(const CError& e) {
         sprintf(ligne,"%s %s %s",argv[1],argv[2], e.gets());
         Tcl_SetResult(interp,ligne,TCL_VOLATILE);
         retour = TCL_ERROR;
      }
      if(ppoints != NULL ) free(ppoints);
   }

#else
     sprintf(ligne,"%s %s not implemented for this operating system",argv[0],argv[1]);
      Tcl_SetResult(interp,ligne,TCL_VOLATILE);
      retour = TCL_ERROR;
#endif

   delete ligne;
   return retour;

}


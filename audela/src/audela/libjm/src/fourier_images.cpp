/***
 * @file : fourier_images.cpp
 * @brief : Méthodes de l'objet Fourier : traitement des fichiers image
 * @author : Jacques MICHELET <jacques.michelet@laposte.net>
 *
 * Mise à jour $Id: fourier_images.cpp,v 1.6 2010-07-22 18:54:35 jacquesmichelet Exp $
 *
 * <pre>
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
 * </pre>
 */
#include <fstream>
#include <math.h>
#include <gsl/gsl_fft_complex.h>

#include "cerror.h"
#include "libtt.h"
#include "cbuffer.h"
#include "cpixels.h"
#include "cpixelsgray.h"
#include "cfile.h"
#include "libjm.h"
#include "fourier.h"

namespace LibJM {

/*****************************************************************************/
/* Analyse de la coherence des parametres de fichiers résultats d'une DFT    */
/*****************************************************************************/
/*****************************************************************************/
void Fourier::coherence_images_tfd( Fourier::Parametres * param_1, Fourier::Parametres * param_2 ) {

    if ( ( param_1->largeur == 0 )
            || ( param_1->hauteur == 0 )
            || ( param_2->largeur == 0 )
            || ( param_2->hauteur == 0 ) ) {
        throw CError( "Both files must have non-null width and height" );
    }

    if ( ( param_1-> largeur != param_2->largeur )
            || ( param_1-> hauteur != param_2->hauteur ) ) {
        throw CError( "Both files must have the same width and the same heigth" );
    }

    if ( param_1->ordre != param_2->ordre ) {
        throw CError( "Both files must have the same order (centered or regular)" );
    }

    if ( ( ( ( param_1->type == Fourier::REAL ) && ( param_2->type == Fourier::IMAG ) )
            || ( ( param_2->type == Fourier::REAL ) && ( param_1->type == Fourier::IMAG ) )
            || ( ( param_1->type == Fourier::SPECTRUM ) && ( param_2->type == Fourier::PHASE ) )
            || ( ( param_2->type == Fourier::SPECTRUM ) && ( param_1->type == Fourier::PHASE ) )
            || ( ( param_1->type == Fourier::NO_TYPE ) && ( param_2->type == Fourier::NO_TYPE ) ) ) == false ) {
        throw CError( "Both files must have a complementary type (Real-Imag or Spectrum-Phase)" );
    }
}

/****************************************************************************/
/* Ouverture et analyse des mots-clés d'un fichier résultat d'une DFT       */
/****************************************************************************/
/****************************************************************************/
void Fourier::lecture_image( const char * nom, Fourier::Parametres * param )
{
    fourier_info2( "nom=" << nom << " param=" << (void*)param );
    try
    {
        CPixels * image = 0;
        CFitsKeywords * keywords = 0;

        CFileFormat format_source = CFile::loadFile( (char *)nom, TFLOAT, &image, &keywords );

        if ( image->getPixelClass() != CLASS_GRAY )
            throw CError( "%s is not a one-colour plane image", nom );

        if ( format_source != CFILE_FITS )
            throw CError( "%s is not a FITS-compliant file", nom );

        /* Vérification du nombre de dimensions de l'image */
        CFitsKeyword *kwd = keywords->FindKeyword( "NAXIS" );
        if ( kwd == 0 )
            throw CError( "%s does not contain a valid header", nom );

        if ( kwd->GetIntValue() != 2 )
            throw CError( "%s must be a 2-dimension image", nom );

        /* Elimination des images couleurs */
        kwd = keywords->FindKeyword( "NAXIS3" );
        if ( kwd )
        {
            throw CError( "%s is not a one-colour plane image", nom );
        }

        /* Récupération des largeur et hauteur de l'image */
        kwd = keywords->FindKeyword( "NAXIS1" );
        if ( kwd == 0 )
            throw CError( "%s does not contain a NAXIS1 keyword", nom );

        param->largeur = kwd->GetIntValue() ;

        kwd = keywords->FindKeyword( "NAXIS2" );
        if ( kwd == 0 )
            throw CError( "%s does not contain a NAXIS2 keyword", nom );

        param->hauteur = kwd->GetIntValue() ;

        /* Vérification des paramètres TFD de l'image */
        kwd = keywords->FindKeyword( "DFT_TYPE" );
        if ( kwd != 0 )
            param->type = Fourier::analyse_dft_type( kwd->GetStringValue() );

        kwd = keywords->FindKeyword( "DFT_ORD" );
        if( kwd != 0 )
            param->ordre = Fourier::analyse_dft_ordre( kwd->GetStringValue() );

        kwd = keywords->FindKeyword( "DFT_NORM" );
        if( kwd != 0 )
            param->norm = kwd->GetFloatValue();

        kwd = keywords->FindKeyword( "DFT_OFFS" );
        if( kwd != 0 )
            param->talon = kwd->GetFloatValue();

        /* Récupération des pixels */
        TYPE_PIXELS * s_pix = 0;
        image->GetPixelsPointer( &s_pix );
        TableauPixels * tp = new TableauPixels( param->largeur, param->hauteur );
        memcpy( tp->pointeur(), s_pix, tp->taille() );
        param->pixels( tp );

        /* Pointeurs stockés pour pouvoir détruire les objets plus tard */
        param->keywords( keywords );

        delete image;
    }
    catch( const CError& e )
    {
        throw CError( e.gets() );
    }

}

/****************************************************************************/
/* Ecriture d'une image en virgule flottantte                               */
/****************************************************************************/
/****************************************************************************/
void Fourier::ecriture_image( const char * nom, Fourier::Parametres * param )
{
    fourier_info2 ( nom << " taille " << param->largeur << "x" << param->hauteur );

    int format_stockage = FLOAT_IMG;    /* Format en virgule flottante */
    param->keywords()->Add( "BITPIX", &format_stockage, TINT, "", "" );

    /* Utilisation d'une zone mémoire à part */
    /* est-ce utile ? */
    TYPE_PIXELS * d = (TYPE_PIXELS *)malloc( param->pixels()->taille() );
    memcpy( d, param->pixels()->pointeur(), param->pixels()->taille() );

    CPixelsGray * pix_dest = new CPixelsGray( param->largeur, param->hauteur, FORMAT_FLOAT, d, 0, 0 );
    CFile::saveFits( (char *)nom, 0, pix_dest, param->keywords() );

    /* Nettoyage */
    delete pix_dest;
    free( d );
 }

/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
void Fourier::tfd_directe_image ( const char * src, const char * dest_1, const char * dest_2, Fourier::format format, Fourier::Ordre ordre )
{
    fourier_info1( "src=" << src << " dest_1=" << dest_1 << " dest_2=" << dest_2 << " format=" << format << " ordre=" << ordre );
    try
    {
        Fourier::Parametres param_src;
        lecture_image( src, &param_src );
        fourier_info1( "largeur=" << param_src.largeur
                << " hauteur=" << param_src.hauteur );
        fourier_info1( "type=" << param_src.type
                << " ordre=" << param_src.ordre
                << " norm=" << param_src.norm
                << " talon=" << param_src.talon );

        /* Place pour les images de sortie */
        Fourier::Parametres param_1;
        Fourier::Parametres param_2;

        if ( format == Fourier::CARTESIAN ) {
            param_1.init( param_src.largeur, param_src.hauteur, ordre, Fourier::REAL );
            param_2.init( param_src.largeur, param_src.hauteur, ordre, Fourier::IMAG );
        }
        else {
            param_1.init( param_src.largeur, param_src.hauteur, ordre, Fourier::SPECTRUM );
            param_2.init( param_src.largeur, param_src.hauteur, ordre, Fourier::PHASE );
        }

        /* Valeur (arbitraire) de normalisation */
        int val_max = 32767;

        /* TFD */
        tfd_2d_directe_complete( &param_src, &param_1, &param_2, val_max );

        /* Sauvegardes avec les entêtes spécifiques */
        if ( format == Fourier::CARTESIAN )
            param_src.keywords()->Add( "DFT_TYPE", (void*)"REAL", TSTRING, "Real part of a Discrete Fourier Transform", "" );
        else
            param_src.keywords()->Add( "DFT_TYPE", (void*)"SPECTRUM", TSTRING, "Module of a Discrete Fourier Transform", "" );

        if ( ordre == Fourier::CENTERED )
            param_src.keywords()->Add( "DFT_ORD", (void*)"CENTERED", TSTRING, "Low spatial frequencies are located at image center", "" );
        else
            param_src.keywords()->Add( "DFT_ORD", (void*)"REGULAR", TSTRING, "High spatial frequencies are located at image center", "" );

        param_src.keywords()->Add( "DFT_NORM", &param_1.norm, TFLOAT, "Normalisation value", "adu" );
        param_src.keywords()->Add( "DFT_OFFS", &param_1.talon, TFLOAT, "Normalisation value", "adu" );
        CPixelsGray * pix_dest_1 = new CPixelsGray( param_src.largeur, param_src.hauteur, FORMAT_FLOAT, param_1.pixels()->pointeur(), 0, 0 );
        CFile::saveFits( (char *)dest_1, 0, pix_dest_1, param_src.keywords() );

        if ( format == Fourier::CARTESIAN )
            param_src.keywords()->Add( "DFT_TYPE", (void*)"IMAG", TSTRING, "Imaginary part of a Discrete Fourier Transform", "" );
        else
            param_src.keywords()->Add( "DFT_TYPE", (void*)"PHASE", TSTRING, "Phase of a Discrete Fourier Transform", "" );

        param_src.keywords()->Add( "DFT_NORM", &param_2.norm, TFLOAT, "Normalisation value", "adu" );
        param_src.keywords()->Add( "DFT_OFFS", &param_2.talon, TFLOAT, "Normalisation value", "adu" );
        CPixelsGray * pix_dest_2 = new CPixelsGray( param_src.largeur, param_src.hauteur, FORMAT_FLOAT, param_2.pixels()->pointeur(), 0, 0 );
        CFile::saveFits( (char *)dest_2, 0, pix_dest_2, param_src.keywords() );

        delete pix_dest_1;
        delete pix_dest_2;
    }
    catch( const CError& e )
    {
        throw CError( e.gets() );
    }

}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void Fourier::tfd_inverse_image( const char * src_1, const char * src_2, const char * dest )
{
    fourier_info1( "src_1=" << src_1 << " src_2=" << src_2 << " dest=" << dest );
    try
    {
        Fourier::Parametres param_1;
        lecture_image( src_1, &param_1 );
        fourier_info1( "largeur_1=" << param_1.largeur
                << " hauteur_1=" << param_1.hauteur << "\n" );
        fourier_info1( "type_1=" << param_1.type
                << " ordre_1=" << param_1.ordre
                << " norm_1=" << param_1.norm
                << " talon_1=" << param_1.talon << "\n" );

        Fourier::Parametres param_2;
        lecture_image( src_2, &param_2 );
        fourier_info1( "largeur_2=" << param_2.largeur
                << " hauteur_2=" << param_2.hauteur << "\n" );
        fourier_info1( "type_2=" << param_2.type
                << " ordre_2=" << param_2.ordre
                << " norm_2=" << param_2.norm
                << " talon_2=" << param_2.talon << "\n" );

        coherence_images_tfd( &param_1, &param_2 );

        /* Place pour l'image de sortie */
        TYPE_PIXELS * tab_dest = new TYPE_PIXELS[ param_1.largeur * param_1.hauteur * sizeof( TYPE_PIXELS ) ];

        /* Valeur (arbitraire) de normalisation */
        int val_max = 32767;

        /* TFD inverse */
        Fourier::tfd_2d_inverse_complete( &param_1, &param_2, tab_dest, val_max );

        /* Suppression et transformation des mots clés */
        param_1.keywords()->Delete( (char *)"DFT_NORM" );
        param_1.keywords()->Delete( (char *)"DFT_OFFS" );
        param_1.keywords()->Delete( (char *)"DFT_ORD" );
        param_1.keywords()->Delete( (char *)"DFT_TYPE" );
        param_1.keywords()->Add( "DFT_TYPE", (void*)"I_DFT", TSTRING, "Result of an Inverse Discrete Fourier Transform", "" );

        /* Sauvegarde */
        CPixelsGray * pix_dest = new CPixelsGray( param_1.largeur, param_1.hauteur, FORMAT_FLOAT, tab_dest, 0, 0 );
        CFile::saveFits( (char *)dest, 0, pix_dest, param_1.keywords() );

        /* Nettoyage */
        delete[] tab_dest;
        delete pix_dest;
    }
    catch( const CError& e )
    {
        throw CError( e.gets() );
    }
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void Fourier::correl_convol_image ( const char * src_1, const char * src_2, const char * dest, Fourier::operateur op, Fourier::Ordre ordre, bool normalisation )
{
    if ( src_2 != 0 )
    {
        fourier_info1( "src_1=" << src_1 << " src_2=" << src_2 << " dest=" << dest << " op=" << op << " ordre=" << ordre << " normalisation=" << normalisation );
    }
    else
    {
        fourier_info1( "src_1=" << src_1 << " src_2=0 dest=" << dest << " op=" << op << " ordre=" << ordre << " normalisation=" << normalisation );
    }
    try
    {
        Fourier::Parametres param_src1;
        Fourier::Parametres param_src2;

        lecture_image( src_1, &param_src1 );
        fourier_info1 ( src_1 << " taille " << param_src1.largeur << "x" << param_src1.hauteur );

        /* Recherche des valeurs extrêmes */
        TYPE_PIXELS maximum;
        TYPE_PIXELS minimum;
        Fourier::extrema( &param_src1, &minimum, &maximum );

        Fourier::Parametres * param_src = 0;
        if ( src_2 != 0 )
        {
            lecture_image( src_2, &param_src2 );
            fourier_info1 ( src_2 << " taille " << param_src2.largeur << "x" << param_src2.hauteur );

            param_src = Fourier::inclusion( &param_src1, &param_src2 );
            if ( param_src == 0 )
                throw CError( "These images sizes are not yet supported" );
            fourier_info1 ( "Retaillage des images en " << param_src->largeur << "x" << param_src->hauteur );
        }
        else
        {
            param_src = &param_src1;
            fourier_info1 ( "Pas de deuxième image" );
        }

        /* TFD */
        Fourier::Parametres fourier_reel1, fourier_imag1;
        fourier_reel1.init( param_src->largeur, param_src->hauteur, Fourier::REGULAR, Fourier::REAL );
        fourier_imag1.init( param_src->largeur, param_src->hauteur, Fourier::REGULAR, Fourier::IMAG );
        Fourier::tfd_2d_directe_simple( &param_src1, &fourier_reel1, &fourier_imag1 );

        Fourier::Parametres fourier_reel2, fourier_imag2;
        if ( src_2 != 0 )
        {
            fourier_reel2.init( param_src->largeur, param_src->hauteur, Fourier::REGULAR, Fourier::REAL );
            fourier_imag2.init( param_src->largeur, param_src->hauteur, Fourier::REGULAR, Fourier::IMAG );
            Fourier::tfd_2d_directe_simple( &param_src2, &fourier_reel2, &fourier_imag2 );
        }
        else
        {
            fourier_reel2.copie( fourier_reel1 );
            fourier_imag2.copie( fourier_imag1 );
        }

        /* Multiplication complexe des 2 images */
        Fourier::Parametres fourier_reel0, fourier_imag0;
        fourier_reel0.init( param_src->largeur, param_src->hauteur, Fourier::REGULAR, Fourier::REAL );
        fourier_imag0.init( param_src->largeur, param_src->hauteur, Fourier::REGULAR, Fourier::IMAG );
        if ( op == Fourier::CORRELATION )
            Fourier::produit_complexe( &fourier_reel1, &fourier_imag1, &fourier_reel2, &fourier_imag2, &fourier_reel0, &fourier_imag0, Fourier::CONJUGATE );
        else // op == Fourier::CONVOLUTION
            Fourier::produit_complexe( &fourier_reel1, &fourier_imag1, &fourier_reel2, &fourier_imag2, &fourier_reel0, &fourier_imag0, Fourier::STANDARD );

        /* TFD inverse */
        Fourier::Parametres param_dest;
        param_dest.init( param_src->largeur, param_src->hauteur, Fourier::NO_ORDER, Fourier::NO_TYPE );
        param_dest.keywords( param_src->keywords() );
        Fourier::tfd_2d_inverse_simple( &fourier_reel0, &fourier_imag0, &param_dest );

        /* Normalisation et re-arrangement */
        if ( normalisation )
            Fourier::normalisation( &param_dest, minimum, maximum, ordre );
        else
            Fourier::normalisation( &param_dest, 0, 0, ordre );


        /* Sauvegarde */
        ecriture_image( dest, &param_dest );
#if 0
        fourier_info1 ( dest << " taille " << param_dest.largeur << "x" << param_dest.hauteur );
        int format_stockage = FLOAT_IMG;    /* Format en virgule flottante */
        param_dest.keywords()->Add( "BITPIX", &format_stockage, TINT, "", "" );
        TYPE_PIXELS * d = (TYPE_PIXELS *)malloc( param_dest.pixels()->taille() );
        memcpy( d, param_dest.pixels()->pointeur(), param_dest.pixels()->taille() );
        CPixelsGray * pix_dest = new CPixelsGray( param_dest.largeur, param_dest.hauteur, FORMAT_FLOAT, d, 0, 0 );
        CFile::saveFits( (char *)dest, 0, pix_dest, param_dest.keywords() );

        /* Nettoyage */
        free( d );
        delete pix_dest;
#endif
    }
    catch ( const CError& e )
    {
        throw e;
    }
}

}


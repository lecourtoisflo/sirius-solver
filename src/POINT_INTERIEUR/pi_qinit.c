// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************************

   FONCTION: Initialisations

                   
   AUTEUR: R. GONZALEZ

************************************************************************************/

# include "pi_sys.h"

# include "pi_fonctions.h"
# include "pi_define.h"

# define VALEUR_MIN_POUR_S  1.e+1
# define VALEUR_MAX_POUR_S  1.e+3
# define ECART_U            1.e+3

/*------------------------------------------------------------------------*/
/*                          Initialisation                                */ 

void PI_Qinit( PROBLEME_PI * Pi ,
               double ToleranceAdmissibilite_E , 
               int   ChoixToleranceAdmissibiliteParDefaut, 	       
							 double ToleranceStationnarite_E , 
               int   ChoixToleranceStationnariteParDefaut, 	       
							 double ToleranceComplementarite_E , 
               int   ChoixToleranceComplementariteParDefaut )
{
int i  ; double TestBornes; int NbBor; int NbBorInf; int NbBorSup;
double X; int   NbDiv     ; int Cnt  ;

NbBor    = 0;
NbBorInf = 0;
NbBorSup = 0;
for ( i = 0 ; i < Pi->NombreDeVariables ; i++ ) {
  if ( Pi->TypeDeVariable[i] == BORNEE ) NbBor++;
  else if ( Pi->TypeDeVariable[i] == BORNEE_INFERIEUREMENT ) NbBorInf++;
  else if ( Pi->TypeDeVariable[i] == BORNEE_SUPERIEUREMENT ) NbBorSup++;
}

NbDiv = ( ( 2. * NbBor) + NbBorInf + NbBorSup );
if ( NbDiv != 0 ) {
  Pi->Ro = 1. / (double) NbDiv;
}
else Pi->Ro = 1.0;

if ( ChoixToleranceAdmissibiliteParDefaut != OUI_PI && 
     ChoixToleranceAdmissibiliteParDefaut != NON_PI ) {
  printf(" Point interieur, fournir un type de tolerance pour l admissibilite \n"); 
  Pi->AnomalieDetectee = OUI_PI;
  longjmp( Pi->Env , Pi->AnomalieDetectee ); 
}
if ( ChoixToleranceStationnariteParDefaut != OUI_PI && 
     ChoixToleranceStationnariteParDefaut != NON_PI ) {
  printf(" Point interieur, fournir un type de tolerance pour la stationnarite \n"); 
  Pi->AnomalieDetectee = OUI_PI;
  longjmp( Pi->Env , Pi->AnomalieDetectee ); 
}
if ( ChoixToleranceComplementariteParDefaut != OUI_PI && 
     ChoixToleranceComplementariteParDefaut != NON_PI ) {
  printf(" Point interieur, fournir un type de tolerance pour la complementarite \n"); 
  Pi->AnomalieDetectee = OUI_PI;
  longjmp( Pi->Env , Pi->AnomalieDetectee ); 
}

if ( ChoixToleranceAdmissibiliteParDefaut == OUI_PI ) {
  Pi->SeuilDAdmissibilite = TOLERANCE_ADMISSIBILITE_PAR_DEFAUT;
}
else {
  Pi->SeuilDAdmissibilite =ToleranceAdmissibilite_E;
}

if ( ChoixToleranceStationnariteParDefaut == OUI_PI ) {
  Pi->SeuilDeStationnarite = TOLERANCE_STATIONNARITE_PAR_DEFAUT;
}
else {
  Pi->SeuilDeStationnarite = ToleranceStationnarite_E;
}

if ( ChoixToleranceComplementariteParDefaut == OUI_PI ) {
  Pi->SeuilDOptimalite = TOLERANCE_COMPLEMENTARITE_PAR_DEFAUT;
}
else {
  Pi->SeuilDOptimalite = ToleranceComplementarite_E; 
}

Pi->Smin = 0.0;

Pi->Resolution = BON;
Pi->Range      = RANGE;

memset( (char *) Pi->DeltaUDeltaS1 , 0 , Pi->NombreDeVariables   * sizeof( double ) ); 
memset( (char *) Pi->DeltaUDeltaS2 , 0 , Pi->NombreDeVariables   * sizeof( double ) ); 
memset( (char *) Pi->Lambda        , 0 , Pi->NombreDeContraintes * sizeof( double ) ); 
memset( (char *) Pi->Lambda0       , 0 , Pi->NombreDeContraintes * sizeof( double ) );

memset( (char *) Pi->RegulVar        , 0 , Pi->NombreDeVariables   * sizeof( double ) ); 
memset( (char *) Pi->RegulContrainte , 0 , Pi->NombreDeContraintes * sizeof( double ) );

TestBornes = 1.e-8;
for ( i = 0 ; i < Pi->NombreDeVariables ; i++ ) {
  if ( Pi->TypeDeVariable[i] == BORNEE ) {
    if ( ( Pi->Umax[i] - Pi->Umin[i] ) < TestBornes ) {
      printf(" Point interieur, borne min = borne max sur la variables %d \n", i);
      printf("    borne min = %lf    borne max = %lf \n", Pi->Umin[i], Pi->Umax[i]);
      Pi->AnomalieDetectee = OUI_PI;
      longjmp( Pi->Env , Pi->AnomalieDetectee ); 
    }
		
		/*Pi->U[i] = 0.5 * ( Pi->Umax[i] + Pi->Umin[i] );*/
		/* On prend le plus petit entre X/2 et racine carr�e de X */		
	 	X = Pi->Umax[i] - Pi->Umin[i];
	 	if ( X > 4. ) Pi->U[i] = Pi->Umin[i] + sqrt( X );
	  else Pi->U[i] = Pi->Umin[i] + ( 0.5 * X );
				
    X = fabs( Pi->L[i] );
    if ( X >= VALEUR_MIN_POUR_S && X <= VALEUR_MAX_POUR_S ) {
      Pi->S1[i] = X;
      Pi->S2[i] = X;            
    }
    else if ( X < VALEUR_MIN_POUR_S ) {
      Pi->S1[i] = VALEUR_MIN_POUR_S;
      Pi->S2[i] = VALEUR_MIN_POUR_S;    
    }
    else {
      Pi->S1[i] = VALEUR_MAX_POUR_S;
      Pi->S2[i] = VALEUR_MAX_POUR_S;      
    }				
  }
  else if ( Pi->TypeDeVariable[i] == BORNEE_INFERIEUREMENT ) {
    Pi->U[i] = Pi->Umin[i] + ECART_U;
    X = fabs( Pi->L[i] );    
    if ( X >= VALEUR_MIN_POUR_S && X <= VALEUR_MAX_POUR_S ) Pi->S1[i] = X;
    else if ( X < VALEUR_MIN_POUR_S ) Pi->S1[i] = VALEUR_MIN_POUR_S;
    else Pi->S1[i] = VALEUR_MAX_POUR_S;
    Pi->S2[i] = 0.;		
  }
  else if ( Pi->TypeDeVariable[i] == BORNEE_SUPERIEUREMENT ) {
    Pi->U[i]  = Pi->Umax[i] - ECART_U;
    Pi->S1[i] = 0.;
    X = fabs( Pi->L[i] );    
    if ( X >= VALEUR_MIN_POUR_S  && X <= VALEUR_MAX_POUR_S ) Pi->S2[i] = X;               
    else if ( X < VALEUR_MIN_POUR_S ) Pi->S2[i] = VALEUR_MIN_POUR_S;
    else Pi->S2[i] = VALEUR_MAX_POUR_S;		
  }
  else if ( Pi->TypeDeVariable[i] == NON_BORNEE ) {
    Pi->U[i]  = 0.;
    Pi->S1[i] = 0.;
    Pi->S2[i] = 0.;
  }
  else printf("Bug dans le point interieur, type de variable %d non reconnu\n",i); 
}

memcpy( (char *) Pi->U0 , (char *) Pi->U , Pi->NombreDeVariables * sizeof( double ) );

return;
}
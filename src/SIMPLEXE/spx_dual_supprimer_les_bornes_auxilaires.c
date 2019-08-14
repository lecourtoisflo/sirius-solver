// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: On essai de remettre les bornes initiales tout en restant
	           dual realisable 
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_define.h"
# include "spx_fonctions.h"  

# ifdef UTILISER_BORNES_AUXILIAIRES
/*----------------------------------------------------------------------------*/

void SPX_DualSupprimerLesBornesAuxiliaires( PROBLEME_SPX * Spx )
{
int Var; int i; int * NumerosDesVariablesHorsBase; double * CBarre; 
char * StatutBorneSupCourante; char * PositionDeLaVariable;
int NombreDeBornesAuxilairesAuDepart; double * SeuilDAmissibiliteDuale;

NumerosDesVariablesHorsBase = Spx->NumerosDesVariablesHorsBase;
CBarre                      = Spx->CBarre;
StatutBorneSupCourante      = Spx->StatutBorneSupCourante;
PositionDeLaVariable        = Spx->PositionDeLaVariable;
  
NombreDeBornesAuxilairesAuDepart = Spx->NombreDeBornesAuxiliairesUtilisees;

SeuilDAmissibiliteDuale = Spx->SeuilDAmissibiliteDuale2;

/* Boucle sur les variables hors base */
for ( i = 0 ; i < Spx->NombreDeVariablesHorsBase ; i++ ) {
  Var = NumerosDesVariablesHorsBase[i];		
	if ( StatutBorneSupCourante[Var] == BORNE_NATIVE ) continue;	

  if ( StatutBorneSupCourante[Var] == BORNE_AUXILIAIRE_DE_VARIABLE_BORNEE_INFERIEUREMENT ) {      		
    /* Si la variable est devenue duale admissible avec sa borne native on peut la passer sur borne inf
		   et remettre son type de borne initial */
		if ( CBarre[Var] > -SeuilDAmissibiliteDuale[Var] ) {
      SPX_SupprimerUneBorneAuxiliaire( Spx, Var );				
      PositionDeLaVariable[Var] = HORS_BASE_SUR_BORNE_INF;		
		}
  }				
	else if ( StatutBorneSupCourante[Var] == BORNE_AUXILIAIRE_DE_VARIABLE_NON_BORNEE ) {
    if ( fabs( CBarre[Var] ) < SeuilDAmissibiliteDuale[Var] ) {
      SPX_SupprimerUneBorneAuxiliaire( Spx, Var );		
      PositionDeLaVariable[Var] = HORS_BASE_A_ZERO;
		}
	}	
}

if ( Spx->NombreDeBornesAuxiliairesUtilisees != NombreDeBornesAuxilairesAuDepart ) {
  Spx->CalculerBBarre = OUI_SPX;
  if (Spx->spx_params->VERBOSE_SPX) {
	  printf("Iteration %d Nombre de bornes auxiliaires utilisees %d\n", Spx->Iteration, Spx->NombreDeBornesAuxiliairesUtilisees);
  }
}

return;
}

# endif

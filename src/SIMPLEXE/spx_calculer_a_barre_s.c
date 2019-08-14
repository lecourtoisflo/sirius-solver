// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Calcul de A_BARRE_S = B-1 * AS
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"   

# include "lu_define.h"

/*----------------------------------------------------------------------------*/
void SPX_TenterRestaurationCalculABarreSEnHyperCreux( PROBLEME_SPX * Spx )
{
double * ABarreS; int Count; int iMx; int i;
	
if ( Spx->CountEchecsABarreS == 0 ) {
  if ( Spx->Iteration % Spx->spx_params->CYCLE_TENTATIVE_HYPER_CREUX == 0 ) {
		Spx->NbEchecsABarreS    = Spx->spx_params->SEUIL_REUSSITE_CREUX;
		Spx->CountEchecsABarreS = Spx->spx_params->SEUIL_REUSSITE_CREUX + 2;
	}
}
if ( Spx->CountEchecsABarreS == 0 ) return;

Spx->CountEchecsABarreS--;		
/* On compte le nombre de termes non nuls du resultat */
ABarreS = Spx->ABarreS;
Count = 0;

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  iMx = Spx->RangDeLaMatriceFactorisee;
}
else {
  iMx = Spx->NombreDeContraintes;
}

for ( i = 0 ; i < iMx ; i++ ) if ( ABarreS[i] != 0.0 ) Count++;

if ( Count < 0.1 * iMx ) Spx->NbEchecsABarreS--;
if ( Spx->NbEchecsABarreS <= 0 ) {
	if (Spx->spx_params->VERBOSE_SPX) {
		printf("Remise en service de l'hyper creux pour le calcul de ABarreS, iteration %d\n", Spx->Iteration);
	}
  Spx->AvertissementsEchecsABarreS = 0;
  Spx->CountEchecsABarreS = 0;
	Spx->CalculABarreSEnHyperCreux = OUI_SPX;
}
else if ( Spx->CountEchecsABarreS <= 0 ) {
  Spx->CountEchecsABarreS = 0;
  if ( Spx->CalculABarreSEnHyperCreux == NON_SPX ) Spx->AvertissementsEchecsABarreS ++;
  if ( Spx->AvertissementsEchecsABarreS >= Spx->spx_params->SEUIL_ABANDON_HYPER_CREUX ) {
	  if (Spx->spx_params->VERBOSE_SPX) {
		  printf("Arret prolonge de l'hyper creux pour le calcul de ABarreS, iteration %d\n", Spx->Iteration);
	  }
	  Spx->CalculABarreSEnHyperCreuxPossible = NON_SPX;		
	}
}

return;
}

/*----------------------------------------------------------------------------*/

void SPX_CalculerABarreS( PROBLEME_SPX * Spx )
{
double * ABarreS; char OK; int IndexBase; int * CntDeABarreSNonNuls; int i;  char TypeDEntree;
char TypeDeSortie; char CalculEnHyperCreux; char HyperCreuxInitial;

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  SPX_CalculerABarreSAvecBaseReduite( Spx, &HyperCreuxInitial, &CalculEnHyperCreux, &TypeDEntree, &TypeDeSortie );
}
else {
  SPX_CalculerABarreSAvecBaseComplete( Spx, &HyperCreuxInitial, &CalculEnHyperCreux, &TypeDEntree, &TypeDeSortie );
}

ABarreS = Spx->ABarreS;
CntDeABarreSNonNuls = Spx->CntDeABarreSNonNuls;
								 
if ( CalculEnHyperCreux == OUI_SPX ) {
  if ( TypeDeSortie != COMPACT_LU ) {
    CalculEnHyperCreux = NON_SPX;
    /* Ca s'est pas bien passe et on s'est forcement retrouve en VECTEUR_LU */		
		Spx->NbEchecsABarreS++;
		/*printf("Echec hyper creux ABarreS iteration %d\n",Spx->Iteration);*/
		if ( Spx->NbEchecsABarreS >= Spx->spx_params->SEUIL_ECHEC_CREUX ) {
			if (Spx->spx_params->VERBOSE_SPX) {
				printf("Arret de l'hyper creux pour le calcul de ABarreS, iteration %d\n", Spx->Iteration);
			}
		  Spx->CalculABarreSEnHyperCreux = NON_SPX;
			Spx->CountEchecsABarreS  = 0;
		}		
  }
  else Spx->NbEchecsABarreS = 0;
}

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  IndexBase = Spx->OrdreColonneDeLaBaseFactorisee[Spx->ContrainteDeLaVariableEnBase[Spx->VariableSortante]];
}
else {
  IndexBase = Spx->ContrainteDeLaVariableEnBase[Spx->VariableSortante];
}
										 
if ( CalculEnHyperCreux == OUI_SPX ) {
	Spx->TypeDeStockageDeABarreS = COMPACT_SPX;
  OK = NON_SPX;
  for ( i = 0 ; i < Spx->NbABarreSNonNuls ; i++ ) {
	  if ( CntDeABarreSNonNuls[i] == IndexBase ) {
      Spx->ABarreSCntBase = ABarreS[i];
			OK = OUI_SPX;
      break;
		}
	}
	if ( OK == NON_SPX ) {
	  /* Pb d'epsilon, on prend la aleur de NBarreR */
    Spx->ABarreSCntBase = Spx->NBarreR[Spx->VariableEntrante]; /* Valable que l'on soit en base reduite ou non */
	}	
}
else {
  Spx->TypeDeStockageDeABarreS = VECTEUR_SPX;	
  Spx->ABarreSCntBase = ABarreS[IndexBase];
  /* Si on est pas en hyper creux, on essaie d'y revenir */
	if ( HyperCreuxInitial == NON_SPX ) {
    if ( Spx->CalculABarreSEnHyperCreux == NON_SPX ) {
      if ( Spx->CalculABarreSEnHyperCreuxPossible == OUI_SPX ) {
        SPX_TenterRestaurationCalculABarreSEnHyperCreux( Spx );
	    }
    }   
	}
	
}

return;
}

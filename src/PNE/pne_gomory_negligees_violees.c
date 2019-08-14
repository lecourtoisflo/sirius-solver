// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Detection des coupes de knapsack negliges violees.
                 
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"  
  
# include "pne_fonctions.h"
# include "pne_define.h"

# include "spx_define.h"
  
# include "bb_define.h"

# ifdef PNE_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "pne_memoire.h"
# endif

# define PROFONDEUR_LIMITE_COUPES_DE_G 10

/*----------------------------------------------------------------------------*/

void PNE_DetectionGomoryNegligeesViolees( PROBLEME_PNE * Pne )
{
double S; int Cnt; double E; int * First; int * NbElements; int NbV; double NormeV;
int il; int ilMax; double * Coeff; int * Indice; double B; int NbT; double * X;
int * Colonne; char * LaCoupeEstDansLePool; double * SecondMembre;
double * Coefficient; COUPES_G_NEGLIGEES * CoupesGNegligees; double Marge;
int * TypeDeBorne; BB * Bb;

Marge = 0.1;
   
Bb = Pne->ProblemeBbDuSolveur;
if ( Bb->NoeudEnExamen->ProfondeurDuNoeud > PROFONDEUR_LIMITE_COUPES_DE_G ) return;

if ( Pne->CoupesGNegligees == NULL ) return;
CoupesGNegligees = Pne->CoupesGNegligees;

TypeDeBorne = Pne->TypeDeBorneTrav;
X = Pne->UTrav;
Coeff = Pne->Coefficient_CG;
Indice = Pne->IndiceDeLaVariable_CG;

First = CoupesGNegligees->First;
LaCoupeEstDansLePool = CoupesGNegligees->LaCoupeEstDansLePool;
NbElements = CoupesGNegligees->NbElements;
SecondMembre = CoupesGNegligees->SecondMembre;
Colonne = CoupesGNegligees->Colonne;
Coefficient = CoupesGNegligees->Coefficient;
	 
NbV = 0;
NormeV = 0.0;

for ( Cnt = 0 ; Cnt < CoupesGNegligees->NombreDeCoupes ; Cnt++ ) {
  if ( First[Cnt] < 0 ) continue;	
  if ( LaCoupeEstDansLePool[Cnt] == OUI_PNE ) continue;	
  il = First[Cnt];
	ilMax = il + NbElements[Cnt];
	S = 0;
	NbT = 0;
	B = CoupesGNegligees->SecondMembre[Cnt];
	while ( il < ilMax ) {
	  if ( TypeDeBorne[Colonne[il]] != VARIABLE_FIXE ) {
      /* Car sinon il n'y a pas de variable correspondante dans le simplexe */	
      S += Coefficient[il] * X[Colonne[il]];
		  Coeff[NbT] = Coefficient[il];		 
	    Indice[NbT] = Colonne[il];				
		  NbT++;
		}
		else {
      B -= Coefficient[il] * X[Colonne[il]];
		}		
		il++;
	}
  if ( S > B + Marge && NbT > 0 ) {
    /*
    printf("NbT %d depassement %e\n",NbT,S-B);
		for ( il = 0 ; il < NbT ; il++ ) {
		  printf(" %e (%d) ",Coeff[il],Indice[il]);
		}
		printf("< %e\n",B);
	  */
	  E = S - B;
	  NormeV += E;
    /* On Stocke la coupe */
		NbV++;
    PNE_EnrichirLeProblemeCourantAvecUneCoupe( Pne, 'G', NbT, B, E, Coeff, Indice );
    Pne->CoupesCalculees[Pne->NombreDeCoupesCalculees-1]->IndexDansGNegligees = Cnt; 
	}
}

if ( Pne->pne_params->AffichageDesTraces == OUI_PNE &&  NbV > 0 ) {
  printf("Adding %d initially neglected Gomory cuts violated by %e\n",NbV,NormeV);
	fflush( stdout );
}

return;
}   















// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Creation des noeuds fis apres evaluation du pere
       
   AUTEUR: R. GONZALEZ

************************************************************************/

#include "bb_sys.h"
#include "bb_define.h"
#include "bb_fonctions.h"

#include "pne_define.h"
#include "pne_fonctions.h"

/*-----------------------------------------------------------------------------------------------*/

void BB_CreerLesNoeudsFils( BB * Bb, NOEUD * NoeudCourant )
{  
NOEUD * Noeud; int CodeRetour    ; int FilsACreer; 
int ValeurDInstanciationAGauche; int NombreDeVariablesAInstancierAGauche;
int * NumerosDesVariablesAInstancierAGauche;
int ValeurDInstanciationADroite; int NombreDeVariablesAInstancierADroite;
int * NumerosDesVariablesAInstancierADroite;
#if VERBOSE_BB
  int i;
#endif

CodeRetour = BB_ChoixDesVariablesAInstancier( Bb,
					                                    NoeudCourant,
                                              &ValeurDInstanciationAGauche,
                                              &NombreDeVariablesAInstancierAGauche,
                                              &NumerosDesVariablesAInstancierAGauche,
				                                      &ValeurDInstanciationADroite,
                                              &NombreDeVariablesAInstancierADroite,
                                              &NumerosDesVariablesAInstancierADroite );    
					      
if ( CodeRetour == PAS_DE_VARIABLES_A_INSTANCIER ) {
  BB_DesallocationPartielleDUnNoeud( NoeudCourant );
  return;
}

#if VERBOSE_BB
  printf("Valeur d'instanciation a gauche -> %d\n",ValeurDInstanciationAGauche);
  printf("Instanciation a gauche de la variable ->");
  for ( i = 0 ; i < NombreDeVariablesAInstancierAGauche ; i++ ) {
    printf(" %d ",NumerosDesVariablesAInstancierAGauche[i]);
  }
  printf("\n");
  printf("Valeur d'instanciation a droite -> %d\n",ValeurDInstanciationADroite);
  printf("Instanciation a droite de la variable ->",NumerosDesVariablesAInstancierADroite[i]);
  for ( i = 0 ; i < NombreDeVariablesAInstancierADroite ; i++ ) {
    printf(" %d ",NumerosDesVariablesAInstancierADroite[i]);
  }
  printf("\n");  
#endif
  
/* Si le noeud courant a deja son fils NoeudSuivantGauche, on le cree pas */
Noeud = NoeudCourant->NoeudSuivantGauche; 
if ( Noeud == 0 ) {
                            /* Noeud gauche */
  FilsACreer = FILS_GAUCHE;
  Noeud = BB_AllouerUnNoeud( Bb,
			                       NoeudCourant,
                             NoeudCourant->ProfondeurDuNoeud + 1,
			                       FilsACreer,
                             ValeurDInstanciationAGauche,
			                       NombreDeVariablesAInstancierAGauche,
			                       NumerosDesVariablesAInstancierAGauche, 
                             Bb->MinorantEspereAEntierInf );
  NoeudCourant->NoeudSuivantGauche = Noeud;
} 

/* Si le noeud courant a deja son fils NoeudSuivantDroit, on le cree pas */
Noeud = NoeudCourant->NoeudSuivantDroit; 
if ( Noeud == 0 ) {
                            /* Noeud droit */
  FilsACreer = FILS_DROIT;
  Noeud = BB_AllouerUnNoeud( Bb,
			                       NoeudCourant,                         
                             NoeudCourant->ProfondeurDuNoeud + 1,
			                       FilsACreer,
                             ValeurDInstanciationADroite,
			                       NombreDeVariablesAInstancierADroite,
			                       NumerosDesVariablesAInstancierADroite, 
                             Bb->MinorantEspereAEntierSup );
  NoeudCourant->NoeudSuivantDroit = Noeud;
}

return;
}                        



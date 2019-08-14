// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Marquage des contraintes mixtes sur lesquelles on peut
	           faire des MIR.
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"

# include "pne_fonctions.h"
# include "pne_define.h"

# ifdef PNE_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "pne_memoire.h"
# endif     

# define ZERO_COEFF 1.e-8

/*----------------------------------------------------------------------------*/

void PNE_DetecterLesContraintesMixtes( PROBLEME_PNE * Pne )
{
int Cnt; int NbCont; int NbBin; int il; int ilMax; int Var; char YaDesContraintesMixtes;
int * MdebTrav; int * NbTermTrav; int * NuvarTrav; int * TypeDeBorneTrav; double * ATrav;
int * TypeDeVariableTrav; char * ContrainteMixte; char * SensContrainteTrav; double * BTrav;
int * CntDeBorneSupVariable; int * CntDeBorneInfVariable; int NbBinPotentiel; double Ai;
int * FoisCntSuccesMirMarchandWolseyTrouvees;

if ( Pne->ContrainteMixte == NULL ) {   
  Pne->ContrainteMixte  = (char *) malloc( Pne->NombreDeContraintesTrav * sizeof( char ) );
  if ( Pne->ContrainteMixte == NULL ) {
    printf(" Solveur PNE , memoire insuffisante. Sous-programme: PNE_AnalyseInitialeDesBornesVariables \n");
    Pne->AnomalieDetectee = OUI_PNE;
    longjmp( Pne->Env , Pne->AnomalieDetectee ); /* rq: le 2eme argument ne sera pas utilise */
  }	
}
if ( Pne->FoisCntSuccesMirMarchandWolseyTrouvees == NULL ) {
  Pne->FoisCntSuccesMirMarchandWolseyTrouvees  = (int *) malloc( Pne->NombreDeContraintesTrav * sizeof( int ) );
  if ( Pne->FoisCntSuccesMirMarchandWolseyTrouvees == NULL ) {
    printf(" Solveur PNE , memoire insuffisante. Sous-programme: PNE_AnalyseInitialeDesBornesVariables \n");
    Pne->AnomalieDetectee = OUI_PNE;
    longjmp( Pne->Env , Pne->AnomalieDetectee ); /* rq: le 2eme argument ne sera pas utilise */
  }
}  

CntDeBorneSupVariable = Pne->CntDeBorneSupVariable;
CntDeBorneInfVariable =	Pne->CntDeBorneInfVariable;

BTrav = Pne->BTrav;
SensContrainteTrav = Pne->SensContrainteTrav;
MdebTrav = Pne->MdebTrav;
NbTermTrav = Pne->NbTermTrav;   
NuvarTrav = Pne->NuvarTrav;
ATrav = Pne->ATrav;
TypeDeBorneTrav = Pne->TypeDeBorneTrav;
TypeDeVariableTrav = Pne->TypeDeVariableTrav;

YaDesContraintesMixtes = NON_PNE;
ContrainteMixte = Pne->ContrainteMixte;
FoisCntSuccesMirMarchandWolseyTrouvees = Pne->FoisCntSuccesMirMarchandWolseyTrouvees;

for ( Cnt = 0 ; Cnt < Pne->NombreDeContraintesTrav ; Cnt++ ) {
  ContrainteMixte[Cnt] = NON_PNE;
  FoisCntSuccesMirMarchandWolseyTrouvees[Cnt] = 0;
  /* Ne pas compter les contraintes de bornes variables */
  if (  NbTermTrav[Cnt] <= 1 ) continue;  
	if ( SensContrainteTrav[Cnt] == '<' && NbTermTrav[Cnt] == 2 && BTrav[Cnt] == 0.0 ) continue;	
	NbCont = 0;
	/*if ( SensContrainteTrav[Cnt] == '<' ) NbCont++;*/
	NbBin = 0;
	NbBinPotentiel = 0;
	il = MdebTrav[Cnt];
  ilMax = il + NbTermTrav[Cnt];
  while ( il < ilMax ) {    
    Var = NuvarTrav[il];
		Ai = fabs( ATrav[il] );
		if ( Ai > ZERO_COEFF ) {
      if ( TypeDeBorneTrav[Var] != VARIABLE_FIXE ) {
        if ( TypeDeVariableTrav[Var] == ENTIER ) NbBin++;
        else {
				  NbCont++;
					if ( CntDeBorneSupVariable != NULL && CntDeBorneInfVariable != NULL ) {
					  if ( CntDeBorneSupVariable[Var] >= 0 ) NbBinPotentiel++;
						else if ( CntDeBorneInfVariable[Var] >= 0 ) NbBinPotentiel++;
					}
				}
			}
		}
    il++;
  }	
	if ( NbCont > 0 && ( NbBin > 0 || NbBinPotentiel > 0 ) ) {
	  YaDesContraintesMixtes = OUI_PNE;
		ContrainteMixte[Cnt] = OUI_PNE;
	}
}
if ( YaDesContraintesMixtes == NON_PNE ) {
  free( Pne->ContrainteMixte );
	Pne->ContrainteMixte = NULL;
  free( Pne->FoisCntSuccesMirMarchandWolseyTrouvees );
	Pne->FoisCntSuccesMirMarchandWolseyTrouvees = NULL;	
}

return;
}
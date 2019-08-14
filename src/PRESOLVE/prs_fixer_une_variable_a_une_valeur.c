// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Init des indicateurs de fixation d'une variable.					 
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "prs_sys.h"

# include "prs_fonctions.h"
# include "prs_define.h"

# include "pne_define.h"
		
# ifdef PRS_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "prs_memoire.h"
# endif

/*----------------------------------------------------------------------------*/

void PRS_FixerUneVariableAUneValeur( PRESOLVE * Presolve, int Var, double X )   
{
PROBLEME_PNE * Pne;
Pne = (PROBLEME_PNE *) Presolve->ProblemePneDuPresolve;
Pne->TypeDeVariableTrav[Var] = REEL;
Presolve->ValeurDeXPourPresolve[Var] = X;
/* Car on se sert de ces bornes dans le postsolve 
Presolve->BorneInfPourPresolve[Var] = X;
Presolve->BorneSupPourPresolve[Var] = X;							
*/
Presolve->TypeDeBornePourPresolve[Var] = VARIABLE_FIXE;
Presolve->TypeDeValeurDeBorneInf[Var] = VALEUR_IMPLICITE;
Presolve->TypeDeValeurDeBorneSup[Var] = VALEUR_IMPLICITE;
return;
}

/*----------------------------------------------------------------------------*/
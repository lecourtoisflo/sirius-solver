// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Resolution du systeme transpose pour obtenir une ligne
             de l'inverse de la base    

   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"   

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"

/*----------------------------------------------------------------------------*/
/* Calcul de la ligne de B^{-1} de la variable sortante */
void SPX_CalculerErBMoins1( PROBLEME_SPX * Spx, char CalculEnHyperCreux ) 
{

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  SPX_CalculerErBMoins1AvecBaseReduite( Spx, CalculEnHyperCreux );
}
else {
  SPX_CalculerErBMoins1AvecBaseComplete( Spx, CalculEnHyperCreux );
}
										 
return;
}
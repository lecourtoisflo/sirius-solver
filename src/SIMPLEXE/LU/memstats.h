// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#ifndef __MEMSTATS_H__
# define __MEMSTATS_H__

# include <stddef.h> /* for size_t */

 


/*!
** \brief Structure contenant des statistiques sur la memoire a un instant donne
*/
typedef struct
{
	/*! Quantite de Memoire totale de la machine (en octets) */
	size_t total;
	/*! Quantite de Memoire disponible (en octets) */
	size_t libre;

} MemoireInfo;




# ifdef __cplusplus
extern "C"
{
# endif


/*!
** \brief Recupere toutes les informations sur la memoire
**
** Dans tous les cas, la structure donnee est initialisee (avec des
** valeurs par defaut en cas d'erreur).
**
** \param[out] info Structure ou seront stockees les informations
** recoltees
** \return 0 en cas de reussite, une valeur quelconque en cas d'erreur
*/
int MemoireStatistiques(MemoireInfo* info);



/*!
** \brief Donne la quantite de memoire actuellement disponible
** \return Une taille en octets
*/
size_t MemoireDisponible( void );

/*!
** \brief Donne la quantite de Memoire totale de la machine
** \return Une taille en octets
*/
size_t MemoireTotale( void );


# ifdef __cplusplus
}
# endif

#endif /* __MEMSTATS_H__ */
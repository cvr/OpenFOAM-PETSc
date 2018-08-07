/***********************************************************************************
 * Header file defining PETScOpenFOAM solver types. Includes relevant PETSc headers
 *
 * Authors: Timothy Costa (Intel Corp.)
 * Date Modified: 8/7/2018
 * *********************************************************************************/

#ifndef _PETScOpenFOAM_PLUGIN_H_
#define _PETScOpenFOAM_PLUGIN_H_

#include "lduMatrix.H"

#include "petscsys.h"
#include "petscmat.h"
#include "petscvec.h"
#include "petscis.h"
#include "petscksp.h"
#include "petscpc.h"

enum PETScOpenFOAMPreconditionerType {
	PETScOpenFOAMNone	= 0,
	PETScOpenFOAMICC	= 1,
	PETScOpenFOAMILU	= 2
};

enum PETScOpenFOAMSolverType {
	PETScOpenFOAMCG		= 0
};

#endif

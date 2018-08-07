/***********************************************************************************
 * Header file defining PETScOpenFOAM CG solver. 
 *
 * Authors: Timothy Costa (Intel Corp.)
 * Date Modified: 8/7/2018
 * *********************************************************************************/

#ifndef _PETScOpenFOAMCG_H_
#define _PETScOpenFOAMCG_H_

#include "PETScOpenFOAM.H"

namespace Foam
{

class PETScOpenFOAMCG : public lduMatrix::solver
{

	PETScOpenFOAMCG(const Foam::PETScOpenFOAMCG&);

	void operator=(const Foam::PETScOpenFOAMCG);

public:

	TypeName("PETScOpenFOAMCG");

	PETScOpenFOAMCG
        (
		const word& fieldName,
        	const lduMatrix& matrix,
	        const FieldField<Field, scalar>& interfaceBouCoeffs,
        	const FieldField<Field, scalar>& interfaceIntCoeffs,
        	const lduInterfaceFieldPtrsList& interfaces,
        	const dictionary& solverControls
    	);

	virtual ~PETScOpenFOAMCG() {}

	virtual solverPerformance solve
	(
		scalarField& psi,
		const scalarField& source,
		const direction cmpt=0
	) const;

};

}

#endif

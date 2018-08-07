/***********************************************************************************
 * Source for PETScOpenFOAM CG solver
 *
 * Authors: Timothy Costa (Intel Corp.)
 * Date Modified: 8/7/2018
 * *********************************************************************************/

#include "../include/PETScOpenFOAM.H"
#include "../include/PETScOpenFOAMCG.H"
#include "PETScOpenFOAMCommon.CXX"

namespace Foam
{
	defineTypeNameAndDebug(PETScOpenFOAMCG, 0);

	lduMatrix::solver::addsymMatrixConstructorToTable<PETScOpenFOAMCG>
	addPETScOpenFOAMCGSymMatrixConstructorToTable_;
}

Foam::PETScOpenFOAMCG::PETScOpenFOAMCG
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const dictionary& solverControls
)
:
    lduMatrix::solver
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces,
        solverControls
    )
{}

Foam::solverPerformance Foam::PETScOpenFOAMCG::solve
(
    scalarField& psi,
    const scalarField& source,
    const direction cmpt
) const
{

    word precond_name = lduMatrix::preconditioner::getName(controlDict_);
    int pLevels     = controlDict_.lookupOrDefault<int>("pLevels", 0);

    solverPerformance solverPerf(typeName + '(' + precond_name + ')', fieldName_);

    // WIP

}

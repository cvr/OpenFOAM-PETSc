/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | faSavageHutterFOAM
    \\  /    A nd           | Copyright (C) 2017 Matthias Rauter
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::frictionModels::Voellmy

Description
    An incompressible Voellmy friction model.

SourceFiles
    Voellmy.C

Author
    Matthias Rauter matthias.rauter@uibk.ac.at

\*---------------------------------------------------------------------------*/

#ifndef Voellmy_H
#define Voellmy_H

#include "frictionModel.H"
#include "dimensionedScalar.H"
#include "volFields.H"
#include "IOdictionary.H"
#include "typeInfo.H"
#include "runTimeSelectionTables.H"
#include "dimensionedScalar.H"
#include "tmp.H"
#include "autoPtr.H"
#include "faMatrices.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace frictionModels
{

/*---------------------------------------------------------------------------*\
                           Class Voellmy Declaration
\*---------------------------------------------------------------------------*/

class Voellmy
:
    public frictionModel
{
    // Private data

        //- Coulomb friction parameter m
        dimensionedScalar mu_;

        //- Voellmy friction parameter xi
        dimensionedScalar xi_;

public:

    //- Runtime type information
    TypeName("Voellmy");


    // Constructors

        //- Construct from components
        Voellmy
        (
            const dictionary& frictionProperties,
            const areaVectorField& Us,
            const areaScalarField& h,
            const areaScalarField& p
        );


    //- Destructor
    virtual ~Voellmy() = default;


    // Member Functions

        //- Return explicit coefficients for the linear system of equations
        //- considering friction
        virtual const areaVectorField& tauSc() const;

        //- Return implicit coefficients for the linear system of equations
        //- considering friction
        virtual const areaScalarField& tauSp() const;


        //- Read the friction properties dictionary
        virtual bool read(const dictionary& frictionProperties);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace frictionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | cfMesh: A library for mesh generation
   \\    /   O peration     |
    \\  /    A nd           | Author: Franjo Juretic (franjo.juretic@c-fields.com)
     \\/     M anipulation  | Copyright (C) Creative Fields, Ltd.
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
    Foam::Module::meshOctreeCubeBasic

Description
    A cube stores information needed for mesh generation

SourceFiles
    meshOctreeCubeBasic.C

\*---------------------------------------------------------------------------*/

#ifndef meshOctreeCubeBasic_H
#define meshOctreeCubeBasic_H

#include "meshOctreeCubeCoordinates.H"
#include "contiguous.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Module
{
class meshOctreeCubeBasic;
inline Ostream& operator<<(Ostream&, const meshOctreeCubeBasic&);
inline Istream& operator>>(Istream&, meshOctreeCubeBasic&);

/*---------------------------------------------------------------------------*\
                     Class meshOctreeCubeBasic Declaration
\*---------------------------------------------------------------------------*/

class meshOctreeCubeBasic
:
    public meshOctreeCubeCoordinates
{
    // Private data

        //- cube type
        direction cubeType_;

        //- processor to which the cube belongs to
        short procNo_;


public:

    // Constructors

        //- Null constructor
        inline meshOctreeCubeBasic();

        //- Construct from coordinates
        explicit inline meshOctreeCubeBasic
        (
            const meshOctreeCubeCoordinates&
        );

        //- Construct from coordinates and cube type
        inline meshOctreeCubeBasic
        (
            const meshOctreeCubeCoordinates& cc,
            const direction cubeType,
            const short procNo = ALLPROCS
        );


    //- Destructor
    ~meshOctreeCubeBasic() = default;


    // Enumerators

        enum typesOfCubes
        {
            UNKNOWN = 1,
            OUTSIDE = 2,
            DATA = 4,
            INSIDE = 8,
            ALLPROCS = -2,
            OTHERPROC = -3
        };


    // Member functions

        //- return type
        inline direction cubeType() const;

        //- set cube type
        inline void setCubeType(const direction);

        //- return processor number
        inline short procNo() const;

        //- set processor number
        inline void setProcNo(const short);

        //- return coordinates in the octree
        inline const meshOctreeCubeCoordinates& coordinates() const;


    // Member operators

        inline void operator=(const meshOctreeCubeBasic&);


    // Friend operators

        inline friend Ostream& operator<<
        (
            Ostream&,
            const meshOctreeCubeBasic&
        );

        inline friend Istream& operator>>
        (
            Istream&,
            meshOctreeCubeBasic&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Module

//- Data for meshOctreeCubeBasic are contiguous
template<>
inline bool contiguous<Module::meshOctreeCubeBasic>()
{
    return true;
}

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "meshOctreeCubeBasicI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

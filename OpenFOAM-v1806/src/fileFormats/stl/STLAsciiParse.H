/*--------------------------------*- C++ -*----------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2018 OpenCFD Ltd.
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
    Foam::Detail::STLAsciiParse

Description
    Internal class used when parsing STL ASCII format

SourceFiles
    STLAsciiParse.C

\*---------------------------------------------------------------------------*/

#ifndef STLAsciiParse_H
#define STLAsciiParse_H

#include "DynamicList.H"
#include "HashTable.H"
#include "STLpoint.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace Detail
{

/*---------------------------------------------------------------------------*\
                    Class Detail::STLAsciiParse Declaration
\*---------------------------------------------------------------------------*/

class STLAsciiParse
{
protected:

    // Protected Data

        bool  sorted_;
        label groupId_;      // The current solid group
        label lineNum_;

        //- The number of local points on the current facet
        int nFacetPoints_;

        //- Current vertex component when reading 'vertex'
        int nVertexCmpt_;

        //- Scratch space for reading 'vertex'
        STLpoint currVertex_;

        DynamicList<STLpoint> points_;
        DynamicList<label> facets_;
        DynamicList<word>  names_;
        DynamicList<label> sizes_;
        HashTable<label>   nameLookup_;


    // Protected Member Functions

        //- Action when entering 'solid'
        inline void beginSolid(word solidName);

        //- Action when entering 'facet'
        inline void beginFacet();

        //- Reset vertex component to zero
        inline void resetVertex();

        //- Add next vertex component. On each third call, adds the point.
        //  \return true when point has been added (on the last component)
        inline bool addVertexComponent(float val);

        //- Add next vertex component. On each third call, adds the point.
        //  \return true when point has been added (on the last component)
        inline bool addVertexComponent(const char* text);

        //- Action on 'endfacet'
        inline void endFacet();


        //- No copy construct
        STLAsciiParse(const STLAsciiParse&) = delete;

        //- No copy assignment
        void operator=(const STLAsciiParse&) = delete;


public:

    // Constructors

        //- From input stream and the approximate number of vertices in the STL
        inline STLAsciiParse(const label approxNpoints);


    // Member Functions

        //- Reset stored values
        inline void clear();

        //- Do all the solid groups appear in order?
        inline bool sorted() const;

        //- A list of unstitched triangle points
        inline DynamicList<STLpoint>& points();

        //- A list of facet IDs (group IDs)
        //- corresponds to the number of triangles
        inline DynamicList<label>& facets();

        //- Solid names in the order of their appearance.
        inline DynamicList<word>& names();

        //- Solid sizes in the order of their appearance.
        inline DynamicList<label>& sizes();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Detail
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "STLAsciiParseI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

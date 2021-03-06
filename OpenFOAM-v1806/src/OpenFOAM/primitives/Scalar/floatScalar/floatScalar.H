/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
     \\/     M anipulation  | Copyright (C) 2017 OpenCFD Ltd.
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

Typedef
    Foam::floatScalar

Description
    Floating-point single precision scalar type.

SourceFiles
    floatScalar.C

\*---------------------------------------------------------------------------*/

#ifndef floatScalar_H
#define floatScalar_H

#include "doubleFloat.H"
#include "direction.H"
#include "word.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef float floatScalar;

// Largest and smallest scalar values allowed in certain parts of the code.
// (6 is the number of significant figures in an
//  IEEE single precision number.  See limits.h or float.h)
constexpr floatScalar floatScalarGREAT = 1.0e+6;
constexpr floatScalar floatScalarVGREAT = 1.0e+37;
constexpr floatScalar floatScalarROOTVGREAT = 1.0e+18;
constexpr floatScalar floatScalarSMALL = 1.0e-6;
constexpr floatScalar floatScalarROOTSMALL = 1.0e-3;
constexpr floatScalar floatScalarVSMALL = 1.0e-37;
constexpr floatScalar floatScalarROOTVSMALL = 1.0e-18;


#define Scalar floatScalar
#define ScalarVGREAT floatScalarVGREAT
#define ScalarVSMALL floatScalarVSMALL
#define ScalarROOTVGREAT floatScalarROOTVGREAT
#define ScalarROOTVSMALL floatScalarROOTVSMALL
#define ScalarRead readFloat


inline Scalar mag(const Scalar s)
{
    return ::fabsf(s);
}

inline Scalar hypot(const Scalar x, const Scalar y)
{
    return ::hypotf(x, y);
}

inline Scalar atan2(const Scalar y, const Scalar x)
{
    return ::atan2f(y, x);
}

// Single-precision transcendental functions (with 'f' appended to the name)
#define transFunc(func)            \
inline Scalar func(const Scalar s) \
{                                  \
    return ::func##f(s);           \
}


#ifdef darwin
// Single-precision bessel functions. (No float version for darwin).
#define besselFunc(func)           \
inline Scalar func(const Scalar s) \
{                                  \
    return ::func(s);              \
}

inline Scalar jn(const int n, const Scalar s)
{
    return Scalar(::jn(n, double(s)));
}

inline Scalar yn(const int n, const Scalar s)
{
    return Scalar(::yn(n, double(s)));
}

#else

// Single-precision bessel functions (with 'f' appended to the name)
#define besselFunc(func)           \
inline Scalar func(const Scalar s) \
{                                  \
    return ::func##f(s);           \
}

inline Scalar jn(const int n, const Scalar s)
{
    return ::jnf(n, s);
}

inline Scalar yn(const int n, const Scalar s)
{
    return ::ynf(n, s);
}

#endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Scalar.H"

#undef Scalar
#undef ScalarVGREAT
#undef ScalarVSMALL
#undef ScalarROOTVGREAT
#undef ScalarROOTVSMALL
#undef ScalarRead
#undef transFunc
#undef besselFunc


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

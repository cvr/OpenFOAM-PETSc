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

\*---------------------------------------------------------------------------*/

#include "LPtrList.H"
#include "Istream.H"
#include "Ostream.H"
#include "INew.H"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class LListBase, class T>
template<class INew>
void Foam::LPtrList<LListBase, T>::read(Istream& is, const INew& inew)
{
    is.fatalCheck(FUNCTION_NAME);

    token firstToken(is);

    is.fatalCheck("LPtrList::readList : reading first token");

    if (firstToken.isLabel())
    {
        const label len = firstToken.labelToken();

        // Read beginning of contents
        const char delimiter = is.readBeginList("LPtrList");

        if (len)
        {
            if (delimiter == token::BEGIN_LIST)
            {
                for (label i=0; i<len; ++i)
                {
                    T* p = inew(is).ptr();
                    this->append(p);

                    is.fatalCheck("LPtrList::readList : reading entry");
                }
            }
            else
            {
                T* p = inew(is).ptr();
                this->append(p);

                is.fatalCheck("LPtrList::readList : reading entry");

                for (label i=1; i<len; ++i)
                {
                    this->append(p->clone().ptr());
                }
            }
        }

        // Read end of contents
        is.readEndList("LPtrList");
    }
    else if (firstToken.isPunctuation())
    {
        if (firstToken.pToken() != token::BEGIN_LIST)
        {
            FatalIOErrorInFunction
            (
                is
            )   << "incorrect first token, '(', found " << firstToken.info()
                << exit(FatalIOError);
        }

        token lastToken(is);
        is.fatalCheck(FUNCTION_NAME);

        while
        (
           !(
                lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST
            )
        )
        {
            is.putBack(lastToken);
            this->append(inew(is).ptr());

            is >> lastToken;
            is.fatalCheck(FUNCTION_NAME);
        }
    }
    else
    {
        FatalIOErrorInFunction
        (
            is
        )   << "incorrect first token, expected <int> or '(', found "
            << firstToken.info()
            << exit(FatalIOError);
    }

    is.fatalCheck(FUNCTION_NAME);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class LListBase, class T>
template<class INew>
Foam::LPtrList<LListBase, T>::LPtrList(Istream& is, const INew& inew)
{
    this->read(is, inew);
}


template<class LListBase, class T>
Foam::LPtrList<LListBase, T>::LPtrList(Istream& is)
{
    this->read(is, INew<T>());
}


// * * * * * * * * * * * * * * * Istream Operator  * * * * * * * * * * * * * //

template<class LListBase, class T>
Foam::Istream& Foam::operator>>(Istream& is, LPtrList<LListBase, T>& L)
{
    L.clear();
    L.read(is, INew<T>());

    return is;
}


// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class LListBase, class T>
Foam::Ostream& Foam::operator<<(Ostream& os, const LPtrList<LListBase, T>& lst)
{
    // Size and start delimiter
    os << nl << lst.size() << nl << token::BEGIN_LIST << nl;

    // Contents
    for (auto iter = lst.cbegin(); iter != lst.cend(); ++iter)
    {
        os << *iter << nl;
    }

    // End delimiter
    os << token::END_LIST;

    os.check(FUNCTION_NAME);
    return os;
}

// ************************************************************************* //

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
     \\/     M anipulation  | Copyright (C) 2016-2018 OpenCFD Ltd.
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
    Foam::argList

Description
    Extract command arguments and options from the supplied
    \a argc and \a argv parameters.

    Sequences with "(" ... ")" are transformed into a stringList.
    For example,
    \verbatim
        program -listFiles \( *.txt \)
    \endverbatim
    would create a stringList:
    \verbatim
        ( "file1.txt" "file2.txt" ... "fileN.txt" )
    \endverbatim
    The backslash-escaping is required to avoid interpretation by the shell.

    Default command-line options:
      - \par -case \<dir\>
        Select a case directory instead of the current working directory
      - \par -decomposeParDict \<file\>
        Read decomposePar dictionary from specified location
      - \par -parallel
        Specify case as a parallel job
      - \par -doc
        Display the documentation in browser
      - \par -srcDoc
        Display the source documentation in browser
      - \par -help
        Print the usage

    Additionally, the \b -noFunctionObjects and \b -postProcess options
    may be present for some solvers or utilities.

    Environment variables set by argList or by Time:
      - \par FOAM_CASE
        The path of the global case.
        It is the same for serial and parallel jobs.
      - \par FOAM_CASENAME
        The name of the global case.
      - \par FOAM_EXECUTABLE
        If not already present in the calling environment,
        it is set to the \a name portion of the calling executable.
      - \par FOAM_APPLICATION
        If not already present in the calling environment,
        it is set to the value of the \c application entry
        (from \c controlDict) if that entry is present.

    The value of the \b FOAM_APPLICATION may be inconsistent if the value of
    the \c application entry is adjusted during runtime.

Note
    - The document browser used is defined by the \b FOAM_DOC_BROWSER
      environment variable or the <tt>Documentation/docBrowser</tt> entry
      in the <tt>~OpenFOAM/controlDict</tt> file.
      The \%f token is used as a placeholder for the file name.
    - The valid (mandatory) arguments can be adjusted
      via the addArgument static method instead of directly
      manipulating the argList::validArgs static member.
    - The valid options can be adjusted
      via the addOption/removeOption static methods instead of directly
      manipulating the argList::validOptions static member.

SourceFiles
    argList.C
    argListI.H

\*---------------------------------------------------------------------------*/

#ifndef argList_H
#define argList_H

#include "stringList.H"
#include "SubList.H"
#include "SLList.H"
#include "HashTable.H"
#include "word.H"
#include "fileName.H"
#include "parRun.H"
#include "OSspecific.H"
#include "ITstream.H"
#include <utility>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class argList Declaration
\*---------------------------------------------------------------------------*/

class argList
{
    // Private data

        //- Track enabled/disabled mandatory arguments
        static bool argsMandatory_;

        //- Track enabled/disabled checking of processor directories state
        static bool checkProcessorDirectories_;

        //- Switch on/off parallel mode.
        //  Must be first to be constructed so destructor is done last.
        ParRunControl parRunControl_;

        //- The arguments after removing known options
        stringList args_;

        //- The extracted options
        HashTable<string> options_;

        word executable_;
        fileName rootPath_;
        bool distributed_;
        fileName globalCase_;
        fileName case_;
        string argListStr_;


    // Private Member Functions

        //- Helper for resolving aliases for -options within validOptionsCompat
        static word optionCompat(const word& optName);

        //- Helper for resolving ignored options
        static int optionIgnore(const word& optName);

        //- Helper function for printUsage
        static void printOptionUsage
        (
            const label location,
            const string& str
        );

        //- Read a List of values from ITstream,
        //- treating a single entry like a list of size 1.
        template<class T>
        static inline void readList(ITstream& is, List<T>& list);

        //- Get rootPath_ / globalCase_ from one of the following forms
        //   * [-case dir]
        //   * cwd
        //
        // Also export FOAM_CASE and FOAM_CASENAME environment variables
        // so they can be used immediately (eg, in decomposeParDict), as well
        // as the FOAM_EXECUTABLE environment.
        void getRootCase();

        //- Transcribe argv into internal args_.
        //  Transform sequences with "(" ... ")" into string lists
        //  return true if any "(" ... ")" sequences were captured
        bool regroupArgv(int& argc, char**& argv);


public:

    // Static data members

        //- A list of valid (mandatory) arguments
        static SLList<string> validArgs;

        //- A list of valid options
        static HashTable<string> validOptions;

        //- A list of valid parallel options
        static HashTable<string> validParOptions;

        //- A list of aliases for options.
        //  Stored as (alias = canonical, version)
        static HashTable<std::pair<word,int>> validOptionsCompat;

        //- A list of options to ignore.
        //  Stored as (option = bool, version)
        static HashTable<std::pair<bool,int>> ignoreOptionsCompat;

        //- Short usage information for validOptions
        static HashTable<string> optionUsage;

        //- Additional notes for usage
        static SLList<string> notes;

        //- Min offset for displaying usage (default: 20)
        static string::size_type usageMin;

        //- Max screen width for displaying usage (default: 80)
        static string::size_type usageMax;

        //- Standard name for the post-processing option
        static word postProcessOptionName;

        //! \cond internalClass
        class initValidTables
        {
        public:

            initValidTables();
        };
        //! \endcond


    // Constructors

        //- Construct from argc and argv
        //- checking the arguments and options as requested.
        argList
        (
            int& argc,
            char**& argv,
            bool checkArgs = true,
            bool checkOpts = true,
            bool initialise = true
        );

        //- Construct copy with new options
        argList
        (
            const argList& args,
            const HashTable<string>& options,
            bool checkArgs = true,
            bool checkOpts = true,
            bool initialise = true
        );


    //- Destructor
    virtual ~argList();


    // Member Functions

    // Low-level

        //- Scan for -help, -doc options etc prior to checking the validity
        //- of other args/opts and finally initialising.
        void parse
        (
            bool checkArgs,
            bool checkOpts,
            bool initialise
        );


    // Access

        //- Name of executable without the path
        inline const word& executable() const;

        //- Return root path
        inline const fileName& rootPath() const;

        //- Return distributed flag
        //- (i.e. are rootPaths different on different machines)
        inline bool distributed() const;

        //- Return case name (parallel run) or global case (serial run)
        inline const fileName& caseName() const;

        //- Return case name
        inline const fileName& globalCaseName() const;

        //- Return parRunControl
        inline const ParRunControl& parRunControl() const;

        //- Return the path to the caseName
        inline fileName path() const;

        //- Return the number of arguments
        inline label size() const;

        //- Return arguments
        inline const stringList& args() const;

        //- Non-const access to the command arguments (non-options)
        inline stringList& args();

        //- Return options
        inline const HashTable<string>& options() const;

        //- Return non-const access to the command options
        inline HashTable<string>& options();

        //- Read a value from the argument at index.
        //  Index 1 is the first non-option argument.
        template<class T>
        inline T read(const label index) const;

        //- Read a value from the argument at index.
        //  Index 1 is the first non-option argument.
        template<class T>
        inline List<T> readList(const label index) const;

        //- Return true if the named option is found
        inline bool found(const word& optName) const;

        //- Return how many of the specified options were used
        label count(const UList<word>& optionNames) const;

        //- Return how many of the specified options were used
        label count(std::initializer_list<word> optionNames) const;

        //- Return an input stream from the named option
        inline ITstream lookup(const word& optName) const;

        //- Read a value from the named option
        //  The default template parameter is string (ie, no conversion).
        template<class T=string>
        inline T opt(const word& optName) const;

        //- Read a value from the named option if present.
        //  \return true if the named option was found.
        template<class T>
        inline bool readIfPresent(const word& optName, T& val) const;

        //- Read a value from the named option if present.
        //  \return true if the named option was found, otherwise
        //  use the supplied default and return false.
        template<class T>
        inline bool readIfPresent
        (
            const word& optName,
            T& val,
            const T& deflt
        ) const;

        //- Read a value from the named option if present.
        //  Return supplied default otherwise.
        template<class T>
        inline T lookupOrDefault
        (
            const word& optName,
            const T& deflt
        ) const;

        //- Read a List of values from the named option,
        //- treating a single entry like a list of size 1.
        template<class T>
        inline List<T> readList(const word& optName) const;

        //- If named option is present, read a List of values
        //- treating a single entry like a list of size 1.
        //  \return true if the named option was found.
        template<class T>
        inline bool readListIfPresent(const word& optName, List<T>& list) const;


    // Older style access (including 1712 release)

        //- Read a value from the argument at index.
        //  Index 1 is the first non-option argument.
        //  \deprecated in favour of read() - JAN-2018
        template<class T>
        inline T argRead(const label index) const;

        //- Return true if the named option is found
        //  \deprecated in favour of found() - JAN-2018
        inline bool optionFound(const word& optName) const;

        //- Return an input stream from the named option
        //  \deprecated in favour of lookup() - JAN-2018
        inline ITstream optionLookup(const word& optName) const;

        //- Read a value from the named option
        //  \deprecated in favour of opt() - JAN-2018
        template<class T>
        inline T optionRead(const word& optName) const;

        //- Read a value from the named option if present.
        //  Return true if the named option was found.
        //  \deprecated in favour of readIfPresent() - JAN-2018
        template<class T>
        inline bool optionReadIfPresent
        (
            const word& optName,
            T& val
        ) const;

        //- Read a value from the named option if present.
        //  Return true if the named option was found, otherwise
        //  use the supplied default and return false.
        //  \deprecated in favour of readIfPresent() - JAN-2018
        template<class T>
        inline bool optionReadIfPresent
        (
            const word& optName,
            T& val,
            const T& deflt
        ) const;

        //- Read a value from the named option if present.
        //  Return supplied default otherwise.
        //  \deprecated in favour of lookupOrDefault() - JAN-2018
        template<class T>
        inline T optionLookupOrDefault
        (
            const word& optName,
            const T& deflt
        ) const;

        //- Read a List of values from the named option
        //  \deprecated in favour of readList() - JAN-2018
        template<class T>
        inline List<T> optionReadList(const word& optName) const;


    // Edit

        //- Append a (mandatory) argument to validArgs
        static void addArgument(const string& argName);

        //- Add a bool option to validOptions with usage information
        static void addBoolOption
        (
            const word& optName,
            const string& usage = ""
        );

        //- Add an option to validOptions with usage information
        //  An option with an empty param is a bool option
        static void addOption
        (
            const word& optName,
            const string& param = "",
            const string& usage = ""
        );

        //- Specify an alias for the option name.
        //
        //  \param optName the currently used option name
        //  \param compat alias name and the last OpenFOAM version (YYMM)
        //      when the alias was not needed.
        //      Setting a zero or negative version suppresses warnings about
        //      the alias.
        static void addOptionCompat
        (
            const word& optName,
            std::pair<const char*, int> compat
        );

        //- Specify an option to be ignored.
        //
        //  \param compat optName and the last OpenFOAM version (YYMM)
        //      when the option was directly supported.
        //      Setting a zero or negative version suppresses warnings about
        //      the alias.
        //  \param expectArg the option is non-bool
        static void ignoreOptionCompat
        (
            std::pair<const char*,int> compat,
            const bool expectArg
        );

        //- Add option usage information to optionUsage
        static void addUsage
        (
            const word& optName,
            const string& usage
        );

        //- Add extra notes for the usage information
        //  This string is used "as-is" without additional formatting
        static void addNote(const string& note);

        //- Remove option from validOptions and from optionUsage
        static void removeOption(const word& optName);

        //- Flag command arguments as being non-mandatory
        static void nonMandatoryArgs();

        //- Disable emitting the banner information.
        //  Adjusts the Foam::infoDetailLevel flag.
        static void noBanner();

        //- Banner status (enabled/disabled).
        //  Queries the Foam::infoDetailLevel flag.
        static bool bannerEnabled();

        //- Remove the 'noFunctionObjects' option,
        //- optionally adding a 'withFunctionObjects' option instead
        static void noFunctionObjects(bool addWithOption = false);

        //- Suppress JobInfo, overriding controlDict setting
        static void noJobInfo();

        //- Remove the parallel options
        static void noParallel();

        //- Remove checking of processor directories
        static void noCheckProcessorDirectories();

        //- Return true if the post-processing option is specified
        static bool postProcess(int argc, char *argv[]);

        //- Set option directly (use with caution)
        //  An option with an empty param is a bool option.
        //  Not all valid options can also be set: eg, -case, -roots, ...
        //  Return true if the existing option value needed changing,
        //  or if the option did not previously exist.
        bool setOption(const word& optName, const string& param = "");

        //- Unset option directly (use with caution)
        //  Not all valid options can also be unset: eg, -case, -roots ...
        //  Return true if the option existed before being unset.
        bool unsetOption(const word& optName);


    // Print

        //- Print option compatibility
        void printCompat() const;

        //- Print notes (if any)
        void printNotes() const;

        //- Print usage
        void printUsage(bool full=true) const;

        //- Display documentation in browser
        //  Optionally display the application source code
        void displayDoc(bool source=false) const;


    // Check

        //- Check argument list
        bool check(bool checkArgs=true, bool checkOpts=true) const;

        //- Check root path and case path
        bool checkRootCase() const;


    // Member Operators

        //- The string corresponding to the argument index.
        //  Index 0 is the executable.
        //  Index 1 is the first non-option argument.
        inline const string& operator[](const label index) const;

        //- The string associated with the named option
        inline const string& operator[](const word& optName) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "argListI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

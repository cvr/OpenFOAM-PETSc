<!--
   |--------------------------------------------------------------------------|
   | =========                 |                                              |
   | \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox        |
   |  \\    /   O peration     |                                              |
   |   \\  /    A nd           | Copyright (C) 2016-2018 OpenCFD Ltd.         |
   |    \\/     M anipulation  |                                              |
   |--------------------------------------------------------------------------|
  -->

---

# OpenFOAM&reg; ThirdParty Build

OpenFOAM depends to a certain extent on third-party libraries
(*opensource only*). It also provides some interfaces to *opensource* or
*proprietary* libraries. This third-party package contains configurations and
scripts for building third-party packages. It should normally only be used in
conjunction with the corresponding OpenFOAM version.

## Organization

The ThirdParty directory contains a number of build scripts as well as
some directories:

| Directory         | Contains
|-------------------|-------------------------------------------------------
| etc/              | auxiliary tools and content used for the build process
| build/            | intermediate build objects
| platforms/        | the installation directories


## Configuration of Third-Party Versions

For most of the build scripts, the default software version
is provided by an appropriate OpenFOAM `etc/config.sh/...` entry.
This approach avoids duplicate entries for the default versions and
ensures the best overall consistency between the OpenFOAM installation
and its corresponding third-party installation.

Nonethess, the distributed make scripts can generally be used for a
variety of versions of the third-party libraries, with the software
version specified on the command-line. For example,

    $ ./makeFFTW -help
    usage: makeFFTW [OPTION] [fftw-VERSION]

---

## Before Starting

0. Review the [system requirements](http://www.openfoam.com/documentation/system-requirements.php)
   and decide on the following:
   * compiler type/version - if the system compiler is not relatively recent,
     you will need a [third-party compiler](#makeGcc) installation.
   * MPI type/version.
   * ParaView type/version.
   * CMake type/version, ...
1. If you are using a system MPI (eg, openmpi), ensure that this environment
   has also been properly activated for your user.
   Often (but not always) a `mpi-selector` command is available for this purpose.
   You may need to open a new shell afterwards for the change to take effect.
   Using the following command may help diagnosing things:

       which mpicc

2. Adjust the OpenFOAM `etc/bashrc`, `etc/config.sh/...` or equivalent
   `prefs.sh` files to reflect your preferred configuration.
   For many config files, there are several configuration possibilities:
   - Define a particular third-party version.
   - Use a system installation.
   - Disable use of an optional component.
   - Define an alternative site-wide central location.
   - After making the desired changes, use `wmRefresh` or equivalent to use the configurations.


---

## Building

Many components of ThirdParty are *optional* or are invoked
automatically as part of the top-level OpenFOAM `Allwmake`.
Nonetheless it may be necessary or useful to build various
ThirdParty components prior to building OpenFOAM itself.

### Bootstrapping *(optional)*

* `makeGcc` _or_ `makeLLVM` <a name="makeGcc"></a>
   - Makes a third-party [gcc](#gcc-compiler) or [clang](#clang-compiler) installation,
     which is needed if the system gcc is [too old](#gcc-compiler).
     If your system compiler is recent enough, you can skip this step.
   - If you do use this option, you will need the following adjustments to the
     OpenFOAM `etc/bashrc` or your equivalent `prefs.sh` file:
     - `WM_COMPILER_TYPE=ThirdParty`
     - `WM_COMPILER=Gcc48` (for example)
     - `WM_COMPILER=Clang40` (for example)
     - or `WM_COMPILER=Clang` and adjust `clang_version` in the OpenFOAM
     `etc/config.sh/compiler` or equivalent.
   - More description is contained in the header comments of the
     `makeGcc` and `makeLLVM` files.
   - *Attention*: If you are building a newer version of clang, you may need to
     update your CMake beforehand.
* `makeCmake`
   - Makes a third-party [CMake](#general-packages) installation, which is
     needed if a system CMake does not exist or is [too old](#min-cmake),
   - Note that CMake is being used by an number of third-party packages
     (CGAL, LLVM, ParaView, VTK, ...)
     so this may become an increasingly important aspect of the build.

Note that the order of the bootstrapping process may need to be
reversed, or even require a few loops. For example, if you may need a
newer version of CMake before being able to build LLVM/Clang and
subsequently use the newly build clang to create a newer version of
CMake in the desired location.

Additionally, if you are using clang but with ThirdParty locations for
gmp/mpfr you will need some extra work. Here is an example:

* Compile a new ThirdParty clang version:

      ./makeLLVM llvm-4.0.1

* Now adjust the OpenFOAM `prefs.sh` to use the new compiler settings,
  and update the OpenFOAM environment (eg, `wmRefresh`)

* Next use (abuse) the `makeGcc` script to compile gmp/mpfr libraries.
  It is best to pass the desired versions explicitly, and necessary
  to set the CC/CXX variables so that the correct compiler is used:

      CC=clang CXX=clang++  ./makeGcc gmp-6.1.2 mpfr-4.0.0 gcc-system

  specifying `gcc-system` effectively disables building of gcc,
  but will build the gmp/mpfr components.

* As a final step, it will be necessary to add the ThirdParty
  gmp/mpfr locations in the OpenFOAM config files since they are
  normally only used in combination with a ThirdParty gcc.
  The location to make these changes is in the `etc/config.sh/CGAL`,
  since this is the component that uses the mpfr library.
  For example,

      gmp_version=gmp-6.1.2
      mpfr_version=mpfr-4.0.0

      export GMP_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$gmp_version
      export GMP_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER_ARCH/$mpfr_version

* Update update the OpenFOAM environment (eg, `wmRefresh`) again.


### Build Sequence

1. `Allwmake`
   - This will be automatically invoked by the top-level OpenFOAM `Allwmake`, but
     can also be invoked directly to find possible build errors.
   - Builds an mpi library (openmpi or mpich), scotch decomposition, boost, CGAL, FFTW.
   - If the optional kahip or metis  directories are found, they will also be compiled.
2. `makeParaView`  *(optional but highly recommended)*
   - This is optional, but extremely useful for visualization and for
     run-time post-processing function objects.
     You can build this at a later point in time, but then you should
     remember to rebuild the post-processing function objects and the
     reader module as well.
3. Make any additional optional components


#### Optional Components

`makeADIOS`
- Only required for [ADIOS](#parallel) support,
  which is currently staged in the [add-ons repository][link AddOns].

`makeCGAL`
- Builds [boost](#general-packages) and [CGAL](#general-packages).
  Automatically invoked from the ThirdParty `Allwmake`,
  but can be invoked directly to resolve possible build errors.

`makeFFTW`
- Builds [FFTW](#general-packages).
  Automatically invoked from the ThirdParty `Allwmake`,
  but can be invoked directly to resolve possible build errors.

`makeKAHIP`
- Builds [KaHIP](#parallel) decomposition library.
  Automatically invoked from the ThirdParty `Allwmake`,
  but can be invoked directly to resolve possible build errors.

`makeMETIS`
- Builds [METIS](#parallel) decomposition library.
  Automatically invoked from the ThirdParty `Allwmake`,
  but can be invoked directly to resolve possible build errors.

`makeMGridGen`
- Optional agglomeration routines.

`makeCCMIO`
- Only required for conversion to/from STARCD/STARCCM+ files.

`makeMesa`, `makeVTK`
- Additional support for building offscreen rendering components.
  Useful if you want to render on computer servers without graphics cards.
  The `makeParaView.example` and `makeVTK.example` files offer some
  suggestions about compiling such a configuration.

`makeQt`
- Script to build a [Qt](#makeQt), including qmake.
- Possibly needed for `makeParaView`.
- The associated `etc/relocateQt` may be of independent use.
  Read the file for more details.

`makeGperftools`
- Build gperftools (originally Google Performance Tools)

`minCmake`
- Scour specified directories for CMakeLists.txt and their cmake_minimum.
  Report in sorted order.

`Allclean`
- After building, this script may be used to remove intermediate build information
and save some disk space.


## Build Notes

### CGAL
- The zlib library and zlib development headers are required.

### Scotch
- The zlib library and zlib development headers are required.


### Mesa
- Needed for off-screen rendering.
- Building with [mesa-11][link mesa11] and [mesa-13][link mesa13] both
  seem okay, as does building with [mesa-17][link mesa].
- Building with mesa-12 is not possible since it fails to create
  the necessary `include/GL` directory and `osmesa.h` file.

### VTK
- Needed for off-screen rendering and run-time post-processing without
  ParaView.
- Rather than downloading VTK separately, it is easy to reuse the VTK
  sources that are bundled with ParaView.
  For example, by using a symbolic link:

      ln -s ParaView-v5.5.2/VTK VTK-9.0.0

  The appropriate VTK version number can be found from the contents of
  the `vtkVersion.cmake` file.
  For example,

      $ cat ParaView-v5.5.2/VTK/CMake/vtkVersion.cmake

      # VTK version number components.
      set(VTK_MAJOR_VERSION 9)
      set(VTK_MINOR_VERSION 0)
      set(VTK_BUILD_VERSION 0)

### ParaView
- Building ParaView requires CMake, qmake and a `qt` development files.
  Use the `-cmake`, `-qmake` and `-qt-*` options for `makeParaView` as
  required.
  See additional notes below about [making Qt](#makeQt) if necessary.

**NOTE** this step may not be entirely successful for your particular
system. Building ParaView itself is generally not a significant problem
but its dependency on particular Qt versions can be a problem.

If you fail at this step due to Qt dependencies, you may have success
building a slightly older Qt version. Another alternative may be to
use the ParaView-5.4 sources that were included in the ThirdParty-v1712
source pack. This, however, has never been tested in combination with
the OpenFOAM Catalyst insitu visualization.


#### 5.5.x binary package

For general functionality, the paraview version distributed with
the operating system or a [binary package][download ParaView]])
may be sufficient for your needs.
- No known issues with the native OpenFOAM reader.


Using a binary package does mean you miss these elements (which may or
may not be important for you):
- cannot visualize a `blockMeshDict`
- cannot build Catalyst insitu visualization
- no alternative OpenFOAM reader module for some special features not
  found in the native OpenFOAM reader.

#### 5.5.x
- Requires patching for the vtk-m configuration (on some systems).
- Recommended patching for ParaView Catalyst.
- Recommended patching for file series
  No known issues with the native OpenFOAM reader.

#### 5.4.x
- Compiles without patching.
  No known issues with the native OpenFOAM reader.

#### 5.3.0 and older are neither recommended nor supported
- Various compilation issues and known bugs.

### ADIOS
- The github release currently requires GNU autoconf tools (eg,
  autoconf, autoheader, automake) for its configuration.
- Some inconsistency in directory names (ADIOS vs. adios) between releases.
- Optionally uses bzip2, zlib development headers (eg, libbz2-devel, zlib-devel)
  for the corresponding compression tranforms.
- The [zfp floating point compression][page zfp] library is now included as
  part of ADIOS.

### Making Qt <a name="makeQt"></a>
- Building a third-party Qt installation (prior to building ParaView) requires
  some additional effort, but should nonetheless work smoothly.

1. Download a [*qt-everywhere-opensource-src*][link Qt5] package and
   unpack in the third-party directory.
2. Use the `makeQt` script with the QT version number. For example,

       ./makeQt 5.9.3

3. Build ParaView using this third-party QT. For example,

       ./makeParaView -qt-5.9.3  5.5.2

- ParaView versions prior to 5.3.0 do not properly support QT5.

- If you relocate the third-party directory to another location
  (eg, you built in your home directory, but want to install it in a
  central location), you will need to use the `etc/relocateQt` script
  afterwards.

*Note* On some older systems it can be quite difficult to build the
latest QT. In these cases, it is sometimes possible to build a
slightly older QT (eg, [qt-5.6.3][link Qt56]) instead.


---

## Versions

### Gcc Compiler <a name="gcc-compiler"></a>

The minimum version of gcc required is 4.8.0.

| Name              | Location
|-------------------|--------------------------------------------
| [gcc][page gcc]   | [releases][link gcc]
| [gmp][page gmp]   | system is often ok, otherwise [download][link gmp]
| [mpfr][page mpfr] | system is often ok, otherwise [download][link mpfr]
| [mpc][page mpc]   | system is often ok, otherwise [download][link mpc]


#### Potential MPFR conflicts

If you elect to use a third-party version of mpfr, you may experience
conflicts with your installed system mpfr.
On some systems, mpfr is compiled as *non-threaded*, whereas the
third-party will use *threaded* by default.
This can cause some confusion at the linker stage, since it may
resolve the system mpfr first (and find that it is *non-threaded*).

You can avoid this by one of two means:
1. Use system components for gmp/mpfr/mpc:  `makeGcc -system ...`
2. Use third-party mpfr, but without threading: `makeGcc -no-threadsafe ...`


#### 32-bit build (on 64-bit)

If you have a 64-bit system, but wish to have a 32-bit compiler, you
will need to enable multi-lib support for Gcc: `makeGcc -multilib`,
which is normally disabled, since many (most?) 64-bit systems do not
install the 32-bit development libraries by default.


### Clang Compiler <a name="clang-compiler"></a>

The minimum version of clang required is 3.5.

*Attention*: If you are building a newer version of clang, you may need to
update your CMake beforehand since GNU *configure* can only be used prior
to clang version 3.9.

If your system gcc is particularly old
(see [minimum gcc requirements for clang](#min-gcc))
you may have additional hurdles to using the newest versions of clang.


| Name                  | Location
|-----------------------|------------------------
| [clang][page clang]   | [download][link clang] or [newer][newer clang]
| [llvm][page llvm]     | [download][link llvm] or [newer][newer llvm]
| [openmp][page omp]    | [download][link omp] or [newer][newer omp]


### General <a name="general-packages"></a>

| Name                  | Location
|-----------------------|------------------------
| [CMake][page cmake]   | [download][link cmake]
| [boost][page boost]   | [download][link boost]
| [CGAL][page CGAL]     | [download][link CGAL]
| [FFTW][page FFTW]     | [download][link FFTW]
| [ADF/CGNS][page CGNS], ccm | [link ccmio][link ccmio]
| gperftools            | [repo][repo gperftools] or [download][link gperftools]


### Parallel Processing <a name="parallel"></a>

| Name                  | Location
|-----------------------|------------------------
| [openmpi][page openmpi] | [download][link openmpi]. The newer [openmpi][newer openmpi] make exhibit stability issues.
| [adios][page adios]   | [repo][repo adios] or [github download][link adios] or [alt download][altlink adios]
| [scotch, ptscotch][page scotch] | [download][link scotch]
| [kahip][page kahip] | [download][link kahip]
| [metis][page metis] | [download][link metis]


### Visualization <a name="viz-version"></a>

| Name                  | Location
|-----------------------|------------------------
| [MESA][page mesa]     | [download][link mesa] or [older 13][link mesa13], [older 11][link mesa11]
| [ParaView][page ParaView] | [download][link ParaView] or [older paraview-54][link ParaView54] or [binaries][download ParaView]
| [Qt][page Qt]         | [QT5][link Qt5] for ParaView-5.3.0 and later, or the [older qt-56][link Qt56] for older systems.


### CMake Minimum Requirements <a name="min-cmake"></a>

The minimum CMake requirements for building various components.

    2.8         llvm-3.4.2
    2.8.11      CGAL-4.9
    2.8.11      CGAL-4.11
    2.8.12.2    llvm-3.7.0
    2.8.12.2    llvm-3.8.0
    2.8.4       cmake-3.6.0
    3.3         ParaView-5.5.2
    3.4.3       llvm-3.9.1
    3.4.3       llvm-4.0.0 - llvm-6.0.0
    3.6         ADIOS2


### GCC Minimum Requirements <a name="min-gcc"></a>

The minimum gcc/g++ requirements for building various components.

    4.7         llvm-3.7.0
    4.7         llvm-3.6.2
    4.7         llvm-3.5.2
    4.4         llvm-3.4.2

If your system gcc/g++ is too old to build the desired llvm/clang
version, you may need to build a lower llvm/clang version and then use
that clang compiler for building the newer llvm/clang version.


<!-- gcc-related -->
[page gcc]:       http://gcc.gnu.org/releases.html
[page gmp]:       http://gmplib.org/
[page mpfr]:      http://www.mpfr.org/
[page mpc]:       http://www.multiprecision.org/

[link gcc]:       http://gcc.gnu.org/releases.html
[link gmp]:       ftp://ftp.gnu.org/gnu/gmp/gmp-6.1.0.tar.bz2
[link mpfr]:      ftp://ftp.gnu.org/gnu/mpfr/mpfr-3.1.4.tar.bz2
[link mpc]:       ftp://ftp.gnu.org/gnu/mpc/mpc-1.0.3.tar.gz


<!-- clang-related -->
[page llvm]:      http://llvm.org/
[page clang]:     http://clang.llvm.org/
[page omp]:       http://openmp.llvm.org/

[link clang]:     http://llvm.org/releases/3.7.1/cfe-3.7.1.src.tar.xz
[link llvm]:      http://llvm.org/releases/3.7.1/llvm-3.7.1.src.tar.xz
[link omp]:       http://llvm.org/releases/3.7.1/openmp-3.7.1.src.tar.xz

[newer clang]:    http://llvm.org/releases/4.0.1/cfe-4.0.1.src.tar.xz
[newer llvm]:     http://llvm.org/releases/4.0.1/llvm-4.0.1.src.tar.xz
[newer omp]:      http://llvm.org/releases/4.0.1/openmp-4.0.1.src.tar.xz


<!-- parallel -->
[page adios]:     https://www.olcf.ornl.gov/center-projects/adios/
[repo adios]:     https://github.com/ornladios/ADIOS
[link adios]:     https://github.com/ornladios/ADIOS/archive/v1.13.0.tar.gz
[altlink adios]:  http://users.nccs.gov/%7Epnorbert/adios-1.13.0.tar.gz
[page zfp]:       http://computation.llnl.gov/projects/floating-point-compression/zfp-versions

[page scotch]:    https://www.labri.fr/perso/pelegrin/scotch/
[link scotch]:    https://gforge.inria.fr/frs/download.php/file/34099/scotch_6.0.3.tar.gz
[link scotch]:    https://gforge.inria.fr/frs/download.php/file/37398/scotch_6.0.5a.tar.gz

[page kahip]:     http://algo2.iti.kit.edu/documents/kahip/
[link kahip]:     http://algo2.iti.kit.edu/schulz/software_releases/KaHIP_2.00.tar.gz

[page metis]:     http://glaros.dtc.umn.edu/gkhome/metis/metis/overview
[link metis]:     http://glaros.dtc.umn.edu/gkhome/fetch/sw/metis/metis-5.1.0.tar.gz

[page openmpi]:   http://www.open-mpi.org/
[link openmpi]:   https://www.open-mpi.org/software/ompi/v1.10/downloads/openmpi-1.10.4.tar.bz2
[newer openmpi]:  https://www.open-mpi.org/software/ompi/v2.1/downloads/openmpi-2.1.1.tar.bz2


<!-- general -->
[page cmake]:     http://www.cmake.org/
[link cmake]:     http://www.cmake.org/files/v3.5/cmake-3.5.2.tar.gz

[page boost]:     http://boost.org
[link boost]:     https://sourceforge.net/projects/boost/files/boost/1.64.0/boost_1_64_0.tar.bz2

[page CGAL]:      http://cgal.org
[link CGAL]:      https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-4.9.1/CGAL-4.9.1.tar.xz

[page FFTW]:      http://www.fftw.org/
[link FFTW]:      http://www.fftw.org/fftw-3.3.7.tar.gz

[page cgns]:      http://cgns.github.io/
[link ccmio]:     http://portal.nersc.gov/project/visit/third_party/libccmio-2.6.1.tar.gz (check usage conditions)
[altlink ccmio]:  http://portal.nersc.gov/svn/visit/trunk/third_party/libccmio-2.6.1.tar.gz (check usage conditions)

[repo gperftools]: https://github.com/gperftools/gperftools
[link gperftools]: https://github.com/gperftools/gperftools/releases/download/gperftools-2.5/gperftools-2.5.tar.gz


<!-- Visualization -->

[page ParaView]:  http://www.paraview.org/
[download ParaView]: https://www.paraview.org/download/
[link ParaView54]: http://www.paraview.org/files/v5.4/ParaView-v5.4.1.tar.gz
[link ParaView]:   http://www.paraview.org/files/v5.5/ParaView-v5.5.2.tar.gz

[page mesa]:  http://mesa3d.org/
[link mesa]:  ftp://ftp.freedesktop.org/pub/mesa/mesa-17.1.1.tar.xz
[link mesa13]: ftp://ftp.freedesktop.org/pub/mesa/13.0.6/mesa-13.0.6.tar.xz
[link mesa11]: ftp://ftp.freedesktop.org/pub/mesa/older-versions/11.x/11.2.2/mesa-11.2.2.tar.xz

[page Qt]: https://www.qt.io/download-open-source/
[repo Qt]: http://code.qt.io/cgit/qt-creator/qt-creator.git
[link Qt56]: http://download.qt.io/official_releases/qt/5.6/5.6.3/single/qt-everywhere-opensource-src-5.6.3.tar.xz
[link Qt5]: http://download.qt.io/official_releases/qt/5.9/5.9.3/single/qt-everywhere-opensource-src-5.9.3.tar.xz

<!-- OpenFOAM -->

[link AddOns]: https://develop.openfoam.com/Community/OpenFOAM-addOns
[link community-projects]: http://www.openfoam.com/community/projects.php

---

<!-- Standard Footer -->
## Additional OpenFOAM Links

- [Community AddOns][link AddOns] repository
- [Collaborative and Community-based Developments][link community-projects]
- [Download](http://www.openfoam.com/download) and
  [installation instructions](http://www.openfoam.com/code/build-guide.php)
- [Documentation](http://www.openfoam.com/documentation)
- [Reporting bugs/issues](http://www.openfoam.com/code/bug-reporting.php) (including bugs/suggestions/feature requests) in OpenFOAM
- [Contacting OpenCFD](http://www.openfoam.com/contact)

---

Copyright 2016-2018 OpenCFD Ltd

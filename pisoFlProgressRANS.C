/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2008 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    pierceFlamelet

Description
    Compressible flamelet LES solver.

\*---------------------------------------------------------------------------*/

// Chemtable
#include "IFstream.H"
#include "word.H"
#include "scalarList.H"
#include "dimensionedConstants.H"

#include "fvCFD.H"
#include "RASModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
  #include "setRootCase.H"

  #include "createTime.H"
  #include "createMesh.H"
  #include "createFields.H"

  #include "readChemTable.H" // read chemtable properties

  //# include "maxTemp.H"

  #include "readPISOControls.H"
  #include "initContinuityErrs.H"
  #include "readTimeControls.H"
  #include "compressibleCourantNo.H"
  #include "setInitialDeltaT.H"

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

  Info<< "\nStarting time loop\n" << endl;

  // lduMatrix::debug = 0;

  while (runTime.run())
  {
    #include "readTimeControls.H"
    #include "readPISOControls.H"
    #include "compressibleCourantNo.H"
    #include "setDeltaT.H"

    runTime++;

    #include "updateProperties.H" // update alpha, wc
    #include "updateRho.H"

    Info<< "Time = " << runTime.timeName() << nl << endl;

      // --- PISO loop
      for (int corr=1; corr<=nCorr; corr++)
      {
        #include "rhoEqn.H"

        turbulence->correct();

        #include "UEqn.H"

        #include "zEqn.H"
        #include "cEqn.H"
        #include "zVarEqn.H"
        #include "boundValues.H"
        #include "updateProperties.H" // update alpha, wc
        #include "updateRho.H"

        psi.oldTime();
        // psi = W/(R*T);
        psi = rho / pref;

        p.storePrevIter();
        rho.storePrevIter();

        #include "pEqn.H"
      }

    rho = p * psi;

    Info << "Zmean max: " << max(Zmean).value()
	       << "\tCmean max: " << max(Cmean).value()
	       << "\tZvar max: " << max(Zvar).value()
	       << "\tU max: " << mag(max(U).value())
         << "\tp max: " << max(p).value()
      	 << "\trho max: " << max(rho).value()
        << endl;

    Info << "Zmean min: " << min(Zmean).value()
	       << "\tCmean min: " << min(Cmean).value()
	       << "\tZvar min: " << min(Zvar).value()
         << "\tU min: " << mag(min(U).value())
         << "\tp min: " << min(p).value()
         << "\trho min: " << min(rho).value()
         << endl;

    runTime.write();

    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
	       << "  ClockTime = " << runTime.elapsedClockTime() << " s"
	       << nl << endl;
    }

  Info<< "End\n" << endl;

  return(0);
}

// ************************************************************************* //

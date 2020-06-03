rho = p*psi;

volScalarField rUA = 1.0/UEqn.A();
U = rUA*UEqn.H();

phi = fvc::interpolate(rho)*
  (
   (fvc::interpolate(U) & mesh.Sf())
   + fvc::ddtPhiCorr(rUA, rho, U, phi)
   );

for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++)
  {
    fvScalarMatrix pEqn
      (
       fvm::ddt(psi,p)
       + fvc::div(phi)
       - fvm::laplacian(rho*rUA, p)
       );

    pEqn.solve();

    if (nonOrth == nNonOrthCorr)
      {
	phi += pEqn.flux();
      }

  }

#include "rhoEqn.H"

p.relax();

rho = p*psi;
rho.relax();

U -= rUA*fvc::grad(p);
U.correctBoundaryConditions();

bound(p, pMin);

fvScalarMatrix zEqn
(
 fvm::ddt(rho, Zmean)
 + fvm::div(phi, Zmean)
 - fvm::laplacian(turbulence->alphaEff(), Zmean)
 );

zEqn.solve();

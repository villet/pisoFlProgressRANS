fvScalarMatrix cEqn
(
 fvm::ddt(rho, Cmean)
 + fvm::div(phi, Cmean)
 - fvm::laplacian(turbulence->alphaEff(), Cmean)
 ==
 rho * wc
 );

cEqn.solve();

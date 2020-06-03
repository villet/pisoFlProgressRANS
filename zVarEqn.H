// Calculate mixture fraction variance                                                                    
fvScalarMatrix ZvarEqn
(
 fvm::ddt(rho, Zvar)
 + fvm::div(phi, Zvar)
 - fvm::laplacian(turbulence->alphaEff(), Zvar)
     ==
 2.86*turbulence->mut()*magSqr(fvc::grad(Zmean))
 - fvm::Sp(2.0*rho*turbulence->epsilon()/turbulence->k(), Zvar)
 );

// cx -> oletetaan 2.0

ZvarEqn.solve();

// Gravity neglected at this point
// can be added on the right hand side of
// momentum eq. == rho*g

// Solve the Momentum equation
fvVectorMatrix UEqn
(
   fvm::ddt(rho, U)
 + fvm::div(phi, U)
 + turbulence->divDevRhoReff(U)
 );

UEqn.relax();

if (momentumPredictor)
{
  solve(UEqn == -fvc::grad(p));
}

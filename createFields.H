Info << "Reading pressure field p" << endl;

volScalarField p
(
 IOobject
 (
  "p",
  runTime.timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );

dimensionedScalar pref = dimensionedScalar("pref", dimensionSet(1,-1,-2,0,0,0,0), 1e5);

Info<< "Reading field U" << endl;

volVectorField U
(
 IOobject
 (
  "U",
  runTime.timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );

Info << "Reading mean mixture fraction field Zmean" << endl;

volScalarField Zmean
(
 IOobject
 (
  "Zmean",
  runTime.timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );

Info << "Reading mixture fraction variance field Zvar" << endl;

volScalarField Zvar
(
 IOobject
 (
  "Zvar",
  runTime.timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );


Info << "Reading mean progress variable field Cmean" << endl;

volScalarField Cmean
(
 IOobject
 (
  "Cmean",
  runTime.timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );

Info << "Creating density field rho" << endl;

volScalarField rho
(
 IOobject
 (
  "rho",
  runTime.timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );
//rho.oldTime();

Info << "Creating temperature field T" << endl;

volScalarField T
(
 IOobject
 (
  "T",
  runTime.timeName(),
  mesh,
  IOobject::READ_IF_PRESENT,
  IOobject::AUTO_WRITE
  ),
 mesh,
 dimensionedScalar("T", dimTemperature, 293.0)
 );

volScalarField psi
(
 IOobject
 (
  "psi",
  runTime.timeName(),
  mesh,
  IOobject::NO_READ,
  IOobject::NO_WRITE
  ),
 rho/pref
 );

Info << "Creating diffusivity field alpha" << endl;

volScalarField alpha
(
 IOobject
 (
  "alpha",
  mesh.time().timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );

Info << "Creating viscosity field mu" << endl;

volScalarField mu
(
 IOobject
 (
  "mu",
  mesh.time().timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );

Info << "Creating progress variable source term wc" << endl;

volScalarField wc
(
 IOobject
 (
  "wc",
  mesh.time().timeName(),
  mesh,
  IOobject::MUST_READ,
  IOobject::AUTO_WRITE
  ),
 mesh
 );

Info << "Creating molar mass W" << endl;

#include "compressibleCreatePhi.H"

Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::RASModel> turbulence
(
 compressible::RASModel::New(rho, U, phi, alpha, mu)
);

dimensionedScalar pMin
(
 mesh.solutionDict().subDict("PISO").lookup("pMin")
 );

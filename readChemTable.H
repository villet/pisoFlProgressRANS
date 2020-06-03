IFstream chemTableFile("chemtable");

label nZ;
label nzVar;
label nProg;
label nVars;

chemTableFile >> nZ;
chemTableFile >> nzVar;
chemTableFile >> nProg;
chemTableFile >> nVars;

Info<< "Chemtable has " << nZ << " mean mixture fraction points" << endl;
Info<< "Chemtable has " << nzVar << " mixture fraction variance points" << endl;
Info<< "Chemtable has " << nProg << " mean progress variable points" << endl;
Info<< "Chemtable has " << nVars << " tabulated variables" << endl;

scalarList nzPoints;
nzPoints.setSize(nZ);

for (label i = 0; i <= nZ - 1; i++)
  {
    chemTableFile >> nzPoints[i];
  }

scalarList nZvarPoints;
nZvarPoints.setSize(nzVar);

for (label i = 0; i <= nzVar - 1; i++)
  {
    chemTableFile >> nZvarPoints[i];
  }

scalarList nProgPoints;
nProgPoints.setSize(nProg);

for (label i = 0; i <= nProg - 1; i++)
  {
    chemTableFile >> nProgPoints[i];
  }

wordList varName;
varName.setSize(nVars);

scalarList testi;
testi.setSize(nVars*nZ*nzVar*nProg);

Info << "Reading variable";

for (label l = 0; l <= nVars - 1; l++)
  {
    chemTableFile >> varName[l];
    Info << " " << varName[l];

    for (label i = 0; i <= nProg - 1; i++)
      {
	for (label j = 0; j <= nzVar - 1; j++)
	  {
	    for (label k = 0; k <= nZ - 1; k++)
	      {
		chemTableFile >> testi[l*nZ*nzVar*nProg + k + j*nZ + i*nZ*nzVar];
	      }
	  }
      }

  }

Info << endl;

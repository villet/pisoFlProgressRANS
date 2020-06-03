scalar temp=0;
label kds=0;
forAll(testi, i)
{
if (testi[i]> temp)
{
  temp=testi[i];
  kds = i;
}
}

Info << kds << " kds" << endl;

for (label l = 0; l <= nVars - 1; l++)
  {
    for (label i = 0; i <= nProg - 1; i++)
      {
        for (label j = 0; j <= nzVar - 1; j++)
          {
            for (label k = 0; k <= nZ - 1; k++)
              {
                if((l*nZ*nzVar*nProg + k + j*nZ + i*nZ*nzVar) == kds)
                   {
                     Info << l << " " << k << " " << j << " " << i << endl;
                     Info << l << " " << nzPoints[k] << " " << nZvarPoints[j] << " " << nProgPoints[i] << endl;
                   }
              }
          }
      }
  }

forAll(mesh.cells(), celli)
{
  label x1 = 0;
  label x2 = 0;
  scalar w1 = 1.0;
  
  label y1 = 0;
  label y2 = 0;
  label w2 = 1.0;

  label z1 = 0;
  label z2 = 0;
  scalar w3 = 1.0;
  
  for (label i = 0; i <= nZ - 1; i++)
    {
      
      if (Zmean[celli] <= nzPoints[i] || (i == nZ - 1) )
	{
          x2 = i;
	  x1 = i - 1;
	  
	  if (x2 > 0)
	    {
	      if (x2 == nZ -1 && Zmean[celli] >= nzPoints[nZ - 1])
		{
                  w1 = 1.0;
		}
	      else
		{
                  w1 = (Zmean[celli] - nzPoints[x1]) / (nzPoints[x2] - nzPoints[x1]);
                }
	    }
	  
	  else
	    {
	      x2 = 1;
	      x1 = 0;
	      w1 = 0.0;
	    }
	  
          break;
	} 
    }
  
  for (label i = 0; i <= nzVar - 1; i++)
    {
      if (Zvar[celli] <= nZvarPoints[i] || (i == nzVar - 1) )
        {
          y2 = i;
          y1 = i - 1;

          if (y2 > 0)
            {
              if (y2 == nzVar -1 && Zvar[celli] >= nZvarPoints[nzVar - 1])
                {
                  w2 = 1.0;
                }
              else
                {
                  w2 = (Zvar[celli] - nZvarPoints[y1]) / (nZvarPoints[y2] - nZvarPoints[y1]);
                }
            }

          else
            {
              y2 = 1;
              y1 = 0;
              w2 = 0.0;
            }

          break;
        }

    }
  
  for (label i = 0; i <= nProg - 1; i++)
    {

      if (Cmean[celli] <= nProgPoints[i] || (i == nProg - 1) )
        {
          z2 = i;
          z1 = i - 1;

          if (z2 > 0)
            {
              if (z2 == nProg - 1 && Cmean[celli] >= nProgPoints[nProg - 1])
                {
                  w3 = 1.0;
                }
              else
                {
                  w3 = (Cmean[celli] - nProgPoints[z1]) / (nProgPoints[z2] - nProgPoints[z1]);
                }
            }

          else
            {
              z2 = 1;
              z1 = 0;
              w3 = 0.0;
            }

          break;
        }

    }
  
      T[celli] = w3*( w2*
		      (w1*testi[nZ*nzVar*nProg + x2 + y2*nZ + z2*nZ*nzVar]
			  + (1-w1)*testi[nZ*nzVar*nProg + x1 + y2*nZ + z2*nZ*nzVar])
		      + (1-w2) * 
		      (w1*testi[nZ*nzVar*nProg + x2 + y1*nZ + z2*nZ*nzVar] +
		       (1-w1)*testi[nZ*nzVar*nProg + x1 + y1*nZ + z2*nZ*nzVar]))
	+ (1-w3)*( w2*
		   (w1*testi[nZ*nzVar*nProg + x2  + y2*nZ + z1*nZ*nzVar] +
		    (1-w1)*testi[nZ*nzVar*nProg + x1 + y2*nZ + z1*nZ*nzVar])
		   + (1-w2)*
		   (w1*testi[nZ*nzVar*nProg + x2  + y1*nZ + z1*nZ*nzVar] +
		    (1-w1)*testi[nZ*nzVar*nProg + x1 + y1*nZ + z1*nZ*nzVar]));

      mu[celli] = w3*( w2*
		       (w1*testi[2*nZ*nzVar*nProg + x2 + y2*nZ + z2*nZ*nzVar]
			+ (1-w1)*testi[2*nZ*nzVar*nProg + x1 + y2*nZ + z2*nZ*nzVar])
		       + (1-w2) *
		       (w1*testi[2*nZ*nzVar*nProg + x2 + y1*nZ + z2*nZ*nzVar] +
			(1-w1)*testi[2*nZ*nzVar*nProg + x1 + y1*nZ + z2*nZ*nzVar]))
	+ (1-w3)*( w2*
                   (w1*testi[2*nZ*nzVar*nProg + x2  + y2*nZ + z1*nZ*nzVar] +
                    (1-w1)*testi[2*nZ*nzVar*nProg + x1 + y2*nZ + z1*nZ*nzVar])
                   + (1-w2)*
                   (w1*testi[2*nZ*nzVar*nProg + x2  + y1*nZ + z1*nZ*nzVar] +
                    (1-w1)*testi[2*nZ*nzVar*nProg + x1 + y1*nZ + z1*nZ*nzVar]));
      
      alpha[celli] = w3*( w2*
			  (w1*testi[3*nZ*nzVar*nProg + x2 + y2*nZ + z2*nZ*nzVar]
			   + (1-w1)*testi[3*nZ*nzVar*nProg + x1 + y2*nZ + z2*nZ*nzVar])
			  + (1-w2) *
			  (w1*testi[3*nZ*nzVar*nProg + x2 + y1*nZ + z2*nZ*nzVar] +
			   (1-w1)*testi[3*nZ*nzVar*nProg + x1 + y1*nZ + z2*nZ*nzVar]))
        + (1-w3)*( w2*
                   (w1*testi[3*nZ*nzVar*nProg + x2  + y2*nZ + z1*nZ*nzVar] +
                    (1-w1)*testi[3*nZ*nzVar*nProg + x1 + y2*nZ + z1*nZ*nzVar])
                   + (1-w2)*
                   (w1*testi[3*nZ*nzVar*nProg + x2  + y1*nZ + z1*nZ*nzVar] +
                    (1-w1)*testi[3*nZ*nzVar*nProg + x1 + y1*nZ + z1*nZ*nzVar]));

      wc[celli] = w3*( w2*
                          (w1*testi[4*nZ*nzVar*nProg + x2 + y2*nZ + z2*nZ*nzVar]
			   + (1-w1)*testi[4*nZ*nzVar*nProg + x1 + y2*nZ + z2*nZ*nzVar])
                          + (1-w2) *
                          (w1*testi[4*nZ*nzVar*nProg + x2 + y1*nZ + z2*nZ*nzVar] +
                           (1-w1)*testi[4*nZ*nzVar*nProg + x1 + y1*nZ + z2*nZ*nzVar]))
        + (1-w3)*( w2*
                   (w1*testi[4*nZ*nzVar*nProg + x2  + y2*nZ + z1*nZ*nzVar] +
                    (1-w1)*testi[4*nZ*nzVar*nProg + x1 + y2*nZ + z1*nZ*nzVar])
                   + (1-w2)*
                   (w1*testi[4*nZ*nzVar*nProg + x2  + y1*nZ + z1*nZ*nzVar] +
                    (1-w1)*testi[4*nZ*nzVar*nProg + x1 + y1*nZ + z1*nZ*nzVar]));

}

T.correctBoundaryConditions();
mu.correctBoundaryConditions();
alpha.correctBoundaryConditions();
wc.correctBoundaryConditions();

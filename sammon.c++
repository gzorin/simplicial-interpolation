/************************************************************************

This library implements simplicial interpolation as described in
"Interpolated Mappings for Musical Instruments", Organised Sound 7(2),
Cambridge University Press.

Copyright 2002 Camille Goudeseune.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation;  either version 2.1
of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Contact:
  Camille Goudeseune
  Integrated Systems Laboratory
  Beckman Institute for Advanced Science and Technology
  405 N Mathews
  Urbana IL 61801 USA
  camilleg@isl.uiuc.edu

************************************************************************/

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sammon.h"
#include "util.h"

vertex* computeSammon(const e_vertex* pi, const int cpt, const double scalar)
{
  double* rgzTarget = new double[triangularNumber(cpt - 1)];
  double* rgzBest = new double[cpt*d];
  double* rgz = new double[cpt*d];

  // Modification of the published algorithm:  store the squares of
  // the distances, instead of the distances themselves.
  // Avoids computing a zillion square roots.

  // Compute target values for distance matrix.
  int i, j, k=0;
  for (i=0; i<cpt-1; ++i)
  for (j=i+1; j<cpt; ++j, ++k)
    {
    rgzTarget[k] = 0.;
    for (int idim=0; idim<e; ++idim)
      rgzTarget[k] += sq(pi[i][idim] - pi[j][idim]);
    }

  // Run Sammon's Mapping crun times, and keep the best run so far.
  const int crun = 1000;
  int runLastGood = -1;
  double errorMin = DBL_MAX;
  for (int run=0; run<crun; ++run)
    {
    if (run - runLastGood > crun/4)
      // Abort... haven't improved in quite a while.
      break;

    // Build initial configuration of points.
    for (i=0; i<cpt*d; ++i)
      rgz[i] = drand48() * scalar;
    
    // Now rgz[i*d + 0] to rgz[i*d + d-1] are the coords of
    // the i'th point of the configuration.

    // Iterate.
    const int iterMax = 75 * cpt;
    for (int iter=0; iter<iterMax; iter++)
      {
      // Pick a pair of points uniformly randomly (fast and simple).
      do {
	i = (rand() >> 4) % cpt;
	j = (rand() >> 4) % cpt;
	}
      while (i == j);
      if (i>j)
	{ int t=i; i=j; j=t; }
      // Now 0 <= i < j < cpt.

      // Find the distance between them, target and current.
      const double distTarget = rgzTarget[TriIJ(i, j, cpt)];

      double* aa = &rgz[i*d];
      double* bb = &rgz[j*d];
      const double* a = aa;
      const double* b = bb;
      double distCur = 0.;
      for (k=0; k<d; ++k)
	distCur += sq(b[k] - a[k]);

      // Adjust their relative positions.
      const double temperature = 1. - (double)iter/iterMax;     // From 1 to 0.
      const double gamma = .8 * temperature * temperature;      // Also down to 0.
      const double magnitude = gamma * (distTarget - distCur) / distCur;
      for (k=0; k<d; ++k)
	{
	const double c = magnitude * (a[k]-b[k]);
	aa[k] += c;
	bb[k] -= c;
	}
      
      // Now rgz's pairwise distances approximate rgzTarget's.
      // See how good the approximation really is.
      // (Later optimization:  update error only incrementally, since only two points changed.)

      double error = 0.;
      k = 0;
      for (i=0; i<cpt-1; ++i)
      for (j=i+1; j<cpt; ++j, ++k)
	{
	const double* a = &rgz[i*d];
	const double* b = &rgz[j*d];
	double sum = 0.;
	for (int _=0; _<d; ++_)
	  sum += sq(b[_] - a[_]);
	error += fabs(rgzTarget[k] - sum);
	}
      if (error < errorMin)
	{
	// Best approximation so far.  Keep this one.
	// (Later optimization:  reduce gamma and hang around here for a while,
	// lest we wander away from the local minimum near here.)
	errorMin = error;
	runLastGood = run;
	memcpy(rgzBest, rgz, cpt*d*sizeof(double));
	//if (run > 0)
	//  printf("\t%5d/%.3f: %.3f\n", run, float(iter)/iterMax, sqrt(error));
	}
      }
    }
  delete [] rgz;
  delete [] rgzTarget;
  // printf("Sammon's mapping: best error is %.3f\n", sqrt(errorMin));
  vertex* result = new vertex[cpt];
  for (i=0; i<cpt; ++i)
  for (k=0; k<d; ++k)
    result[i][k] = rgzBest[i*d + k];
  delete [] rgzBest;
  return result;
}

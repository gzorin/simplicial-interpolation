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

void* GA(					// run the GA.  returns member.
        int cbMemberArg,			// size of population
        void (*pfnGenerateRandom)(void* pv),	// randomly generate a member
        void (*pfnMutateRandom)(void* pv, long cIter),	// mutate a member
        void (*pfnTweak)(void* pv),			// tweak a member
        double (*pfnComputeSuitability)(void* pv),	// fitness function
        double zSuitabilityMax,				// perfect fit
	int cBestArg,				// # of members to keep per gen.
	double tMaxSec				// timeout
        );

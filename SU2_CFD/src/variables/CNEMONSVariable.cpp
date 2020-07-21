﻿/*!
 * \file CNEMONSVariable.cpp
 * \brief Definition of the solution fields.
 * \author F. Palacios, T. Economon, S.R. Copeland, W. Maier
 * \version 6.2.0 "Falcon"
 *
 * The current SU2 release has been coordinated by the
 * SU2 International Developers Society <www.su2devsociety.org>
 * with selected contributions from the open-source community.
 *
 * The main research teams contributing to the current release are:
 *  - Prof. Juan J. Alonso's group at Stanford University.
 *  - Prof. Piero Colonna's group at Delft University of Technology.
 *  - Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *  - Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *  - Prof. Rafael Palacios' group at Imperial College London.
 *  - Prof. Vincent Terrapon's group at the University of Liege.
 *  - Prof. Edwin van der Weide's group at the University of Twente.
 *  - Lab. of New Concepts in Aeronautics at Tech. Institute of Aeronautics.
 *
 * Copyright 2012-2019, Francisco D. Palacios, Thomas D. Economon,
 *                      Tim Albring, and the SU2 contributors.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/variables/CNEMONSVariable.hpp"
#include <math.h>

CNEMONSVariable::CNEMONSVariable(su2double val_pressure,
                                 const su2double *val_massfrac,
                                 su2double *val_mach, 
                                 su2double val_temperature,
                                 su2double val_temperature_ve,
                                 unsigned long npoint,
                                 unsigned long val_ndim,
                                 unsigned long val_nvar,
                                 unsigned long val_nvarprim,
                                 unsigned long val_nvarprimgrad,
                                 CConfig *config,
                                 CNEMOGas *fluidmodel) : CNEMOEulerVariable(val_pressure,
                                                                       val_massfrac,
                                                                       val_mach,
                                                                       val_temperature,
                                                                       val_temperature_ve,
                                                                       npoint,
                                                                       val_ndim,
                                                                       val_nvar,
                                                                       val_nvarprim,
                                                                       val_nvarprimgrad,
                                                                       config,
                                                                       fluidmodel) {
                               


  Temperature_Ref = config->GetTemperature_Ref();
  Viscosity_Ref   = config->GetViscosity_Ref();
  Viscosity_Inf   = config->GetViscosity_FreeStreamND();
  Prandtl_Lam     = config->GetPrandtl_Lam();
  DiffusionCoeff.resize(nPoint, nSpecies)  = su2double(0.0);
  //Dij.resize(nPoint, nSpecies, nSpecies, 0.0);
  LaminarViscosity.resize(nPoint)  = su2double(0.0);
  ThermalCond.resize(nPoint)  = su2double(0.0);  
  ThermalCond_ve.resize(nPoint)  = su2double(0.0);

  Max_Lambda_Visc.resize(nPoint) = su2double(0.0); //Cat this should only exist in NSNEMO variable
    
}

bool CNEMONSVariable::SetVorticity(void) {

  for (unsigned long iPoint=0; iPoint<nPoint; ++iPoint) {

    su2double u_y = Gradient_Primitive(iPoint, VEL_INDEX, 1);
    su2double v_x = Gradient_Primitive(iPoint, VEL_INDEX+1, 0);
    su2double u_z = 0.0;
    su2double v_z = 0.0;
    su2double w_x = 0.0;
    su2double w_y = 0.0;

    if (nDim == 3) {
      u_z = Gradient_Primitive(iPoint,VEL_INDEX, 2);
      v_z = Gradient_Primitive(iPoint,VEL_INDEX+1, 2);
      w_x = Gradient_Primitive(iPoint,VEL_INDEX+2, 0);
      w_y = Gradient_Primitive(iPoint,VEL_INDEX+2, 1);
    }

    Vorticity(iPoint,0) = w_y-v_z;
    Vorticity(iPoint,1) = -(w_x-u_z);
    Vorticity(iPoint,2) = v_x-u_y;

  }
  return false;
}

bool CNEMONSVariable::SetPrimVar_Compressible(unsigned long iPoint, CConfig *config, CNEMOGas *fluidmodel) {

  bool nonPhys, bkup;
  unsigned short iVar, iSpecies;

  nonPhys = Cons2PrimVar(config, Solution[iPoint], Primitive[iPoint], dPdU[iPoint], dTdU[iPoint], dTvedU[iPoint], eves[iPoint], Cvves[iPoint], fluidmodel);

  if (nonPhys) {
    for (iVar = 0; iVar < nVar; iVar++)
      Solution(iPoint,iVar) = Solution_Old(iPoint,iVar);
    bkup = Cons2PrimVar(config, Solution[iPoint], Primitive[iPoint], dPdU[iPoint], dTdU[iPoint], dTvedU[iPoint], eves[iPoint], Cvves[iPoint], fluidmodel);
  }

  SetVelocity2(iPoint);

  Ds                       = fluidmodel->GetDiffusionCoeff();
  for (iSpecies = 0; iSpecies < nSpecies; iSpecies++) 
    DiffusionCoeff(iPoint, iSpecies) = Ds[iSpecies];
  
  LaminarViscosity(iPoint) = fluidmodel->GetViscosity();

  thermalconductivities    = fluidmodel->GetThermalConductivities();
  ThermalCond(iPoint)      = thermalconductivities[0];
  ThermalCond_ve(iPoint)   = thermalconductivities[1];

  return nonPhys;
}



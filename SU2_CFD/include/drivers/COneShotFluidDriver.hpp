/*!
 * \file COneShotFluidOutput.hpp
 * \brief Headers of the main subroutines for driving one-shot problems.
 *        The subroutines and functions are in the <i>driver_structure.cpp</i> file.
 * \author B. Munguía
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

#pragma once
#include "CDiscAdjSinglezoneDriver.hpp"

/*!
 * \class COneShotFluidDriver
 * \brief Class that extends the DiscAdjFluidDriver class with methods for one-shot optimization
 * \author L. Kusch,  B. Munguía
 * \version 6.0.1 "Falcon"
 */
class COneShotFluidDriver : public CDiscAdjSinglezoneDriver {

protected:
  unsigned short RecordingState; /*!< \brief The kind of recording the tape currently holds.*/

  unsigned short nDV_Total; /*!< \brief Total number of design variables used in optimization.*/
  unsigned short nConstr;   /*!< \brief Total number of constraints used in optimization.*/

  unsigned long OneShotIter; /*!< \brief Iterations of One-Shot solver, after PrimalDual. */

  bool Converged, ConvergedStore; /*!< \brief Whether or not the line search converged. */

  su2double* Gradient;                  /*!< \brief Vector to store gradient obtained from projection .*/
  su2double* ShiftLagGrad;              /*!< \brief Saved gradient N_u of the shifted Lagrangian .*/
  su2double* ShiftLagGradOld;           /*!< \brief Saved old gradient N_u of the shifted Lagrangian .*/
  su2double* DesignVarUpdate;           /*!< \brief Update of the design variable Delta u = u_{k+1} - u_k .*/
  su2double* SearchDirection;           /*!< \brief Search direction for optimization.*/
  su2double** BFGSInv;                  /*!< \brief Inverse matrix for BFGS update.*/
  su2double* DesignVar;                 /*!< \brief Current design variable value.*/
  su2double* AugLagGrad;                /*!< \brief Gradient of doubly augmented Lagrangian.*/
  su2double* AugLagGradAlpha;           /*!< \brief Alpha term of gradient of doubly augmented Lagrangian.*/
  su2double* AugLagGradBeta;            /*!< \brief Beta term of gradient of doubly augmented Lagrangian.*/
  su2double* AugLagGradGamma;           /*!< \brief Gamma term of gradient of doubly augmented Lagrangian.*/
  su2double* AugLagGradOld;             /*!< \brief Gradient of doubly augmented Lagrangian (old value).*/
  su2double* AugLagLamGrad;             /*!< \brief Gradient of doubly augmented Lagrangian wrt constraint multiplier.*/
  su2double Lagrangian, LagrangianOld;  /*!< \brief Value of doubly augmented Lagrangian.*/
  su2double GradDotDir;                 /*!< \brief Gradient dotted with search direction at first Armijo search step (stepsize = 1.0).*/
  su2double GradDotDirOld;              /*!< \brief Gradient dotted with search direction for previous iteration.*/
  su2double ObjFuncStore;               /*!< \brief Objective function at old flow, new design.*/
  
  su2double lb, ub;    /*!< \brief Lower and upper bounds of design variables.*/
  su2double epsilon;   /*!< \brief Estimator for the active set.*/
  su2double fdstep;    /*!< \brief Finite difference step size. */
  su2double stepsize0; /*!< \brief Initial guess for the stepsize.*/
  su2double CWolfeOne; /*!< \brief First Wolfe line search parameter.*/
  su2double CWolfeTwo; /*!< \brief Second Wolfe line search parameter.*/
  bool* ActiveSetDV;   /*!< \brief Flag for indices belonging to the active set (lower and upper design bounds are reached).*/

  su2double* ConstrFunc;         /*!< \brief Constraint function values.*/
  su2double* ConstrFuncStore;   /*!< \brief Constraint function values (stored for line search).*/
  su2double* ConstrFuncOld;     /*!< \brief Old constraint function values.*/
  su2double* Lambda;             /*!< \brief Lagrange multipliers for constraint functions.*/
  su2double* LambdaOld;         /*!< \brief Old Lagrange multipliers for constraint functions.*/
  su2double* LambdaStore;       /*!< \brief Old Lagrange multipliers for constraint functions.*/
  su2double* LambdaTilde;       /*!< \brief Stored Lagrange multipliers for update.*/
  su2double* LambdaTildeOld;   /*!< \brief Old stored Lagrange multipliers for update.*/
  su2double* LambdaTildeStore; /*!< \brief Old stored Lagrange multipliers for update.*/
  su2double** BCheckInv;        /*!< \brief Inverse matrix for multiplier update.*/
  su2double  BCheckNorm;        /*!< \brief Norm of the matrix for multiplier update.*/

  const unsigned short ALPHA_TERM = 0, BETA_TERM = 1, GAMMA_TERM = 2, SHIFTED = 3 , TOTAL_AUGMENTED = 3, TOTAL_AUGMENTED_OLD = 4;

public:

  /*!
    * \brief Constructor of the class.
    * \param[in] confFile - Configuration file name.
    * \param[in] val_nZone - Total number of zones.
    */
  COneShotFluidDriver(char* confFile,
                   unsigned short val_nZone,
                   SU2_Comm MPICommunicator);

  /*!
   * \brief Destructor of the class.
   */
  ~COneShotFluidDriver(void);

  /*!
   * \brief Preprocess the one-shot iteration
   * \param[in] TimeIter - index of the current time-step.
   */
  void Preprocess(unsigned long TimeIter);

  /*!
   * \brief Runs main routines of the one-shot class.
   */
  void Run();

  /*!
   * \brief Runs an optimization using the one-shot method.
   */
  void RunOneShot();

  /*!
   * \brief Executes one primal and dual iteration.
   */
  void PrimalDualStep();

  /*!
   * \brief Compute functionals to be used for objective and constraints.
   */
  void ComputeFunctionals();

  /*!
   * \brief Executes all operations needed to find the "BCheck"-term of the doubly augmented Lagrangian.
   */
  void ComputeGammaTerm();

  /*!
   * \brief Executes all operations needed to find the "alpha"-term of the doubly augmented Lagrangian.
   */
  void ComputeAlphaTerm();

  /*!
   * \brief Executes all operations needed to find the "beta"-term of the doubly augmented Lagrangian.
   */
  void ComputeBetaTerm();

  /*!
   * \brief Record one iteration of a flow iteration in within multiple zones.
   * \param[in] kind_recording - Type of recording (either CONS_VARS, MESH_COORDS, COMBINED or NONE)
   */
  void SetRecording(unsigned short kind_recording) override;

  /*!
   * \brief Projection of the surface sensitivity using algorithmic differentiation (AD) (see also SU2_DOT).
   * \param[in] kind_radient - Gradient term to be computed.
   */
  void SetProjection_AD(CSurfaceMovement *surface_movement);

  /*!
   * \brief Performs a surface deformation and volumetric deformation (see also SU2_DEF).
   * \param[in] surface_movement - surface movement class
   * \param[in] grid_movement - volumetric movement class
   */
  void SurfaceDeformation(CSurfaceMovement *surface_movement, CVolumetricMovement *grid_movement);

  /*!
   * \brief Update the inverse of the BFGS approximation.
   * \param[in] config - config class.
   */
  void BFGSUpdate(CConfig *config, unsigned short search_flag);

  /*!
   * \brief Compute the search direction using the approximated inverse, the gradient N_u and an active set projection.
   */
  void ComputeSearchDirection();

  /*!
   * \brief Compute the updated design variable with the search direction and the given step size.
   * \param[in] stepsize - factor for the line search.
   */  
  void ComputeDesignVarUpdate(su2double stepsize);

  /*!
   * \brief Check if the first Wolfe descent condition is fulfilled (line search condition).
   */
  unsigned short CheckArmijo(bool designing);

  /*!
   * \brief Store gradient dotted with search direction for first Armijo search iteration.
   */
  void StoreGradDotDir(bool designing);

  /*!
   * \brief Store gradient dotted with search direction used in previous iteration.
   */
  void StoreOldGradDotDir();

  /*!
   * \brief Perform parabolic backtracking.
   */
  su2double UpdateStepSizeQuadratic(void);

  /*!
   * \brief Bound step size.
   */
  su2double UpdateStepSizeBound(su2double stepsize, su2double a, su2double b);

  /*!
   * \brief Store values for the updated design variables.
   */
  void UpdateDesignVar();

  /*!
   * \brief Store the values for the Lagrangian and its gradient.
   */
  void StoreLagrangianInformation();

  /*!
   * \brief Calculate value for normal or doubly augmented Lagrangian.
   */
  void CalculateLagrangian();

  /*!
   * \brief Store the gradient of the shifted Lagrangian.
   */
  void SetShiftLagGrad();

  /*!
   * \brief Store the gradient of the augmented Lagrangian.
   * \param[in] kind - which gradient term is being stored (alpha, beta, gamma, total)
   */
  void SetAugLagGrad(unsigned short kind);

  /*!
   * \brief Initialize the adjoint value of the objective function with 0.0.
   */
  void SetAdj_ObjFunction_Zero();

  /*!
   * \brief Initialize the adjoint value of the constraint function with 0.0.
   */
  void SetAdj_ConstrFunction_Zero();

  /*!
   * \brief Find the indices for the epsilon-active set (overestimated).
   */
  void ComputeActiveSet(su2double stepsize);

  /*!
   * \brief Project mesh sensitivities to surface and design variables using AD.
   */
  void ProjectMeshSensitivities();

  /*!
   * \brief Project input value into feasible bounds of design space.
   * \param[in] value - value that is projected
   * \return projected value
   */
  su2double BoundProjection(su2double value);

  /*!
   * \brief Project a given vector value into the epsilon-active set (or inactive set).
   * \param[in] iDV - index of the respective value
   * \param[in] value - corresponding value
   * \param[in] active - is true if the projection is into the active set
   * \return projected value
   */
  su2double ProjectionSet(unsigned short iDV, su2double value, bool active);

  /*!
   * \brief Project a given matrix value into the epsilon-active set (or inactive set).
   * \param[in] iDV - row index of the respective value
   * \param[in] jDV - column index of the respective value
   * \param[in] value - corresponding value
   * \param[in] active - is true if the projection is into the active set
   * \return projected value
   */
  su2double ProjectionPAP(unsigned short iDV, unsigned short jDV, su2double value, bool active);

  /*!
   * \brief Store the constraint multiplier from the line search.
   */
  void StoreLambda();

    /*!
   * \brief Load the constraint multiplier from the line search.
   */
  void LoadLambdaStore();

  /*!
   * \brief Store the old constraint multiplier.
   */
  void StoreOldLambda();

    /*!
   * \brief Load the old constraint multiplier.
   */
  void LoadOldLambda();

  /*!
   * \brief Update the constraint multiplier.
   */
  void UpdateLambda(su2double stepsize);

  /*!
   * \brief Check the sign of active constraint multipliers.
   */
  void CheckLambda();

  /*!
   * \brief Store the gradient of the augmented Lagrangian wrt the constraint multiplier.
   */
  void StoreLambdaGrad();

  /*!
   * \brief Compute an initial value of the constraint multiplier.
   */
  void InitializeLambdaTilde(unsigned short iConstr);

  /*!
   * \brief Set the constraint functions and register if requested.
   */
  void SetConstrFunction();

  /*!
   * \brief Initialize the adjoint value of the constraint functions.
   */
  void SetAdj_ConstrFunction(su2double *seeding);

  /*!
   * \brief Store the objective.
   */
  void StoreObjFunction();

  /*!
   * \brief Store the constraints.
   */
  void StoreConstrFunction();

  /*!
   * \brief Store the old constraints.
   */
  void StoreOldConstrFunction();

  /*!
   * \brief Compute the inverse preconditioner matrix (BCheck^(-1)) for the multiplier update.
   */
  void ComputePreconditioner();
};
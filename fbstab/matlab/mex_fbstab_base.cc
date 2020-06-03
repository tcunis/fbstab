#include "mex_fbstab.h"

using namespace fbstab;

bool mexFBstabBase::action_handler(const mxArray* mxObj, const std::string& command, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( command == "Solve" )
  {
    Solve(mxObj,nlhs,plhs,nrhs,prhs);
    return true;
  }
  else if ( command == "UpdateOptions" )
  {
    UpdateOptions(mxObj,nlhs,plhs,nrhs,prhs);
    return true;
  }
  else if ( command == "DefaultOptions" )
  {
    DefaultOptions(mxObj,nlhs,plhs,nrhs,prhs);
    return true;
  }
  else if ( command == "ReliableOptions" )
  {
    ReliableOptions(mxObj,nlhs,plhs,nrhs,prhs);
    return true;
  }
  else
    return false;
}

mexSolverOut::mexSolverOut(const SolverOut& _sout)
{
  eflag = _sout.eflag;
  residual = _sout.residual;
  newton_iters = _sout.newton_iters;
  prox_iters = _sout.prox_iters;
  solve_time = _sout.solve_time;
  initial_residual = _sout.initial_residual;
}

mexSolverOut::operator mxArray* () const
{
  const char* _fieldnames[6] = {"eflag", "residual", "newton_iters", "prox_iters", "solve_time", "initial_residual"};
  mxArray* _plhs = mxCreateStructMatrix(1,1,6,_fieldnames);
  mxSetField(_plhs,0,"eflag",mxCreateString(eflag_str()));
  mxSetField(_plhs,0,"residual",mxCreateDoubleScalar(residual));
  mxSetField(_plhs,0,"newton_iters",mxCreateDoubleScalar(newton_iters));
  mxSetField(_plhs,0,"prox_iters",mxCreateDoubleScalar(prox_iters));
  mxSetField(_plhs,0,"solve_time",mxCreateDoubleScalar(solve_time));
  mxSetField(_plhs,0,"initial_residual",mxCreateDoubleScalar(initial_residual));
  
//  bool _failed = mxSetClassName(_pmx, "SolverOut");
//
//  if ( _failed ) //return NULL;
//    std::cout << "Unexpected error." << std::endl;
  
  // else
  return _plhs;
}

const char* mexSolverOut::eflag_str() const
{
  switch ( eflag )
  {
    case ExitFlag::SUCCESS:
      return "SUCCESS";
    case ExitFlag::DIVERGENCE:
      return "DIVERGENCE";
    case ExitFlag::MAXITERATIONS:
      return "MAXITERATIONS";
    case ExitFlag::PRIMAL_INFEASIBLE:
      return "PRIMAL_INFEASIBLE";
    case ExitFlag::DUAL_INFEASIBLE:
      return "DUAL_INFEASIBLE";
    case ExitFlag::PRIMAL_DUAL_INFEASIBLE:
      return "PRIMAL_DUAL_INFEASIBLE";
    default:
      return "";
  }
}

#define COPY(_O_,_V_)   _V_ = _O_._V_

mexAlgorithmParameters::mexAlgorithmParameters(const AlgorithmParameters& _opts)
{
  COPY(_opts,sigma0);
  COPY(_opts,sigma_max);
  COPY(_opts,sigma_min);
  COPY(_opts,alpha);
  COPY(_opts,beta);
  COPY(_opts,eta);
  COPY(_opts,delta);
  COPY(_opts,gamma);
  COPY(_opts,abs_tol);
  COPY(_opts,rel_tol);
  COPY(_opts,stall_tol);
  COPY(_opts,infeas_tol);
  COPY(_opts,inner_tol_max);
  COPY(_opts,inner_tol_min);
  COPY(_opts,max_newton_iters);
  COPY(_opts,max_prox_iters);
  COPY(_opts,max_inner_iters);
  COPY(_opts,max_linesearch_iters);
  COPY(_opts,check_feasibility);
  COPY(_opts,nonmonotone_linesearch);
  COPY(_opts,display_level);
}

#define MXGETDOUBLE(_P_,_V_)  _V_ = *mxGetDoubles(mxGetProperty(_P_,0,#_V_))
#define MXGETINT32S(_P_,_V_)  _V_ = *mxGetInt32s(mxGetProperty(_P_,0,#_V_))
#define MXGETBOOLEAN(_P_,_V_) _V_ = mxIsLogicalScalarTrue(mxGetProperty(_P_,0,#_V_))
#define MXGETDISPLAY(_P_,_V_) _V_ = (Display) *mxGetInt32s(mxGetProperty(_P_,0,#_V_))

mexAlgorithmParameters::mexAlgorithmParameters(const mxArray* _prhs)
{
  MXGETDOUBLE(_prhs,sigma0);
  MXGETDOUBLE(_prhs,sigma_max);
  MXGETDOUBLE(_prhs,sigma_min);
  MXGETDOUBLE(_prhs,alpha);
  MXGETDOUBLE(_prhs,beta);
  MXGETDOUBLE(_prhs,eta);
  MXGETDOUBLE(_prhs,delta);
  MXGETDOUBLE(_prhs,gamma);
  
  MXGETDOUBLE(_prhs,abs_tol);
  MXGETDOUBLE(_prhs,rel_tol);
  MXGETDOUBLE(_prhs,stall_tol);
  MXGETDOUBLE(_prhs,infeas_tol);
  
  MXGETDOUBLE(_prhs,inner_tol_max);
  MXGETDOUBLE(_prhs,inner_tol_min);
  
  MXGETINT32S(_prhs,max_newton_iters);
  MXGETINT32S(_prhs,max_prox_iters);
  MXGETINT32S(_prhs,max_inner_iters);
  
  MXGETINT32S(_prhs,max_linesearch_iters);
  
  MXGETBOOLEAN(_prhs,check_feasibility);
  MXGETBOOLEAN(_prhs,nonmonotone_linesearch);
  MXGETDISPLAY(_prhs,display_level);
}

#define MXSETDOUBLE(_P_,_V_)  mxSetField(_P_,0,#_V_,mxCreateDoubleScalar(_V_))
#define MXSETINT32S(_P_,_V_)  MXSETDOUBLE(_P_,_V_)
#define MXSETBOOLEAN(_P_,_V_) mxSetField(_P_,0,#_V_,mxCreateLogicalScalar(_V_))
#define MXSETDISPLAY(_P_,_V_) mxSetField(_P_,0,#_V_,mxCreateDoubleScalar((int) _V_))

mexAlgorithmParameters::operator mxArray* () const
{
  const char* _fieldnames[21] = {"sigma0", "sigma_max", "sigma_min", "alpha", "beta", "eta", "delta", "gamma", "abs_tol", "rel_tol", "stall_tol", "infeas_tol", "inner_tol_max", "inner_tol_min", "max_newton_iters", "max_prox_iters", "max_inner_iters", "max_linesearch_iters", "check_feasibility", "nonmonotone_linesearch", "display_level"};
  mxArray* _plhs = mxCreateStructMatrix(1,1,21,_fieldnames);
  
  MXSETDOUBLE(_plhs,sigma0);
  MXSETDOUBLE(_plhs,sigma_max);
  MXSETDOUBLE(_plhs,sigma_min);
  MXSETDOUBLE(_plhs,alpha);
  MXSETDOUBLE(_plhs,beta);
  MXSETDOUBLE(_plhs,eta);
  MXSETDOUBLE(_plhs,delta);
  MXSETDOUBLE(_plhs,gamma);
  
  MXSETDOUBLE(_plhs,abs_tol);
  MXSETDOUBLE(_plhs,rel_tol);
  MXSETDOUBLE(_plhs,stall_tol);
  MXSETDOUBLE(_plhs,infeas_tol);
  
  MXSETDOUBLE(_plhs,inner_tol_max);
  MXSETDOUBLE(_plhs,inner_tol_min);
  
  MXSETINT32S(_plhs,max_newton_iters);
  MXSETINT32S(_plhs,max_prox_iters);
  MXSETINT32S(_plhs,max_inner_iters);
  
  MXSETINT32S(_plhs,max_linesearch_iters);
  
  MXSETBOOLEAN(_plhs,check_feasibility);
  MXSETBOOLEAN(_plhs,nonmonotone_linesearch);
  MXSETDISPLAY(_plhs,display_level);
  
  return _plhs;
}

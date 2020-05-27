#include "mex_fbstab.h"

using namespace fbstab;

bool mexFBstabBase::action_handler(const mxArray* mxObj, const std::string& command, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( command == "Solve" )
  {
      Solve(mxObj,nlhs,plhs,nrhs,prhs);
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
  mxArray* _pmx = mxCreateStructMatrix(1,1,6,_fieldnames);
  mxSetField(_pmx,0,"eflag",mxCreateString(eflag_str()));
  mxSetField(_pmx,0,"residual",mxCreateDoubleScalar(residual));
  mxSetField(_pmx,0,"newton_iters",mxCreateDoubleScalar(newton_iters));
  mxSetField(_pmx,0,"prox_iters",mxCreateDoubleScalar(prox_iters));
  mxSetField(_pmx,0,"solve_time",mxCreateDoubleScalar(solve_time));
  mxSetField(_pmx,0,"initial_residual",mxCreateDoubleScalar(initial_residual));
  
//  bool _failed = mxSetClassName(_pmx, "SolverOut");
//
//  if ( _failed ) //return NULL;
//    std::cout << "Unexpected error." << std::endl;
  
  // else
  return _pmx;
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

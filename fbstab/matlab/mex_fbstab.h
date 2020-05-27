#pragma once

#include "mex.h"

#include "fbstab/fbstab_mpc.h"

class mexFBstabBase
{
public:
  static bool static_handler(const std::string& command, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) { return false; }
  
  virtual bool action_handler(const mxArray* mxObj, const std::string& command, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
  
protected:
  virtual void Solve(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) = 0;
};

struct mexSolverOut : public fbstab::SolverOut
{
  mexSolverOut(const fbstab::SolverOut&);
  
  operator mxArray*() const;
  
  const char* eflag_str() const;
};

class mexFBstabMpc : public mexFBstabBase
{
public:
  struct ProblemData : public fbstab::FBstabMpc::ProblemData {
    ProblemData(const mxArray*);
  
    int N, nx, nu, nc;
  };
  
  struct Variable : public fbstab::FBstabMpc::Variable {
    Variable(const mxArray*, int N, int nx, int nu, int nc);
  };

  static std::string get_classname() { return "FBstabMpc"; }
  
  mexFBstabMpc(const mxArray* mxObj, int nrhs, const mxArray* prhs[]);
  
protected:
  void Solve(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
  
private:
  fbstab::FBstabMpc _solver;
};

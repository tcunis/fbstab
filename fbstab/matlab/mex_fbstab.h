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
  
  virtual void UpdateOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) = 0;
  
  virtual void DefaultOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) = 0;
  virtual void ReliableOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) = 0;
};

struct mexSolverOut : public fbstab::SolverOut
{
  mexSolverOut(const fbstab::SolverOut&);
  
  operator mxArray*() const;
  
  const char* eflag_str() const;
};

struct mexAlgorithmParameters : virtual public fbstab::AlgorithmParameters
{
  mexAlgorithmParameters(const fbstab::AlgorithmParameters&);
  mexAlgorithmParameters(const mxArray*);
  
  operator mxArray*() const;
};

class mexFBstabMpc : public mexFBstabBase
{
public:
  struct ProblemData : public fbstab::FBstabMpc::ProblemDataRef {
    ProblemData(const mxArray*);
  
    int N, nx, nu, nc;
  };
  
  struct Variable : public fbstab::FBstabMpc::VariableRef {
    Variable(const mxArray*);
    
    operator mxArray*() const;
    
    int nz, nl, nv;
    
    private:
      const mxArray* pmx;
  };
  
  struct Options : public fbstab::FBstabMpc::Options, public mexAlgorithmParameters {
    Options(const fbstab::FBstabMpc::Options& opts) : mexAlgorithmParameters(opts) {}
    Options(const mxArray* prhs) : mexAlgorithmParameters(prhs) {}
  };

  static std::string get_classname() { return "FBstabMpc"; }
  
  mexFBstabMpc(const mxArray* mxObj, int nrhs, const mxArray* prhs[]);
  
protected:
  void Solve(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
  
  void UpdateOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
  
  void DefaultOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
  void ReliableOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
  
private:
  fbstab::FBstabMpc solver_;
};

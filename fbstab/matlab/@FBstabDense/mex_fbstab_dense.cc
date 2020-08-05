#include "mex_fbstab.h"

#include "matrix.h"
#include "mexObjectHandler.h"

#define GetMX(_T_,_I_,_N_,_P_)  (_I_ < _N_ ? (_T_) mxGetScalar(_P_[_I_]) : 1)

using namespace fbstab;

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  mexObjectHandler<mexFBstabDense>(nlhs, plhs, nrhs, prhs);
}


/** mexFBstabDense */
mexFBstabDense::mexFBstabDense(const mxArray* mxObj, int nrhs, const mxArray* prhs[])
: mexFBstabBase(),
  solver_(
    GetMX(int,0,nrhs,prhs),
    GetMX(int,1,nrhs,prhs),
    GetMX(int,2,nrhs,prhs)
  )
{
  if ( nrhs != 3 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Constructor requires three (3) arguments.");
}

void mexFBstabDense::Solve(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 2 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Solve method expects two (2) input arguments.");
  if ( nlhs != 2 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Solve method expects two (2) output argument.");
  if ( !mxIsClass(prhs[0],"FBstabDense.ProblemData") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "First argument of Solve method must be FBstabDense.ProblemData object.");
  if ( !mxIsClass(prhs[1],"FBstabDense.Variable") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Second argument of Solve method must be FBstabDense.Variable object.");
  
  // else
  mexFBstabDense::ProblemData qp(prhs[0]);
  mexFBstabDense::Variable x(prhs[1]);
  
  mexSolverOut sout = solver_.Solve<FBstabDense::ProblemDataRef,FBstabDense::VariableRef>(qp, &x);

  plhs[0] = sout;
  plhs[1] = x; //mxDuplicateArray(prhs[1]);
}

void mexFBstabDense::UpdateOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 1 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "UpdateOptions expects one (1) input argument.");
  if ( nlhs != 0 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "UpdateOptions does not expect output arguments.");
  if ( !mxIsClass(prhs[0],"FBstabDense.Options") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "First argument of UpdateOptions must be FBstabDense.Options object.");
    
  // else
  mexFBstabDense::Options options(prhs[0]);
  
  solver_.UpdateOptions(options);
}

void mexFBstabDense::ReliableOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 0 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "ReliableOptions does not expect input arguments.");
  if ( nlhs != 1 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "ReliableOptions expects one (1) output argument.");
  
  // else
  mexFBstabDense::Options options = solver_.ReliableOptions();
  
  plhs[0] = options;
}

void mexFBstabDense::DefaultOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 0 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "DefaultOptions does not expect input arguments.");
  if ( nlhs != 1 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "DefaultOptions expects one (1) output argument.");
  
  // else
  mexFBstabDense::Options options = solver_.DefaultOptions();
  
  plhs[0] = options;
}

mexFBstabDense::ProblemData::ProblemData(const mxArray* pmx)
{
    if ( !mxIsLogicalScalarTrue(mxGetProperty(pmx,0,"isvalid")) )
      throw mexRuntimeError(get_classname() + ":invalidArguments", "Invalid problem data.");
    
    // else
    nz = *mxGetInt32s(mxGetProperty(pmx,0,"nz"));
    nl = *mxGetInt32s(mxGetProperty(pmx,0,"nl"));
    nv = *mxGetInt32s(mxGetProperty(pmx,0,"nv"));
    
    mxArray* _H = mxGetProperty(pmx,0,"H");
    mxArray* _G = mxGetProperty(pmx,0,"G");
    mxArray* _A = mxGetProperty(pmx,0,"A");
    mxArray* _f = mxGetProperty(pmx,0,"f");
    mxArray* _h = mxGetProperty(pmx,0,"h");
    mxArray* _b = mxGetProperty(pmx,0,"b");
    
    new (&H) Eigen::Map<const Eigen::MatrixXd>(mxGetDoubles(_H), nz, nz);
    new (&G) Eigen::Map<const Eigen::MatrixXd>(mxGetDoubles(_G), nl, nz);
    new (&A) Eigen::Map<const Eigen::MatrixXd>(mxGetDoubles(_A), nv, nz);
    new (&f) Eigen::Map<const Eigen::VectorXd>(mxGetDoubles(_f), nz);
    new (&h) Eigen::Map<const Eigen::VectorXd>(mxGetDoubles(_h), nl);
    new (&b) Eigen::Map<const Eigen::VectorXd>(mxGetDoubles(_b), nv);
}

mexFBstabDense::Variable::Variable(const mxArray* _prhs)
: pmx(_prhs)
{
  if ( !mxIsLogicalScalarTrue(mxGetProperty(pmx,0,"isvalid")) )
      throw mexRuntimeError(get_classname() + ":invalidArguments", "Invalid solution structure.");
      
  // else
  nz = *mxGetInt32s(mxGetProperty(pmx,0,"nz"));
  nl = *mxGetInt32s(mxGetProperty(pmx,0,"nl"));
  nv = *mxGetInt32s(mxGetProperty(pmx,0,"nv"));
  
  mxArray* _z = mxGetProperty(pmx,0,"z");
  mxArray* _l = mxGetProperty(pmx,0,"l");
  mxArray* _v = mxGetProperty(pmx,0,"v");
  mxArray* _y = mxGetProperty(pmx,0,"y");
  
  new (&z) Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_z), nz);
  new (&l) Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_l), nl);
  new (&v) Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_v), nv);
  new (&y) Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_y), nv);
}

mexFBstabDense::Variable::operator mxArray* () const
{
  mxArray* _plhs = mxDuplicateArray(pmx);
  
  mxArray* _z = mxGetProperty(_plhs,0,"z");
  mxArray* _l = mxGetProperty(_plhs,0,"l");
  mxArray* _v = mxGetProperty(_plhs,0,"v");
  mxArray* _y = mxGetProperty(_plhs,0,"y");
  
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_z), nz) = z;
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_l), nl) = l;
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_v), nv) = v;
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_y), nv) = y;
  
  mxSetProperty(_plhs,0,"z",_z);
  mxSetProperty(_plhs,0,"l",_l);
  mxSetProperty(_plhs,0,"v",_v);
  mxSetProperty(_plhs,0,"y",_y);
  
  return _plhs;
}


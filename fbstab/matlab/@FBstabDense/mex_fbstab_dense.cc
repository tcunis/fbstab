#include "mex_fbstab.h"

#include "matrix.h"
#include "mexObjectHandler.h"

#define GetMX(_T_,_I_,_N_,_P_)  (_I_ < _N_ ? (_T_) mxGetScalar(_P_[_I_]) : 1)

using namespace fbstab;

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  mexObjectHandler<mexFBstabMpc>(nlhs, plhs, nrhs, prhs);
}


/** mexFBstabMpc */
mexFBstabMpc::mexFBstabMpc(const mxArray* mxObj, int nrhs, const mxArray* prhs[])
: mexFBstabBase(),
  solver_(
    GetMX(int,0,nrhs,prhs),
    GetMX(int,1,nrhs,prhs),
    GetMX(int,2,nrhs,prhs),
    GetMX(int,3,nrhs,prhs)
  )
{
  if ( nrhs != 4 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Constructor requires four (4) arguments.");
}

void mexFBstabMpc::Solve(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 2 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Solve method expects two (2) input arguments.");
  if ( nlhs != 2 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Solve method expects two (2) output argument.");
  if ( !mxIsClass(prhs[0],"FBstabMpc.ProblemData") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "First argument of Solve method must be FBstabMpc.ProblemData object.");
  if ( !mxIsClass(prhs[1],"FBstabMpc.Variable") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Second argument of Solve method must be FBstabMpc.Variable object.");
  
  // else
  mexFBstabMpc::ProblemData qp(prhs[0]);
  mexFBstabMpc::Variable x(prhs[1]);
  
  mexSolverOut sout = solver_.Solve<FBstabMpc::ProblemDataRef,FBstabMpc::VariableRef>(qp, &x);
  
  plhs[0] = sout;
  plhs[1] = x; //mxDuplicateArray(prhs[1]);
}

void mexFBstabMpc::UpdateOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 1 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "UpdateOptions expects one (1) input argument.");
  if ( nlhs != 0 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "UpdateOptions does not expect output arguments.");
  if ( !mxIsClass(prhs[0],"FBstabMpc.Options") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "First argument of UpdateOptions must be FBstabMpc.Options object.");
    
  // else
  mexFBstabMpc::Options options(prhs[0]);
  
  solver_.UpdateOptions(options);
}

void mexFBstabMpc::ReliableOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 0 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "ReliableOptions does not expect input arguments.");
  if ( nlhs != 1 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "ReliableOptions expects one (1) output argument.");
  
  // else
  mexFBstabMpc::Options options = solver_.ReliableOptions();
  
  plhs[0] = options;
}

void mexFBstabMpc::DefaultOptions(const mxArray* mxObj, int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if ( nrhs != 0 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "DefaultOptions does not expect input arguments.");
  if ( nlhs != 1 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "DefaultOptions expects one (1) output argument.");
  
  // else
  mexFBstabMpc::Options options = solver_.DefaultOptions();
  
  plhs[0] = options;
}

mexFBstabMpc::ProblemData::ProblemData(const mxArray* pmx)
{
    if ( !mxIsLogicalScalarTrue(mxGetProperty(pmx,0,"isvalid")) )
      throw mexRuntimeError(get_classname() + ":invalidArguments", "Invalid problem data.");
    
    // else
    N = *mxGetInt32s(mxGetProperty(pmx,0,"N"));
    nx = *mxGetInt32s(mxGetProperty(pmx,0,"nx"));
    nu = *mxGetInt32s(mxGetProperty(pmx,0,"nu"));
    nc = *mxGetInt32s(mxGetProperty(pmx,0,"nc"));
    
    mxArray* _Q = mxGetProperty(pmx,0,"Q");
    mxArray* _R = mxGetProperty(pmx,0,"R");
    mxArray* _S = mxGetProperty(pmx,0,"S");
    mxArray* _q = mxGetProperty(pmx,0,"q");
    mxArray* _r = mxGetProperty(pmx,0,"r");
    mxArray* _A = mxGetProperty(pmx,0,"A");
    mxArray* _B = mxGetProperty(pmx,0,"B");
    mxArray* _c = mxGetProperty(pmx,0,"c");
    mxArray* _E = mxGetProperty(pmx,0,"E");
    mxArray* _L = mxGetProperty(pmx,0,"L");
    mxArray* _d = mxGetProperty(pmx,0,"d");
    
    Q = MapMatrixSequence(mxGetDoubles(_Q), N+1, nx, nx);
    R = MapMatrixSequence(mxGetDoubles(_R), N+1, nu, nu);
    S = MapMatrixSequence(mxGetDoubles(_S), N+1, nu, nx);
    q = MapMatrixSequence(mxGetDoubles(_q), N+1, nx);
    r = MapMatrixSequence(mxGetDoubles(_r), N+1, nu);
    
    A = MapMatrixSequence(mxGetDoubles(_A), N, nx, nx);
    B = MapMatrixSequence(mxGetDoubles(_B), N, nx, nu);
    c = MapMatrixSequence(mxGetDoubles(_c), N, nx);
    
    E = MapMatrixSequence(mxGetDoubles(_E), N+1, nc, nx);
    L = MapMatrixSequence(mxGetDoubles(_L), N+1, nc, nu);
    d = MapMatrixSequence(mxGetDoubles(_d), N+1, nc);
    
    mxArray* _x0 = mxGetProperty(pmx,0,"x0");
    
    new (&x0) Eigen::Map<const Eigen::VectorXd>(mxGetDoubles(_x0), nx);
}

mexFBstabMpc::Variable::Variable(const mxArray* _prhs)
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

mexFBstabMpc::Variable::operator mxArray* () const
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


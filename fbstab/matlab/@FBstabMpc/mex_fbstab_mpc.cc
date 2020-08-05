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
  mexFBstabMpc::Variable x(prhs[1], qp.N, qp.nx, qp.nu, qp.nc);
  
  mexSolverOut sout = solver_.Solve<FBstabMpc::ProblemData,FBstabMpc::Variable>(qp, &x);
  
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
    
    Q = MatrixSequence(N+1, nx, nx);
    R = MatrixSequence(N+1, nu, nu);
    S = MatrixSequence(N+1, nu, nx);
    q = MatrixSequence(N+1, nx);
    r = MatrixSequence(N+1, nu);
    
    A = MatrixSequence(N, nx, nx);
    B = MatrixSequence(N, nx, nu);
    c = MatrixSequence(N, nx);
    
    E = MatrixSequence(N+1, nc, nx);
    L = MatrixSequence(N+1, nc, nu);
    d = MatrixSequence(N+1, nc);
    
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
    
    for (int _i=0; _i < N+1; _i++)
    {
      Q(_i) = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_Q)+_i*nx*nx, nx, nx);
      R(_i) = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_R)+_i*nu*nu, nu, nu);
      S(_i) = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_S)+_i*nu*nx, nu, nx);
      q(_i) = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_q)+_i*nx, nx);
      r(_i) = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_r)+_i*nu, nu);

      E(_i) = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_E)+_i*nc*nx, nc, nx);
      L(_i) = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_L)+_i*nc*nu, nc, nu);
      d(_i) = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_d)+_i*nc, nc);

      if ( _i >= N ) continue;

      A(_i) = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_A)+_i*nx*nx, nx, nx);
      B(_i) = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_B)+_i*nx*nu, nx, nu);
      c(_i) = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_c)+_i*nx, nx);
    }
    
    mxArray* _x0 = mxGetProperty(pmx,0,"x0");
    
    x0 = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_x0), nx);
}

mexFBstabMpc::Variable::Variable(const mxArray* _prhs, int _N, int _nx, int _nu, int _nc)
: FBstabMpc::Variable(_N, _nx, _nu, _nc), pmx(_prhs)
{
  if ( !mxIsLogicalScalarTrue(mxGetProperty(pmx,0,"isvalid")) )
      throw mexRuntimeError(get_classname() + ":invalidArguments", "Invalid solution structure.");
      
  // else
  int _nz, _nl, _nv;
  _nz = *mxGetInt32s(mxGetProperty(pmx,0,"nz"));
  _nl = *mxGetInt32s(mxGetProperty(pmx,0,"nl"));
  _nv = *mxGetInt32s(mxGetProperty(pmx,0,"nv"));
  
  mxArray* _z = mxGetProperty(pmx,0,"z");
  mxArray* _l = mxGetProperty(pmx,0,"l");
  mxArray* _v = mxGetProperty(pmx,0,"v");
  mxArray* _y = mxGetProperty(pmx,0,"y");
  
  z = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_z), _nz);
  l = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_l), _nl);
  v = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_v), _nv);
  y = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_y), _nv);
}

mexFBstabMpc::Variable::operator mxArray* () const
{
  mxArray* _plhs = mxDuplicateArray(pmx);
  
  int _nz, _nl, _nv;
  _nz = *mxGetInt32s(mxGetProperty(pmx,0,"nz"));
  _nl = *mxGetInt32s(mxGetProperty(pmx,0,"nl"));
  _nv = *mxGetInt32s(mxGetProperty(pmx,0,"nv"));
  
  mxArray* _z = mxGetProperty(_plhs,0,"z");
  mxArray* _l = mxGetProperty(_plhs,0,"l");
  mxArray* _v = mxGetProperty(_plhs,0,"v");
  mxArray* _y = mxGetProperty(_plhs,0,"y");
  
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_z), _nz) = z;
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_l), _nl) = l;
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_v), _nv) = v;
  Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_y), _nv) = y;
  
  mxSetProperty(_plhs,0,"z",_z);
  mxSetProperty(_plhs,0,"l",_l);
  mxSetProperty(_plhs,0,"v",_v);
  mxSetProperty(_plhs,0,"y",_y);
  
  return _plhs;
}

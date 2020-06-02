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
  _solver(
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
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Solve method requires two (2) input arguments.");
  if ( nlhs != 2 )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Solve method requires two (2) output argument.");
  if ( !mxIsClass(prhs[0],"FBstabMpc.ProblemData") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "First argument of Solve method must be FBstab::ProblemData object.");
  if ( !mxIsClass(prhs[1],"FBstabMpc.Variable") )
    throw mexRuntimeError(get_classname() + ":invalidArguments", "Second argument of Solve method must be FBstab::Variable object.");
  
  // else
  mexFBstabMpc::ProblemData qp(prhs[0]);
  mexFBstabMpc::Variable x(prhs[1], qp.N, qp.nx, qp.nu, qp.nc);
  
  mexSolverOut sout = _solver.Solve<FBstabMpc::Variable>(qp, &x);
  
  plhs[0] = sout;
  plhs[1] = x; //mxDuplicateArray(prhs[1]);
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
    
    Q = std::vector<Eigen::MatrixXd>(N+1);
    R = std::vector<Eigen::MatrixXd>(N+1);
    S = std::vector<Eigen::MatrixXd>(N+1);
    q = std::vector<Eigen::VectorXd>(N+1);
    r = std::vector<Eigen::VectorXd>(N+1);
    
    A = std::vector<Eigen::MatrixXd>(N);
    B = std::vector<Eigen::MatrixXd>(N);
    c = std::vector<Eigen::VectorXd>(N);
    
    E = std::vector<Eigen::MatrixXd>(N+1);
    L = std::vector<Eigen::MatrixXd>(N+1);
    d = std::vector<Eigen::VectorXd>(N+1);
    
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
      Q[_i] = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_Q)+_i*nx*nx, nx, nx);
      R[_i] = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_R)+_i*nu*nu, nu, nu);
      S[_i] = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_S)+_i*nu*nx, nu, nx);
      q[_i] = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_q)+_i*nx, nx);
      r[_i] = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_r)+_i*nu, nu);
      
      E[_i] = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_E)+_i*nc*nx, nc, nx);
      L[_i] = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_L)+_i*nc*nu, nc, nu);
      d[_i] = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_d)+_i*nc, nc);
      
      if ( _i >= N ) continue;
      
      A[_i] = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_A)+_i*nx*nx, nx, nx);
      B[_i] = Eigen::Map<Eigen::MatrixXd>(mxGetDoubles(_B)+_i*nx*nu, nx, nu);
      c[_i] = Eigen::Map<Eigen::VectorXd>(mxGetDoubles(_c)+_i*nx, nx);
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


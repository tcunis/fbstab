classdef FBstabMpc < FBstabBase
% FBstabMpc implements the Proximally Stabilized Semismooth Method for
% solving the following quadratic programming problem (1):
%
%     min.  \sum_{i=0}^N 1/2 [x(i)]'% [Q(i) S(i)'] [x(i)] + [q(i)]'*[x(i)]
%                            [u(i)]    [S(i) R(i) ] [u(i)]   [r(i)]  [u(i)]
%
%     s.t.  x(i+1) = A(i)*x(i) + B(i) u(i) + c(i), i = 0 ... N-1
%           x(0) = x0
%           E(i)*x(i) + L(i)*u(i) + d(i) <= 0,     i = 0 ... N
%
% Where
%        [ Q(i),S(i)']
%        [ S(i),R(i) ]
%
% is positive semidefinite for all i \in [0,N].
% See also (29) in https://arxiv.org/pdf/1901.04046.pdf.
%
% The problem is of size (N,nx,nu,nc) where:
% - N > 0 is the horizon length
% - nx > 0 is the number of states
% - nu > 0 is the number of control inputs
% - nc > 0 is the number of constraints per stage
%
% This is a specialization of the general form (2),
%
%     min.  1/2 z'Hz + f'z
%
%     s.t.  Gz =  h
%           Az <= b
%
% which has dimensions nz = (nx + nu)% (N + 1), nl = nx * (N + 1),
% and nv = nc * (N + 1).
%
% Aside from convexity there are no assumptions made about the problem.
% This method can detect unboundedness/infeasibility
% and exploit arbitrary initial guesses.
%%

methods (Access=protected, Static, Hidden)
    varargout = mex_fbstab_mpc(varargin);
    
    function varargout = mexfcn(varargin)
        % Implement FBstabBase#mexfcn
        varargout = cell(1,nargout);
        [varargout{:}] = FBstabMpc.mex_fbstab_mpc(varargin{:});
    end
end

methods
    function obj=FBstabMpc(varargin)
        % Allocates workspaces needed when solving (1).
        %
        %   obj=FBstabMpc(N,nx,nu,nc)
        %   obj=FBstabMpc(s)
        %
        % @param[in] N Horizon length
        % @param[in] nx number of states
        % @param[in] nu number of control input
        % @param[in] nc number of constraints per timestep
        % @param[in] s Inputs s = (N, nx, nu, nc)
        %
        % Throws a runtime_error if any inputs are nonpositive.
        if nargin == 1 && length(varargin{1}) == 4
            varargin = num2cell(varargin{1});
        end
        obj = obj@FBstabBase(varargin{:});
    end
    
    function [sout,x] = Solve(obj,qp,x)
    	% Solves an instance of (1).
        %
        % @param[in]     qp problem data
        % @param[in,out] x  initial guess, overwritten with the solution
        % @param[in]     use_initial_guess if false the solver is initialized at the
        % origin
        % @return       Summary of the optimizer output, see fbstab_algorithm.h.
        [sout,x] = obj.Solve@FBstabBase(qp,x);
    end
end

end

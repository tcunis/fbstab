classdef FBstabDense < FBstabBase
% FBstabDense implements the Proximally Stabilized Semismooth Algorithm
% for solving convex quadratic programs of the following form (1):
%
%     min.    1/2  z'Hz + f'z
%     s.t.    Gz = h
%             Az <= b
%
% where H is symmetric and positive semidefinite and its dual
%
%     min.   1/2  z'Hz + b'v + h'l
%     s.t.   Hz + f + A'v + G'l = 0
%            v >= 0.
%
% Or equivalently for solving its KKT system
%
%     Hz + f + A'v + G'l = 0
%     h - Gz = 0
%     Az <= b, v >= 0
%     (b - Az)' v = 0
%
% where v is a dual variable.
%
% The algorithm is described in https://arxiv.org/pdf/1901.04046.pdf.
% Aside from convexity there are no assumptions made about the problem.
% This method can detect unboundedness/infeasibility and accepts
% arbitrary initial guesses.
%
% The problem is of size (nz, nl, nv) where:
% - nz > 0 is the number of decision variables
% - nl >= 0 is the number of equality constraints
% - nv > 0 is the number of inequality constraints
%%

methods (Access=protected, Static, Hidden)
    varargout = mex_fbstab_dense(varargin);
    
    function varargout = mexfcn(varargin)
        % Implement FBstabBase#mexfcn
        varargout = cell(1,nargout);
        [varargout{:}] = FBstabDense.mex_fbstab_dense(varargin{:});
    end
end

methods
    function obj=FBstabDense(varargin)
        % Allocates needed workspace given the dimensions of the QPs to
        % be solved. Throws a runtime_error if any inputs are non-positive.
        %
        % @param[in] nz number of decision variables
        % @param[in] nl number of equality constraints
        % @param[in] nv number of inequality constraints
        obj = obj@FBstabBase(varargin{:});
    end
    
    function [sout,x] = Solve(obj,qp,x)
    	% Solves an instance of (1).
        %
        % @param[in]     qp problem data
        % @param[in,out] x  initial guess, overwritten with the solution
        % @return       Summary of the optimizer output, see fbstab_algorithm.h.
        [sout,x] = obj.Solve@FBstabBase(qp,x);
    end
    
    function UpdateOptions(obj,options)
        % Allows for setting of solver options. 
        %
        % See ALGORITHMPARAMETERS for a list of adjustable options.
        % @param[in] option New option struct
        obj.UpdateOptions@FBstabBase(options);
    end
    
    function options = DefaultOptions(obj)
        % Returns default settings, recommended for most problems.
        options = FBstabDense.Options.fromstruct(obj.DefaultOptions@FBstabBase);
    end
    
    function options = ReliableOptions(obj)
        % Settings for increased reliability for use on hard problems.
        options = FBstabDense.Options.fromstruct(obj.ReliableOptions@FBstabBase);
    end
end

end

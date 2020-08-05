classdef ProblemData
% Structure to hold the problem data.
%%

properties (Dependent)
    nz, nl, nv, isvalid;
end

properties
    H;  % nz x nz positive semidefinite Hessian matrix.
    G;  % nl x nz equality Jacobian
    A;  % nv x nz inequality Jacobian.
    f;  % nz linear cost.
    h;  % nl equality rhs
    b;  % nv inequality rhs.
end

methods
    function obj=ProblemData(nz,nl,nv)
        % obj=ProblemData
        % obj=ProblemData(nz,nl,nv)
        if nargin==0
            % nothing to do
            return
        end

        obj.H = zeros(nz,nz);
        obj.G = zeros(nl,nz);
        obj.A = zeros(nv,nz);
        obj.f = zeros(nz,1);
        obj.h = zeros(nl,1);
        obj.b = zeros(nv,1);
    end
    
    function obj=set.H(obj,value)
        % Set Hessian matrix
        obj.H = full(value);
    end
    
    function obj=set.G(obj,value)
        % Set equality Jacobian
        obj.G = full(value);
    end
    
    function obj=set.A(obj,value)
        % Set inequality Jacobian
        obj.A = full(value);
    end
    
    function obj=set.f(obj,value)
        % Set linear cost
        obj.f = full(value);
    end
    
    function obj=set.h(obj,value)
        % Set equality rhs
        obj.h = full(value);
    end
    
    function obj=set.b(obj,value)
        % Set inequality rhs
        obj.b = full(value);
    end
    
    function value=get.nz(obj)
        % Number of decision variables
        value = int32(length(obj.f));
    end
    
    function value=get.nl(obj)
        % Number of costates
        value = int32(length(obj.h));
    end
    
    function value=get.nv(obj)
        % Number of inequality duals
        value = int32(length(obj.b));
    end
    
    function value=get.isvalid(obj)
        % Check validity of problem data.
        value = all([
            assertsize(obj.H, [obj.nz,obj.nz])
            assertsize(obj.G, [obj.nl,obj.nz])
            assertsize(obj.A, [obj.nv,obj.nz])
            assertsize(obj.f, [obj.nz,1])
            assertsize(obj.h, [obj.nl,1])
            assertsize(obj.b, [obj.nv,1])
        ]);
    end
end

end
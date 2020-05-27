classdef Variable
% Structure to hold the initial guess and solution.
% These vectors will be overwritten by the solve routine.
%%

properties (Dependent)
    nz, nl, nv, isvalid;
end

properties
    z;  % Decision variables in \reals^nz
    l;  % Equality duals/costates in \reals^nl
    v;  % Inequality duals in \reals^nv
    y;  % Constraint margin, i.e., y = b-Az, in \reals^nv
end

methods
    function obj=Variable(N,nx,nu,nc)
        % obj=Variable(N,nx,nu,nc)
        % obj=Variable(s)
        if nargin==1
            nx = N(2);
            nu = N(3);
            nc = N(4);
            N  = N(1);
        end
        
        obj.z = zeros((N+1)*(nx+nu),1);
        obj.l = zeros((N+1)*nx,1);
        obj.v = zeros((N+1)*nc,1);
        obj.y = zeros((N+1)*nc,1);
    end
    
    function value=get.nz(obj)
        % Number of decision variables
        value=int32(length(obj.z));
    end
    
    function value=get.nl(obj)
        % Number of costates
        value=int32(length(obj.l));
    end
    
    function value=get.nv(obj)
        % Number of inequality duals
        value=int32(length(obj.v));
    end
    
    function value=get.isvalid(obj)
        % Check validity of solution structure.
        value = all([
            assertsize(obj.z, [obj.nz 1])
            assertsize(obj.l, [obj.nl 1])
            assertsize(obj.v, [obj.nv 1])
            assertsize(obj.y, [obj.nv 1])
        ]);
    end
end

end
classdef ProblemData
% Structure to hold references to the problem data.
% See the class documentation or (29) in 
% https://arxiv.org/pdf/1901.04046.pdf for more details.
%%

properties (Dependent)
    N, nx, nu, nc, isvalid;
end

properties
    Q;	% N + 1 vector of nx x nx matrices
    R;  % N + 1 vector of nu x nu matrices
    S;  % N + 1 vector of nu x nx matrices
    q;  % N + 1 vector of nx x 1 vectors
    r;  % N + 1 vector of nu x 1 vectors
    A;  % N vector of nx x nx matrices
    B;  % N  vector of nx x nu matrices
    c;  % N vector of nx vectors
    E;  % N + 1 vector of nc x nx matrices
    L;  % N + 1 vector of nc x nu matrices
    d;  % N + 1 vector of nc x 1 vectors
    
    x0;	% nx x 1 vector
end

methods
    function obj=ProblemData(N,nx,nu,nc)
        % obj=ProblemData
        % obj=ProblemData(N,nx,nu,nc)
        % obj=ProblemData(s)
        if nargin==0
            % nothing to do
            return
        elseif nargin==1
            nx = N(2);
            nu = N(3);
            nc = N(4);
            N  = N(1);
        end
        
        obj.Q = zeros(nx,nx,N+1);
        obj.R = zeros(nu,nu,N+1);
        obj.S = zeros(nu,nx,N+1);
        obj.q = zeros(nx,1,N+1);
        obj.r = zeros(nu,1,N+1);
        obj.A = zeros(nx,nx,N);
        obj.B = zeros(nx,nu,N);
        obj.c = zeros(nx,1,N);
        obj.E = zeros(nc,nx,N+1);
        obj.L = zeros(nc,nu,N+1);
        obj.d = zeros(nc,1,N+1);
        
        obj.x0 = zeros(nx,1);
    end
    
    function value=get.N(obj)
        % Horizon length
        value = int32(size(obj.A,3));
    end
    
    function value=get.nx(obj)
        % Number of states
        value = int32(size(obj.A,1));
    end
    
    function value=get.nu(obj)
        % Number of inputs
        value = int32(size(obj.B,2));
    end
    
    function value=get.nc(obj)
        % Number of constraints (per timestep)
        value = int32(size(obj.E,1));
    end
    
    function value=get.isvalid(obj)
        % Check validity of problem data.
        value = all([
            assertsize(obj.Q, [obj.nx,obj.nx,obj.N+1])
            assertsize(obj.R, [obj.nu,obj.nu,obj.N+1])
            assertsize(obj.S, [obj.nu,obj.nx,obj.N+1])
            assertsize(obj.q, [obj.nx,1,obj.N+1])
            assertsize(obj.r, [obj.nu,1,obj.N+1])
            assertsize(obj.A, [obj.nx,obj.nx,obj.N])
            assertsize(obj.B, [obj.nx,obj.nu,obj.N])
            assertsize(obj.c, [obj.nx,1,obj.N])
            assertsize(obj.E, [obj.nc,obj.nx,obj.N+1])
            assertsize(obj.L, [obj.nc,obj.nu,obj.N+1])
            assertsize(obj.d, [obj.nc,1,obj.N+1])
            assertsize(obj.x0, [obj.nx,1])
        ]);
    end
end

end
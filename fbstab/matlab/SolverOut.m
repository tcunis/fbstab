classdef SolverOut
% Output Data
%
% A negative value for solve_time indicates that no timing data is 
% available.

properties
    eflag;
    residual;
    newton_iters;
    prox_iters;
    solve_time;
    initial_residual;
end

end
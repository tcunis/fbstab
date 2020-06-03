classdef AlgorithmParameters
% Stores algorithm parameters.

properties
  sigma0;       %< Initial stabilization parameter
  sigma_max;    %< Maximum stabilization parameter
  sigma_min;    %< Minimum stabilization parameter
  alpha;        %< Penalized FB function parameter
  beta;         %< Backtracking parameter
  eta;          %< Sufficient decrease parameter
  delta;        %< Reduction factor for subproblem tolerance
  gamma;        %< Reduction factor for sigma

  abs_tol;      %< Absolute tolerance
  rel_tol;      %< Relative tolerance
  stall_tol;    %< Tolerance on ||dx||
  infeas_tol;   %< Relative tolerance for feasibility checking

  inner_tol_max;    %< Maximum value for the subproblem tolerance
  inner_tol_min;    %< Minimum value for the subproblem tolerance

  max_newton_iters;     %< Maximum number of Newton iterations
  max_prox_iters;       %< Maximum number of proximal iterations
  max_inner_iters;      %< Maximum number of iterations that can be applied 
                        %   to a single subproblem
  max_linesearch_iters; %< Maximum backtracking linesearch steps

  check_feasibility;            %< Controls the feasibility checker
  nonmonotone_linesearch;       %<  Controls nonmonotone linesearch
  display_level;                %< Controls verbosity
end

methods
    function obj=set.sigma0(obj,value)
        % Initial stabilization parameter
        obj.sigma0 = setNumeric(obj,'sigma0',value);
    end
    
    function obj=set.sigma_max(obj,value)
        % Maximum stabilization parameter
        obj.sigma_max = setNumeric(obj,'sigma_max',value);
    end
    
    function obj=set.sigma_min(obj,value)
        % Minimum stabilization parameter
        obj.sigma_min = setNumeric(obj,'sigma_min',value);
    end
    
    function obj=set.alpha(obj,value)
        % Penalized FB function parameter
        obj.alpha = setNumeric(obj,'alpha',value);
    end
    
    function obj=set.beta(obj,value)
        % Backtracking parameter
        obj.beta = setNumeric(obj,'beta',value);
    end
    
    function obj=set.eta(obj,value)
        % Sufficient decrease parameter
        obj.eta = setNumeric(obj,'eta',value);
    end
    
    function obj=set.delta(obj,value)
        % Reduction factor for subproblem tolerance
        obj.delta = setNumeric(obj,'delta',value);
    end
    
    function obj=set.gamma(obj,value)
        % Reduction factor for sigma
        obj.gamma = setNumeric(obj,'gamma',value);
    end

    function obj=set.abs_tol(obj,value)
        % Absolute tolerance
        obj.abs_tol = setNumeric(obj,'abs_tol',value);
    end
    
    function obj=set.rel_tol(obj,value)
        % Relative tolerance
        obj.rel_tol = setNumeric(obj,'rel_tol',value);
    end
    
    function obj=set.stall_tol(obj,value)
        % Tolerance on ||dx||
        obj.stall_tol = setNumeric(obj,'stall_tol',value);
    end
    
    function obj=set.infeas_tol(obj,value)
        % Relative tolerance for feasibility checking
        obj.infeas_tol = setNumeric(obj,'infeas_tol',value);
    end

    function obj=set.inner_tol_max(obj,value)
        % Maximum value for the subproblem tolerance
        obj.inner_tol_max = setNumeric(obj,'inner_tol_max',value);
    end
    
    function obj=set.inner_tol_min(obj,value)
        % Minimum value for the subproblem tolerance
        obj.inner_tol_min = setNumeric(obj,'inner_tol_min',value);
    end
    
    function obj=set.max_newton_iters(obj,value)
        % Maximum number of Newton iterations
        obj.max_newton_iters = setInteger(obj,'max_newton_iters',value);
    end
    
    function obj=set.max_prox_iters(obj,value)
        % Maximum number of proximal iterations
        obj.max_prox_iters = setInteger(obj,'max_prox_iters',value);
    end
    
    function obj=set.max_inner_iters(obj,value)
        % Maximum number of iterations that can be 
        % applied to a single subproblem
        obj.max_inner_iters = setInteger(obj,'max_inner_iters',value);
    end
    
    function obj=set.max_linesearch_iters(obj,value)
        % Maximum backtracking linesearch steps
        obj.max_linesearch_iters = setInteger(obj,'max_linesearch_iters',value);
    end
    
    function obj=set.check_feasibility(obj,value)
        % Controls the feasibility checker
        obj.check_feasibility = setLogical(obj,'check_feasibility',value);
    end
    
    function obj=set.nonmonotone_linesearch(obj,value)
        % Controls nonmonotone linesearch
        obj.nonmonotone_linesearch = setLogical(obj,'nonmonotone_linesearch',value);
    end
    
    function obj=set.display_level(obj,value)
        % Controls verbosity
        obj.display_level = setEnum(obj,'display_level',value,0:3);
    end
end     

methods (Access=protected)
    function obj=AlgorithmParameters
    end
    
    function obj=setstruct(obj,data)
        % Copy parameter values from data structure.
        for fd=fieldnames(data)'
            obj.(fd{:}) = data.(fd{:});
        end
    end
end

methods (Access=private)
    function value=setNumeric(~,param,value)
        % Set numeric parameter value.
        % Throws exception if value is not numeric scalar.
        assert(isscalar(value) && isnumeric(value), ...
            'Value for %s must be numeric scalar.', param);
    end
    
    function value=setInteger(~,param,value)
        % Set integer parameter value.
        % Throws exception if value is not numeric scalar.
        assert(isscalar(value) && isnumeric(value), ...
            'Value for %s must be integer scalar.', param);
        
        value = int32(value);
    end
    
    function value=setLogical(~,param,value)
        % Set logical parameter value.
        % Throws exception if value is not logical scalar.
        assert(isscalar(value) && islogical(value), ...
            'Value for %s must be logical scalar.', param);
        
        value = logical(value);
    end
    
    function value=setEnum(~,param,value,range)
        % Set enumeration parameter value.
        % Throws exception if value is not numeric scalar or out of range.
        assert(isscalar(value) && isnumeric(value) && ismember(value,range), ...
            'Value for %s must be integer scalar out of [%s].', param, num2str(range));
        
        value = int32(value);
    end
end

end

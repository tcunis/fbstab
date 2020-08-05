classdef Options < AlgorithmParameters
% A Structure to hold options.

methods (Access=protected)
    function obj=Options
    end
end

methods (Static)
    function obj=fromstruct(data)
        % Create Options object from data structure.
        
        obj = setstruct(FBstabDense.Options, data);
    end
end

end
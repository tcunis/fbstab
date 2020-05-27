function tf = assertsize(A, sz)
% Assert that size(A) is equal to sz.

tf = isequal(size(A), sz);

end
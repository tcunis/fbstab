% This file is part of the fbstab-matlab library
%
% https://github.com/dliaomcp/fbstab-matlab
%
% and is subject to the BSD-3-Clause license 

% call the servo motor example
mpc = load_servo_example(30);
qp = form_sparse_mpc_qp(FBstabDense.ProblemData, mpc);

x0 = FBstabDense.Variable(qp.nz, qp.nl, qp.nv);

solver = FBstabDense(qp.nz, qp.nl, qp.nv);

opts = solver.DefaultOptions;

opts.display_level = 2;
solver.UpdateOptions(opts);
tic
[out,xopt] = solver.Solve(qp,x0);
toc
disp(out)

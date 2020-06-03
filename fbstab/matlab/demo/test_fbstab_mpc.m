% This file is part of the fbstab-matlab library
%
% https://github.com/dliaomcp/fbstab-matlab
%
% and is subject to the BSD-3-Clause license 

% call the servo motor example
[mpc,~,simin] = load_servo_example(50);

alpha = 0.95;
% create the data and linsys objects
data = FBstabMpc.ProblemData; %MpcData(mpc.Q,mpc.R,mpc.S,mpc.q,mpc.r,mpc.A,mpc.B,mpc.c,simin.x0,mpc.E,mpc.L,mpc.d);
data.Q = mpc.Q;
data.R = mpc.R;
data.S = mpc.S;
data.q = permute(mpc.q, [1 3 2]);
data.r = permute(mpc.r, [1 3 2]);
data.A = mpc.A;
data.B = mpc.B;
data.c = permute(mpc.c, [1 3 2]);
data.E = mpc.E;
data.L = mpc.L;
data.d = permute(mpc.d, [1 3 2]);
data.x0 = simin.x0;

sz = [data.N; data.nx; data.nu; data.nc];

% create initial conditions
x = FBstabMpc.Variable(sz);

solver = FBstabMpc(sz);

opts = solver.DefaultOptions;

tic
[out,xopt] = solver.Solve(data,x);
toc
disp(out)
disp(xopt.z(1))

opts.display_level = 2;
solver.UpdateOptions(opts);
tic
[out,xopt] = solver.Solve(data,x);
toc
disp(out)
disp(xopt.z(1))

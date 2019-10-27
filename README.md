# FBstab C++
This repository contains an C++ implementation of FBstab: The proximally regularized Fischer-Burmeister method for convex quadratic programming. FBstab solves quadratic programs of the form,

```
min.  0.5 z'Hz + f'z
s.t.      Gz = h
          Az <= b
```

and its dual

```
min.  0.5 u'Hu + b'l + h'v
s.t.  Hu + f + G'l + A'v = 0
      v >= 0.
```

It assumes that H is positive semidefinite and can detect primal and/or dual infeasibility. FBstab requires no regularity assumptions on the G or A matrices. A mathematical description of FBstab can be found in the following [research article](https://arxiv.org/pdf/1901.04046.pdf).

FBstab was originally designed for solving Model Predictive Control (MPC) problems, it can exploit sparsity and be easily warmstarted. However, the algorithm is applicable to general QPs and the core algorithm is written so as to be easily extensible. We currently provide specializations to dense inequality constrained QPs and linear-quadratic optimal control problems, i.e., MPC problems, and are in the process of writing a version for general sparse QPs. FBstab uses [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) for both backend linear algebra calculations and its interface. We currently provide two versions of FBstab:

`FBstabMpc` solves linear-quadratic optimal control problems of the following form:

```
min.  \sum_{i=0}^N 1/2 [x(i)]' * [Q(i) S(i)'] [x(i)] + [q(i)]'*[x(i)]
                       [u(i)]    [S(i) R(i) ] [u(i)]   [r(i)]  [u(i)]

s.t.  x(i+1) = A(i)*x(i) + B(i) u(i) + c(i), i = 0 ... N-1
      x(0) = x0
      E(i)*x(i) + L(i)*u(i) + d(i) <= 0,     i = 0 ... N
      
Where
      [ Q(i),S(i)']
      [ S(i),R(i) ]
 
is positive semidefinite for all i \in [0,N].
```

`FBstabDense` solves dense inequality constrained QPs of the following form:

```
min.    1/2  z'Hz + f'z
s.t.         Az <= b
```

## Using FBstab
FBstab is written in C++ 11 and uses [Bazel](https://bazel.build/) as its primary build system. FBstab has the following dependencies:

1. [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
2. [googletest](https://github.com/google/googletest) (for testing only)

Once you've cloned the repo and installed bazel you can check if everything is working by nagivating to the project root and running ```bazel test //...``` in your terminal. Make sure you have at least Bazel 1.1.0 installed, this is a common issue.
 
If your project uses Bazel then FBstab's targets can be [made directly available](https://docs.bazel.build/versions/master/external.html). 
Otherwise, we recommend building one of the top level libraries in ```fbstab/BUILD.bazel``` using e.g., the command ```bazel build fbstab_mpc``` and linking with the resulting static libraries in ```bazel-bin/```. Interfaces are exposed in the header files:

- ```fbstab_mpc.h``` 
- ```fbstab_dense.h```

By default, the build system will automatically download appropriate versions of all dependencies. If you wish to use a local copy instead, e.g., if other parts of your project rely on Eigen and you want to enforce consistent versioning, open ```tools/eigen/repository.bzl``` and replace the ```http_archive``` with a ```local_repository``` as described [here](https://docs.bazel.build/versions/master/external.html).

We currently have the following interfaces planned/available:

1. C++: See ``` fbstab/tests/``` for usage examples.
2. MATLAB (In progress)
3. Python (Planned)

FBstab is still under active development so, while the mathematical problem definitions are more or less stable, the interfaces will likely change as the project matures.

FBstab is primarily developed on MacOS using clang but is highly portable, as long as a compliant C++ 11 compiler is available you should be able to build FBstab. Versions of FBstab have been tested on Xenial and Bionic with gcc and clang compilers in the past and we plan to support MacOS, Linux, and Windows. We hope to get CI up and running soon. 

## Citing FBstab
If you find FBstab useful and would like to cite it in your academic publications, we suggest the following BibTeX citation:

```
@article{fbstab,
  author={Liao-McPherson, Dominic and Kolmanovsky, Ilya},
  title={{FB}stab: A Proximally Stabilized Semismooth Algorithm for Convex Quadratic Programming},
  journal={arXiv preprint arXiv:1901.04046},
  year={2019}
}
```

Other FBstab related citations:

```
@inproceedings{fbstab_cdc2019,
  title={The {FBstab} Algorithm for Model Predictive Control: {An} Implicit Condensing Approach},
  author={Liao-McPherson, Dominic and Kolmanovsky, Ilya},
  booktitle={To appear at the 2019 IEEE Conference on Decision and Control (CDC)},
}
```

```
@article{fbrs, 
author={D. {Liao-McPherson} and M. {Huang} and I. {Kolmanovsky}}, 
journal={IEEE Transactions on Automatic Control}, 
title={A Regularized and Smoothed Fischer–Burmeister Method for Quadratic Programming With Applications to Model Predictive Control}, 
year={2019}, 
volume={64}, 
number={7}, 
pages={2937-2944}, 
doi={10.1109/TAC.2018.2872201}, 
ISSN={0018-9286}, 
month={July},}
```

## Acknowledgements
The FBstab algorithm was originally developed with support from the National Science Foundation through the award [CMMI 1562209](https://www.nsf.gov/awardsearch/showAward?AWD_ID=1562209). This C++ implementation was developed jointly by the Toyota Research Institute (TRI) and by the University of Michigan (UM) through a TRI/UM collaborative [project](https://bec.umich.edu/um-tri/semi-smooth-and-variational-methods-for-real-time-dynamic-optimization/).








# MPC interface
matlab_add_mex(NAME mex_fbstab_mpc SRC mex_fbstab_mpc.cc ${FBSTAB_MATLAB_ROOT}/mex_fbstab_base.cc R2018a)
target_compile_features(mex_fbstab_mpc PUBLIC cxx_std_11)
target_include_directories(mex_fbstab_mpc PRIVATE ${FBSTAB_ROOT})
target_include_directories(mex_fbstab_mpc PRIVATE ${FBSTAB_MATLAB_ROOT})
target_include_directories(mex_fbstab_mpc PRIVATE ${FBSTAB_MEXUTILS}/include)
target_link_directories(mex_fbstab_mpc PRIVATE ${FBSTAB_MEXUTILS})
target_link_libraries(mex_fbstab_mpc fbstab_mpc)

install(DIRECTORY . DESTINATION . FILES_MATCHING PATTERN "*.m")
install(TARGETS mex_fbstab_mpc RUNTIME DESTINATION .)
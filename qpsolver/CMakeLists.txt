# Here we provide 3 libraries
#
# * mpc-walkgen_qpsolver is a header-only library which defines an
#   interface template (ie. pure virtual class template)
#   QPSolver<Scalar> for QP solvers.
#   And also factory template for such solvers.
#
# * mpc-walkgen_qpsolver_qpoases_{float,double} are two libraries which provide
#   (non-templated) factories returning QPSolver<Scalar>* based on
#   qpOASES compiled for float and double types, respectively.

# We *need* to hide symbols in order to avoid qpOASES symbols clashes
qi_sanitize_compile_flags(HIDDEN_SYMBOLS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

qi_stage_header_only_lib(mpc-walkgen_qpsolver
  INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  DEPENDS EIGEN3)

set(_mpc-walkgen_qpsolver_headers
  mpc-walkgen/qpsolver.h)
qi_install_header(${_mpc-walkgen_qpsolver_headers} KEEP_RELATIVE_PATHS)


set(_qpoases_name_double "qpOASES")
set(_qpoases_name_float "qpOASESfloat")

set(_qpoases_flags_double "")
set(_qpoases_flags_float "-DQPOASES_REAL_IS_FLOAT")

set(_qpoases_is_shared_double FALSE)
set(_qpoases_is_shared_float TRUE)

foreach(_scalar "double" "float")
  set(_mpc-walkgen_qpsolver_qpoases_${_scalar}_headers
    mpc-walkgen/qpsolver_qpoases_${_scalar}.h)

  qi_install_header(${_mpc-walkgen_qpsolver_qpoases_${_scalar}_headers}
    KEEP_RELATIVE_PATHS)

  qi_create_lib(mpc-walkgen_qpsolver_qpoases_${_scalar} SHARED
    ${_mpc-walkgen_qpsolver_qpoases_${_scalar}_headers}
    src/qpsolver_qpoases_${_scalar}.cpp
    src/qpsolver_qpoases.hxx
    ${_mpc-walkgen_qpsolver_headers})

  if(_qpoases_flags_${_scalar})
    set_target_properties(mpc-walkgen_qpsolver_qpoases_${_scalar}
      PROPERTIES COMPILE_FLAGS "${_qpoases_flags_${_scalar}}")
  endif()
  # mpc-walkgen_qpsolver_qpoases_${_scalar} is a shared lib which depends on
  # the lib ${_qpoases_name_${_scalar}} which is sometimes shared, sometimes
  # static.
  # People using mpc-walkgen_qpsolver_qpoases_${_scalar} should not link
  # with ${_qpoases_name_${_scalar}} when it is static.
  # Since qi_stage_lib exports all the dependencies by default we need to
  # explicitly list the "public" dependencies.
  set(_public_deps qi mpc-walkgen_qpsolver)
  set(_private_deps)
  if(_qpoases_is_shared_${_scalar})
    list(APPEND _public_deps ${_qpoases_name_${_scalar}})
  else()
    list(APPEND _private_deps ${_qpoases_name_${_scalar}})
  endif()
  qi_use_lib(mpc-walkgen_qpsolver_qpoases_${_scalar}
    ${_public_deps}
    ${_private_deps})
  # qi_stage_lib(... DEPENDS ...) expects a flat the list of all the public
  # dependencies, so let find the dependencies of our dependencies...
  set(_full_public_deps)
  foreach(_public_dep ${_public_deps})
    string(TOUPPER ${_public_dep} _public_dep_upper)
    list(APPEND _full_public_deps ${_public_dep_upper})
    list(APPEND _full_public_deps ${${_public_dep_upper}_DEPENDS})
  endforeach()
  qi_stage_lib(mpc-walkgen_qpsolver_qpoases_${_scalar}
    DEPENDS ${_full_public_deps})
endforeach()

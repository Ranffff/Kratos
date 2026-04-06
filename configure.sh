export CXX=clang++
export CC=clang

export KRATOS_SOURCE="$(pwd)"
export KRATOS_BUILD="${KRATOS_SOURCE}/build"
export KRATOS_APP_DIR="${KRATOS_SOURCE}/applications"
export KRATOS_BUILD_TYPE="Release"
export PYTHON_EXECUTABLE="/usr/bin/python3"
export BOOST_ROOT="$HOME/dev/boost_1_85_0"

export KRATOS_APPLICATIONS=""
add_app() {
    export KRATOS_APPLICATIONS="${KRATOS_APPLICATIONS};$1"
}

add_app ${KRATOS_APP_DIR}/LinearSolversApplication
add_app ${KRATOS_APP_DIR}/ConstitutiveLawsApplication
add_app ${KRATOS_APP_DIR}/StructuralMechanicsApplication
add_app ${KRATOS_APP_DIR}/FluidDynamicsApplication
add_app ${KRATOS_APP_DIR}/MappingApplication
add_app ${KRATOS_APP_DIR}/MeshMovingApplication
add_app ${KRATOS_APP_DIR}/CoSimulationApplication
add_app ${KRATOS_APP_DIR}/IgaApplication

if [ ! -d "${KRATOS_BUILD}" ]; then
    mkdir -p "${KRATOS_BUILD}"
fi

cd "${KRATOS_BUILD}"

cmake "${KRATOS_SOURCE}"                                 \
-DCMAKE_BUILD_TYPE="${KRATOS_BUILD_TYPE}"                \
-DCMAKE_CXX_STANDARD=17                                  \
-DCMAKE_CXX_STANDARD_REQUIRED=ON                         \
-DKRATOS_PYTHON_INTERFACE=ON                             \
-DBOOST_ROOT="${BOOST_ROOT}"                             \
-DPYTHON_EXECUTABLE="${PYTHON_EXECUTABLE}"               \
-DKRATOS_APPLICATIONS="${KRATOS_APPLICATIONS}"           \
-DUSE_MPI=OFF

cmake --build . --target install -j4

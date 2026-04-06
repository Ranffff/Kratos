#!/bin/bash

# 1. Function to add applications
add_app () {
    export KRATOS_APPLICATIONS="${KRATOS_APPLICATIONS}$1;"
}

# 2. Set compiler and basic variables
export CC=gcc
export CXX=g++
export KRATOS_SOURCE="$(pwd)"
export KRATOS_BUILD="${KRATOS_SOURCE}/build"
export KRATOS_APP_DIR="${KRATOS_SOURCE}/applications"
export KRATOS_BUILD_TYPE="Release"
export PYTHON_EXECUTABLE="/usr/bin/python3"

# 3. CRITICAL: Point to the manually downloaded Boost
# This avoids the broken system boost in /usr/include
export BOOST_ROOT="$HOME/dev/boost_1_85_0"

# 4. Set applications to compile (Required for your RBF Thesis)
export KRATOS_APPLICATIONS=
add_app ${KRATOS_APP_DIR}/LinearSolversApplication
add_app ${KRATOS_APP_DIR}/StructuralMechanicsApplication
add_app ${KRATOS_APP_DIR}/MappingApplication
add_app ${KRATOS_APP_DIR}/MeshingApplication

# 5. Clean previous build artifacts
rm -rf "${KRATOS_BUILD}"
mkdir -p "${KRATOS_BUILD}"
cd "${KRATOS_BUILD}"

# 6. Configure with CMake
# -DBoost_NO_SYSTEM_PATHS=ON is the key to bypass old boost errors
cmake .. \
-DCMAKE_CXX_STANDARD=17 \
-DCMAKE_CXX_STANDARD_REQUIRED=ON \
-DCMAKE_BUILD_TYPE=${KRATOS_BUILD_TYPE} \
-DKRATOS_PYTHON_INTERFACE=ON \
-DKRATOS_BUILD_APPLICATIONS=ON \
-DBOOST_ROOT="${BOOST_ROOT}" \
-DBoost_NO_SYSTEM_PATHS=ON \
-DUSE_MPI=OFF

# 7. Build with limited cores to prevent freezing
cmake --build . --target install -- -j4

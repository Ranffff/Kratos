//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
// Main authors:    
// Contributor:     
//
// See PhD Thesis Tianyang Wang Chapter 5
// Collaborator:     Based on the theory by Ahrem, Beckert, and Wendland (2007)

#pragma once

// System includes

// External includes
#include "utilities/math_utils.h"
#include "geometries/line_3d_2.h"
// Project includes
#include "rbf_beam_mapper.h"
#include "mapping_application_variables.h"
#include "custom_utilities/mapping_matrix_utilities.h"
#include "mappers/mapper_define.h"

namespace Kratos
{

template<class TSparseSpace, class TDenseSpace>
void RBFBeamMapper<TSparseSpace, TDenseSpace>::UpdateInterface(
    Kratos::Flags MappingOptions, 
    double SearchRadius)
{
    if (MappingOptions.Is(MapperFlags::REMESHED)) {
        this->Initialize();
    }

    BuildRBFSystemMatrix();
}

template<class TSparseSpace, class TDenseSpace>
void RBFBeamMapper<TSparseSpace, TDenseSpace>::Map(
    const Variable<array_1d<double, 3>>& rOriginVariableDisp,
    const Variable<array_1d<double, 3>>& rOriginVariableRot,
    const Variable<array_1d<double, 3>>& rDestinationVariable,
    Kratos::Flags MappingOptions)
{
    const SizeType num_origin_nodes = this->mrModelPartOrigin.NumberOfNodes();
    const SizeType system_size = 6 * num_origin_nodes + 4 * 3; 

    VectorType rhs_vector = ZeroVector(system_size);
    
    // 1. Assemble RHS vector with displacements and rotations
    // Loop over mrModelPartOrigin nodes to fill rhs_vector

    // 2. Solve the linear system
    // TDenseSpace::Solve(mpSystemMatrix, mRBFCoefficients, rhs_vector);

    // 3. Interpolate at destination nodes
    for (auto& r_node : this->mrModelPartDestination.Nodes()) {
        array_1d<double, 3> interpolated_disp = ZeroVector(3);
        
        // Evaluate RBF sum for the current destination node
        
        r_node.FastGetSolutionStepValue(rDestinationVariable) = interpolated_disp;
    }
}

template<class TSparseSpace, class TDenseSpace>
void RBFBeamMapper<TSparseSpace, TDenseSpace>::InverseMap(
    const Variable<array_1d<double, 3>>& rOriginVariableForce,
    const Variable<array_1d<double, 3>>& rOriginVariableMoment,
    const Variable<array_1d<double, 3>>& rDestinationVariable,
    Kratos::Flags MappingOptions)
{
    // Apply conservative mapping using the transpose of the interpolation operators
}

template<class TSparseSpace, class TDenseSpace>
void RBFBeamMapper<TSparseSpace, TDenseSpace>::BuildRBFSystemMatrix()
{
    const SizeType num_origin_nodes = this->mrModelPartOrigin.NumberOfNodes();
    const SizeType system_size = 6 * num_origin_nodes + 4 * 3; 

    // Resize and initialize system matrix
    // MatrixType A_matrix = ZeroMatrix(system_size, system_size);

    // Block assembly: A, P, P^T, 0
    for (SizeType i = 0; i < num_origin_nodes; ++i) {
        for (SizeType j = 0; j < num_origin_nodes; ++j) {
            // Compute distance between node i and node j
            // Evaluate kernel and its derivatives
            // Fill translation-translation block
            // Fill translation-rotation block
            // Fill rotation-translation block
            // Fill rotation-rotation block
        }
    }

    // Add polynomial terms P
}

template<class TSparseSpace, class TDenseSpace>
double RBFBeamMapper<TSparseSpace, TDenseSpace>::EvaluateKernel(const double Distance) const
{
    // Placeholder for kernel evaluation (e.g., Gaussian, Wendland)
    return 0.0;
}

// Template instantiation
template class RBFBeamMapper<MapperDefinitions::SparseSpaceType, MapperDefinitions::DenseSpaceType>;

}  // namespace Kratos.
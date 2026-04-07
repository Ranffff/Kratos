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

RBFBeamMapperInterfaceInfo::RBFBeamMapperInterfaceInfo(const double SupportRadius)
    : MapperInterfaceInfo(), 
      mSupportRadius(SupportRadius)
{
    // Initialize base class
    // Pre-allocate memory for mSupportNodes to improve performance (e.g., reserve 15-20 nodes)
}

RBFBeamMapperInterfaceInfo::RBFBeamMapperInterfaceInfo(const CoordinatesArrayType& rCoordinates,
                                                       const IndexType SourceLocalSystemIndex,
                                                       const IndexType DestinationLocalSystemIndex,
                                                       const double SupportRadius)
    : MapperInterfaceInfo(rCoordinates, SourceLocalSystemIndex, DestinationLocalSystemIndex),
      mSupportRadius(SupportRadius)
{
    // Initialize base class with local coordinates and system indices
    // Pre-allocate memory for mSupportNodes
}

MapperInterfaceInfo::Pointer RBFBeamMapperInterfaceInfo::Create() const 
{
    // Create and return a Kratos::make_shared pointer of RBFBeamMapperInterfaceInfo
    // Pass mSupportRadius to the new instance
    return Kratos::make_shared<RBFBeamMapperInterfaceInfo>(mSupportRadius);
}

MapperInterfaceInfo::Pointer RBFBeamMapperInterfaceInfo::Create(const CoordinatesArrayType& rCoordinates,
                                                                const IndexType SourceLocalSystemIndex,
                                                                const IndexType DestinationLocalSystemIndex) const 
{
    // Create and return a Kratos::make_shared pointer of RBFBeamMapperInterfaceInfo
    // Pass the coordinates, indices, and mSupportRadius to the new instance
    return Kratos::make_shared<RBFBeamMapperInterfaceInfo>(rCoordinates, SourceLocalSystemIndex, DestinationLocalSystemIndex, mSupportRadius);
}

void RBFBeamMapperInterfaceInfo::ProcessSearchResult(const InterfaceObject& rInterfaceObject)
{
    // Call the base class method to save the raw search result
    // e.g., SaveSearchResult(rInterfaceObject, false);
}

void RBFBeamMapperInterfaceInfo::ProcessSearchResultForApproximation(const InterfaceObject& rInterfaceObject) 
{
    // 1. Get the base geometry from rInterfaceObject
    // 2. Iterate through all nodes of this geometry
    // 3. For each node, compute the Euclidean distance to the target point ( this->Coordinates() )
    // 4. If distance <= mSupportRadius:
    //      a. Check if this node ID is already stored in mSupportNodes to avoid duplicates
    //      b. If it is new, emplace it into mSupportNodes
    // 5. Sort mSupportNodes by distance (closest first) using the overloaded operator<
    // 6. If mSupportNodes is not empty, call this->SetLocalSearchWasSuccessful()
}

std::vector<RBFBeamMapperInterfaceInfo::SupportNodeData>& RBFBeamMapperInterfaceInfo::GetSupportNodes() const
{
    // Return the internal mSupportNodes vector
    return mSupportNodes;
}

double RBFBeamMapperInterfaceInfo::GetSupportRadius() const 
{
    // Return the internal support radius
    return mSupportRadius;
}

} // namespace Kratos

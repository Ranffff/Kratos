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

// Project includes
#include "mappers/mapper.h"
#include "custom_searching/interface_communicator.h"
#include "custom_utilities/mapper_local_system.h"
#include "custom_utilities/radial_basis_functions_utilities.h"
#include "custom_utilities/interface_vector_container.h"

#include "custom_utilities/projection_utilities.h"
#include "utilities/geometrical_projection_utilities.h"


namespace Kratos
{

///@name Kratos Classes
///@{

class KRATOS_API(MAPPING_APPLICATION) RBFBeamMapperInterfaceInfo : public MapperInterfaceInfo
{
public:
    KRATOS_CLASS_POINTER_DEFINITION(RBFBeamMapperInterfaceInfo);

    struct SupportNodeData {
        const Node* pNode;
        double Distance;
        array_1d<double, 3> Coordinates;

        SupportNodeData(const Node* p_node, double dist, const array_1d<double, 3>& coords)
            : pNode(p_node), Distance(dist), Coordinates(coords) {}

        bool operator<(const SupportNodeData& rOther) const {
            return Distance < rOther.Distance;
        }
    };

    explicit RBFBeamMapperInterfaceInfo(const double SupportRadius = 1.0);

    explicit RBFBeamMapperInterfaceInfo(const CoordinatesArrayType& rCoordinates,
                                        const IndexType SourceLocalSystemIndex,
                                        const IndexType DestinationLocalSystemIndex,
                                        const double SupportRadius = 1.0);

    ~RBFBeamMapperInterfaceInfo() override = default;

    MapperInterfaceInfo::Pointer Create() const override;

    MapperInterfaceInfo::Pointer Create(const CoordinatesArrayType& rCoordinates,
                                        const IndexType SourceLocalSystemIndex,
                                        const IndexType DestinationLocalSystemIndex) const override;

    void ProcessSearchResult(const InterfaceObject& rInterfaceObject) override;

    void ProcessSearchResultForApproximation(const InterfaceObject& rInterfaceObject) override;

    const std::vector<SupportNodeData>& GetSupportNodes() const;

    double GetSupportRadius() const;

private:
    double mSupportRadius;
    std::vector<SupportNodeData> mSupportNodes;

}; 



template<class TSparseSpace, class TDenseSpace>
class KRATOS_API(MAPPING_APPLICATION) RBFBeamMapper : public Mapper<TSparseSpace, TDenseSpace>
{
public:
    KRATOS_CLASS_POINTER_DEFINITION(RBFBeamMapper);

    using BaseType = Mapper<TSparseSpace, TDenseSpace>;
    using MapperUniquePointerType = typename BaseType::MapperUniquePointerType;
    using MatrixType = typename TDenseSpace::MatrixType;
    using VectorType = typename TDenseSpace::VectorType;

    RBFBeamMapper(ModelPart& rModelPartOrigin,
                  ModelPart& rModelPartDestination,
                  Parameters JsonParameters);

    ~RBFBeamMapper() override = default;

    MapperUniquePointerType Clone(ModelPart& rModelPartOrigin,
                                  ModelPart& rModelPartDestination,
                                  Parameters JsonParameters) const override;

    void UpdateInterface(Kratos::Flags MappingOptions, double SearchRadius) override;

    // Standard Mapping Interface (Overridden from Base Class)
    void Map(const Variable<double>& rOriginVariable,
             const Variable<double>& rDestinationVariable,
             Kratos::Flags MappingOptions) override;

    void Map(const Variable<array_1d<double, 3>>& rOriginVariable,
             const Variable<array_1d<double, 3>>& rDestinationVariable,
             Kratos::Flags MappingOptions) override;

    void InverseMap(const Variable<double>& rDestinationVariable,
                    const Variable<double>& rOriginVariable,
                    Kratos::Flags MappingOptions) override;

    void InverseMap(const Variable<array_1d<double, 3>>& rDestinationVariable,
                    const Variable<array_1d<double, 3>>& rOriginVariable,
                    Kratos::Flags MappingOptions) override;

    // Custom Mapping Interface for Aeroelasticity (Displacement + Rotation Coupling)
    void MapWithRotations(const Variable<array_1d<double, 3>>& rOriginVariableDisp,
                          const Variable<array_1d<double, 3>>& rOriginVariableRot,
                          const Variable<array_1d<double, 3>>& rDestinationVariableDisp,
                          Kratos::Flags MappingOptions);

    void InverseMapWithMoments(const Variable<array_1d<double, 3>>& rDestinationVariableForce,
                               const Variable<array_1d<double, 3>>& rOriginVariableForce,
                               const Variable<array_1d<double, 3>>& rOriginVariableMoment,
                               Kratos::Flags MappingOptions);

    // Input and Output
    std::string Info() const override;
    void PrintInfo(std::ostream& rOStream) const override;
    void PrintData(std::ostream& rOStream) const override;

private:
    ModelPart& mrModelPartOrigin;
    ModelPart& mrModelPartDestination;
    Parameters mMapperSettings;

    MatrixType mRBFCoefficients;
    double mSupportRadius;
    
    typename TSparseSpace::MatrixPointerType mpSystemMatrix;

    void Initialize();
    void BuildRBFSystemMatrix();
    double EvaluateKernel(const double Distance) const;

}; // Class RBFBeamMapper

///@} 

}  // namespace Kratos
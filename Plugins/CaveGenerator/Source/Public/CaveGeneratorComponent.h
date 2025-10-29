// Copyright [Your Name] & Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/EngineTypes.h"
#include "CaveGeneratorComponent.generated.h"

// Forward declarations for Unreal Engine types. This improves compile times by avoiding full header includes.
class USplineComponent;
class UProceduralMeshComponent;
class UInstancedStaticMeshComponent;
class UStaticMesh;
// Forward declaration for our own struct, defined in the .cpp file.
struct FCompleteMeshData;

/**
 * @enum ECaveRoomShape
 * Defines the available procedural shapes for rooms.
 */
UENUM(BlueprintType)
enum class ECaveRoomShape : uint8
{
    Circular UMETA(DisplayName = "Circular Room"),
    Rectangular UMETA(DisplayName = "Rectangular Room")
};

/**
 * @struct FCaveRoomMarker
 * A struct that defines the properties of a single room to be generated along the spline.
 */
USTRUCT(BlueprintType)
struct FCaveRoomMarker
{
    GENERATED_BODY()

    /** The normalized position along the spline (0.0 = start, 1.0 = end) where the room's center should be. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Position = 0.5f;

    /** The procedural shape of the room. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    ECaveRoomShape Shape = ECaveRoomShape::Circular;

    /**
     * The dimensions of the room. Interpretation depends on the shape:
     * - Circular: X=Radius, Y=Length, Z=Height.
     * - Rectangular: X=Width, Y=Length, Z=Height.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker", meta = (DisplayName = "Dimensions (X, Y, Z)"))
    FVector Size = FVector(1000.0f, 1000.0f, 500.0f);

    /** Additional rotation applied to the room around the spline's axis. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    FRotator Rotation = FRotator::ZeroRotator;
};


/**
 * @class UCaveGeneratorComponent
 * The main component responsible for generating a procedural cave system based on a spline.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAVEGENERATOR_API UCaveGeneratorComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UCaveGeneratorComponent();

    // --- SETUP ---
    /** The Spline Component that defines the main path of the cave. This must be set. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cave Generation|Setup")
    USplineComponent* PathSpline;

    // --- TUNNELS ---
    /** A float curve that controls the tunnel's radius along its length. X-axis is normalized distance (0-1), Y-axis is radius in cm. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels")
    UCurveFloat* TunnelRadiusCurve;
    /** The number of sides for the tunnel mesh. Higher values result in a smoother, more circular tunnel. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels", meta = (ClampMin = "3", ClampMax = "64"))
    int32 TunnelSides = 12;
    /** The strength of the noise deformation applied to the cave surface. 0 means no deformation. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels", meta = (ClampMin = "0.0"))
    float RuggednessAmount = 50.0f;
    /** The scale of the noise pattern. Smaller values create finer, sharper details; larger values create smoother, rolling hills. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels", meta = (ClampMin = "0.01"))
    float RuggednessScale = 100.0f;

    // --- ROOMS ---
    /** An array of markers that define the rooms to be generated along the cave path. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Rooms")
    TArray<FCaveRoomMarker> RoomMarkers;

    // --- VISUALS ---
    /** The material to apply to the generated cave mesh. Should be a triplanar material for best results. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Visuals")
    UMaterialInterface* TunnelMaterial;

    // --- ASSETS ---
    /** The Content Browser folder containing the Static Meshes to be procedurally placed. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets", meta = (ContentDir))
    FDirectoryPath AssetFolder;
    /** Approximate number of assets to spawn per square meter on the cave floor. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets", meta = (ClampMin = "0.0"))
    float AssetDensity = 0.01f;
    /** The random scale range applied to each spawned asset. X=Min, Y=Max. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets")
    FVector2D ScaleRange = FVector2D(0.8f, 1.5f);
    /** Vertical offset applied to each asset, useful for partially burying objects in the ground. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets")
    float ZOffset = -10.0f;
    /** The seed for the random number generator, allowing for reproducible asset layouts. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets")
    int32 RandomSeed = 1234;

    /** The main function that generates the entire cave system. Can be called from Blueprints or via the editor button. */
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "Cave Generation|Actions", meta=(DisplayName="Generate Cave"))
    void Generate();

#if WITH_EDITOR
    /** Automatically regenerates the cave when a property is changed in the editor. */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    /** Creates a ring of vertices at a specific distance along the spline. */
    TArray<FVector> CreateVertexRing(float Distance, int32 Sides, float RadiusOverride = -1.f, const FTransform* InTransform = nullptr);
    /** Generates a mesh section for a tunnel between two distances. */
    void GenerateTunnelSection(FCompleteMeshData& MeshData, TArray<FVector>& InOutRing, float StartDistance, float EndDistance);
    /** Generates a mesh section for a room. */
    void GenerateRoom(FCompleteMeshData& MeshData, const FCaveRoomMarker& Marker, const TArray<FVector>& EntryRing, TArray<FVector>& OutExitRing);
    /** Scans the asset folder and populates the instanced mesh components. */
    void PlaceAssets(const FCompleteMeshData& CaveMeshData, const TArray<UStaticMesh*>& Assets);

    /** The component that renders the procedurally generated cave mesh. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UProceduralMeshComponent* ProceduralMesh;

    /** A map holding the Instanced Static Mesh Components used for asset scattering, keyed by the Static Mesh they represent. */
    UPROPERTY(Transient)
    TMap<UStaticMesh*, UInstancedStaticMeshComponent*> InstancedMeshComponents;
};

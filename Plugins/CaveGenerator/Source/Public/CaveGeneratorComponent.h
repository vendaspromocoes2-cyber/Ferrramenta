// Copyright [Your Name]

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/EngineTypes.h"
#include "CaveGeneratorComponent.generated.h"

// Forward declarations
class USplineComponent;
class UProceduralMeshComponent;
class UInstancedStaticMeshComponent;
class UStaticMesh;
struct FCompleteMeshData; // Forward declare a struct

// ... (Enum ECaveRoomShape e Struct FCaveRoomMarker permanecem os mesmos) ...
UENUM(BlueprintType)
enum class ECaveRoomShape : uint8
{
    Circular,
    Rectangular
};

USTRUCT(BlueprintType)
struct FCaveRoomMarker
{
    GENERATED_BODY()
    // ... (propriedades da struct permanecem as mesmas) ...
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Position = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    ECaveRoomShape Shape = ECaveRoomShape::Circular;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    FVector Size = FVector(1000.0f, 1000.0f, 500.0f);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    FRotator Rotation = FRotator::ZeroRotator;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAVEGENERATOR_API UCaveGeneratorComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UCaveGeneratorComponent();

    // --- PROPRIEDADES DE GERAÇÃO ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cave Generation|Setup")
    USplineComponent* PathSpline;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels")
    UCurveFloat* TunnelRadiusCurve;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels", meta = (ClampMin = "3", ClampMax = "64"))
    int32 TunnelSides = 12;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels", meta = (ClampMin = "0.0"))
    float RuggednessAmount = 50.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Tunnels", meta = (ClampMin = "0.01"))
    float RuggednessScale = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Rooms")
    TArray<FCaveRoomMarker> RoomMarkers;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Visuals")
    UMaterialInterface* TunnelMaterial;

    // --- PROPRIEDADES DE ASSETS ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets", meta = (ContentDir))
    FDirectoryPath AssetFolder;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets", meta = (ClampMin = "0.0"))
    float AssetDensity = 0.01f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets")
    FVector2D ScaleRange = FVector2D(0.8f, 1.5f);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets")
    float ZOffset = -10.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Assets")
    int32 RandomSeed = 1234;

    UFUNCTION(BlueprintCallable, Category = "Cave Generation|Actions")
    void Generate();

private:
    // Helper functions
    TArray<FVector> CreateVertexRing(float Distance, int32 Sides, float RadiusOverride = -1.f, const FTransform* InTransform = nullptr);
    void GenerateTunnelSection(FCompleteMeshData& MeshData, TArray<FVector>& InOutRing, float StartDistance, float EndDistance);
    void GenerateRoom(FCompleteMeshData& MeshData, const FCaveRoomMarker& Marker, const TArray<FVector>& EntryRing, TArray<FVector>& OutExitRing);

    // *** CORREÇÃO: Assinatura da função corrigida para corresponder ao .cpp ***
    void PlaceAssets(const FCompleteMeshData& CaveMeshData, const TArray<UStaticMesh*>& Assets);

    UPROPERTY()
    UProceduralMeshComponent* ProceduralMesh;

    UPROPERTY()
    TMap<UStaticMesh*, UInstancedStaticMeshComponent*> InstancedMeshComponents;
};

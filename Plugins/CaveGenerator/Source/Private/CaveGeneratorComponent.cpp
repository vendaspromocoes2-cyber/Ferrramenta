// Copyright [Your Name] & Epic Games, Inc. All Rights Reserved.

#include "CaveGeneratorComponent.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetMathLibrary.h" // Only used for FRandomStream

DEFINE_LOG_CATEGORY_STATIC(LogCaveGenerator, Log, All);

/**
 * @struct FCompleteMeshData
 * Helper struct to hold all vertex data for a mesh section before passing it to the ProceduralMeshComponent.
 */
struct FCompleteMeshData
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs_Channel0;
    TArray<FVector2D> UVs_Channel1;
};

// --- CORE UTILITY FUNCTIONS ---

/** Recalculates normals for a given set of vertices and triangles for smooth shading. */
void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals)
{
	// Implementation...
}

/** Connects two rings of vertices with a strip of triangles. */
void StitchVertexRings(FCompleteMeshData& MeshData, const TArray<FVector>& RingA, const TArray<FVector>& RingB)
{
	// Implementation...
}

/** Scans a folder in the Content Browser and returns all Static Mesh assets found. */
TArray<UStaticMesh*> LoadAssetsFromFolder(const FString& FolderPath)
{
	// Implementation...
}

// --- COMPONENT IMPLEMENTATION ---

UCaveGeneratorComponent::UCaveGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PathSpline = nullptr; TunnelMaterial = nullptr; TunnelRadiusCurve = nullptr;
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->SetupAttachment(this);
}

void UCaveGeneratorComponent::OnComponentCreated() { Super::OnComponentCreated(); }

#if WITH_EDITOR
void UCaveGeneratorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PathSpline)
    {
        Generate();
    }
    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UCaveGeneratorComponent::Generate()
{
	// 1. --- VALIDATION ---
	if (!PathSpline || !TunnelRadiusCurve || TunnelSides < 3) return;

	const float SplineLength = PathSpline->GetSplineLength();
	if (SplineLength < 1.0f) return;

	// 2. --- INITIALIZATION ---
	ProceduralMesh->ClearAllMeshSections();
    for (auto& Elem : InstancedMeshComponents) {
        if (Elem.Value) {
            Elem.Value->ClearInstances();
            Elem.Value->DestroyComponent();
        }
    }
    InstancedMeshComponents.Empty();

	TArray<UStaticMesh*> SpawnableMeshes = LoadAssetsFromFolder(AssetFolder.Path);
	RoomMarkers.Sort([](const FCaveRoomMarker& A, const FCaveRoomMarker& B) { return A.Position < B.Position; });

	FCompleteMeshData CombinedMeshData;
    TArray<FVector> LastExitRing;
    float CurrentDistanceOnSpline = 0.0f;

    // 3. --- SEQUENTIAL MESH GENERATION ---
    // Create the very first ring at the start of the spline to begin the process.
    LastExitRing = CreateVertexRing(CurrentDistanceOnSpline, TunnelSides);

    // Generate segments (tunnels and rooms) in order.
    for (const FCaveRoomMarker& Room : RoomMarkers)
    {
        float RoomStartDistance = Room.Position * SplineLength;

        GenerateTunnelSection(CombinedMeshData, LastExitRing, CurrentDistanceOnSpline, RoomStartDistance);

        TArray<FVector> RoomExitRing;
        GenerateRoom(CombinedMeshData, Room, LastExitRing, RoomExitRing);

        LastExitRing = RoomExitRing;
        CurrentDistanceOnSpline = RoomStartDistance + Room.Size.Y;
    }
    // Generate the final tunnel section from the last room to the end of the spline.
    GenerateTunnelSection(CombinedMeshData, LastExitRing, CurrentDistanceOnSpline, SplineLength);

	// 4. --- FINALIZATION & ASSET PLACEMENT ---
    // Calculate UVs based on world position for triplanar mapping.
    const float UVScale = 0.01f;
    for(const FVector& Vertex : CombinedMeshData.Vertices) {
        const FVector WorldPos = GetComponentTransform().TransformPosition(Vertex);
        CombinedMeshData.UVs_Channel0.Add(FVector2D(WorldPos.X * UVScale, WorldPos.Z * UVScale));
        CombinedMeshData.UVs_Channel1.Add(FVector2D(WorldPos.X * UVScale, WorldPos.Y * UVScale));
    }

	RecalculateNormals(CombinedMeshData.Vertices, CombinedMeshData.Triangles, CombinedMeshData.Normals);

    TArray<TArray<FVector2D>> AllUVs;
    AllUVs.Add(CombinedMeshData.UVs_Channel0);
    AllUVs.Add(CombinedMeshData.UVs_Channel1);
	ProceduralMesh->CreateMeshSection_LinearColor(0, CombinedMeshData.Vertices, CombinedMeshData.Triangles, CombinedMeshData.Normals, AllUVs, {}, {}, true);

    if (TunnelMaterial) ProceduralMesh->SetMaterial(0, TunnelMaterial);

    if (SpawnableMeshes.Num() > 0)
    {
        PlaceAssets(CombinedMeshData, SpawnableMeshes);
    }
    UE_LOG(LogCaveGenerator, Log, TEXT("Cave generation complete. Vertices: %d, Triangles: %d"), CombinedMeshData.Vertices.Num(), CombinedMeshData.Triangles.Num() / 3);
}

// ... (Implementation of all helper functions: CreateVertexRing, GenerateTunnelSection, GenerateRoom, PlaceAssets, etc.)
// ... (The logic is unchanged, only comments and formatting are improved.)

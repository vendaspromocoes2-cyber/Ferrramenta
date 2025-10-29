// Copyright [Your Name] & Epic Games, Inc. All Rights Reserved.

#include "CaveGeneratorComponent.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogCaveGenerator, Log, All);

// Struct to hold all data for a mesh before passing it to the PMC.
// UVs are removed as they are not needed for the triplanar material approach.
struct FCompleteMeshData
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
};

// --- CORE UTILITY FUNCTIONS ---

/** Recalculates normals for a given set of vertices and triangles for smooth shading. */
void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals)
{
	OutNormals.Init(FVector::ZeroVector, Vertices.Num());
	for (int32 i = 0; i < Triangles.Num(); i += 3) {
		const FVector& V0 = Vertices[Triangles[i]]; const FVector& V1 = Vertices[Triangles[i + 1]]; const FVector& V2 = Vertices[Triangles[i + 2]];
		const FVector FaceNormal = FVector::CrossProduct(V2 - V0, V1 - V0).GetSafeNormal();
		OutNormals[Triangles[i]] += FaceNormal; OutNormals[Triangles[i + 1]] += FaceNormal; OutNormals[Triangles[i + 2]] += FaceNormal;
	}
	for (FVector& Normal : OutNormals) Normal.Normalize();
}

/** Connects two rings of vertices with a strip of triangles. */
void StitchVertexRings(FCompleteMeshData& MeshData, const TArray<FVector>& RingA, const TArray<FVector>& RingB)
{
    if (RingA.Num() != RingB.Num() || RingA.Num() < 2) return;
    const int32 RingSize = RingA.Num();
    const int32 StartIndexA = MeshData.Vertices.Num();
    MeshData.Vertices.Append(RingA);
    const int32 StartIndexB = MeshData.Vertices.Num();
    MeshData.Vertices.Append(RingB);

    for (int32 i = 0; i < RingSize; ++i)
    {
        int32 A0 = StartIndexA + i; int32 A1 = StartIndexA + (i + 1) % RingSize;
        int32 B0 = StartIndexB + i; int32 B1 = StartIndexB + (i + 1) % RingSize;
        MeshData.Triangles.Add(A0); MeshData.Triangles.Add(B1); MeshData.Triangles.Add(A1);
        MeshData.Triangles.Add(A0); MeshData.Triangles.Add(B0); MeshData.Triangles.Add(B1);
    }
}

/** Scans a folder in the Content Browser and returns all Static Mesh assets found. */
TArray<UStaticMesh*> LoadAssetsFromFolder(const FString& FolderPath)
{
    TArray<UStaticMesh*> LoadedMeshes;
    if (FolderPath.IsEmpty()) return LoadedMeshes;
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetData;
    AssetRegistryModule.Get().GetAssetsByPath(FName(*FolderPath), AssetData, true);
    for (const FAssetData& Data : AssetData) {
        if (Data.GetClass() == UStaticMesh::StaticClass()) {
            if (UStaticMesh* Mesh = Cast<UStaticMesh>(Data.GetAsset())) {
                LoadedMeshes.Add(Mesh);
            }
        }
    }
    UE_LOG(LogCaveGenerator, Log, TEXT("Found %d Static Meshes in '%s'."), LoadedMeshes.Num(), *FolderPath);
    return LoadedMeshes;
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
	if (!PathSpline || !TunnelRadiusCurve || TunnelSides < 3) return;

	const float SplineLength = PathSpline->GetSplineLength();
	if (SplineLength < 1.0f) return;

	ProceduralMesh->ClearAllSections();
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
    LastExitRing = CreateVertexRing(CurrentDistanceOnSpline, TunnelSides);

    for (const FCaveRoomMarker& Room : RoomMarkers)
    {
        float RoomStartDistance = Room.Position * SplineLength;
        GenerateTunnelSection(CombinedMeshData, LastExitRing, CurrentDistanceOnSpline, RoomStartDistance);
        TArray<FVector> RoomExitRing;
        GenerateRoom(CombinedMeshData, Room, LastExitRing, RoomExitRing);
        LastExitRing = RoomExitRing;
        CurrentDistanceOnSpline = RoomStartDistance + Room.Size.Y;
    }
    GenerateTunnelSection(CombinedMeshData, LastExitRing, CurrentDistanceOnSpline, SplineLength);

	RecalculateNormals(CombinedMeshData.Vertices, CombinedMeshData.Triangles, CombinedMeshData.Normals);

	ProceduralMesh->CreateMeshSection_LinearColor(0, CombinedMeshData.Vertices, CombinedMeshData.Triangles, CombinedMeshData.Normals, TArray<FVector2D>(), {}, {}, true);

    if (TunnelMaterial) ProceduralMesh->SetMaterial(0, TunnelMaterial);

    if (SpawnableMeshes.Num() > 0)
    {
        PlaceAssets(CombinedMeshData, SpawnableMeshes);
    }
    UE_LOG(LogCaveGenerator, Log, TEXT("Cave generation complete. Vertices: %d, Triangles: %d"), CombinedMeshData.Vertices.Num(), CombinedMeshData.Triangles.Num() / 3);
}

TArray<FVector> UCaveGeneratorComponent::CreateVertexRing(float Distance, int32 Sides, float RadiusOverride, const FTransform* InTransform)
{
    TArray<FVector> RingVertices;
    const float SplineLen = PathSpline->GetSplineLength();
    if (SplineLen <= 0) return RingVertices;

    const float NormalizedDistance = FMath::Clamp(Distance / SplineLen, 0.0f, 1.0f);
    const float Radius = RadiusOverride > 0.f ? RadiusOverride : TunnelRadiusCurve->GetFloatValue(NormalizedDistance);

    FTransform Transform = InTransform ? *InTransform : PathSpline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
    Transform.SetScale3D(FVector::OneVector);

    for (int32 i = 0; i < Sides; ++i)
    {
        float Angle = (360.0f / Sides) * i;
        FVector Dir = FVector(0, 1, 0).RotateAngleAxis(Angle, FVector(1, 0, 0));
        FVector VertexPos = Transform.TransformPosition(Dir * Radius);

        float Noise = FMath::PerlinNoise3D(GetComponentTransform().TransformPosition(VertexPos) / RuggednessScale);
        RingVertices.Add(VertexPos + (Transform.TransformVector(Dir).GetSafeNormal() * Noise * RuggednessAmount));
    }
    return RingVertices;
}

void UCaveGeneratorComponent::GenerateTunnelSection(FCompleteMeshData& MeshData, TArray<FVector>& InOutRing, float StartDistance, float EndDistance)
{
    if (EndDistance <= StartDistance) return;
    const float StepSize = 100.0f;
    TArray<FVector> CurrentRing = InOutRing;

    for (float Dist = StartDistance + StepSize; Dist < EndDistance; Dist += StepSize)
    {
        TArray<FVector> NextRing = CreateVertexRing(Dist, TunnelSides);
        StitchVertexRings(MeshData, CurrentRing, NextRing);
        CurrentRing = NextRing;
    }

    TArray<FVector> FinalRing = CreateVertexRing(EndDistance, TunnelSides);
    StitchVertexRings(MeshData, CurrentRing, FinalRing);
    InOutRing = FinalRing;
}

void UCaveGeneratorComponent::GenerateRoom(FCompleteMeshData& MeshData, const FCaveRoomMarker& Marker, const TArray<FVector>& EntryRing, TArray<FVector>& OutExitRing)
{
    float RoomStartDistance = Marker.Position * PathSpline->GetSplineLength();
    float RoomEndDistance = RoomStartDistance + Marker.Size.Y;

    FTransform EntryTransform = PathSpline->GetTransformAtDistanceAlongSpline(RoomStartDistance, ESplineCoordinateSpace::Local);
    EntryTransform.SetRotation(EntryTransform.GetRotation() * Marker.Rotation.Quaternion());
    FTransform ExitTransform = PathSpline->GetTransformAtDistanceAlongSpline(RoomEndDistance, ESplineCoordinateSpace::Local);
    ExitTransform.SetRotation(ExitTransform.GetRotation() * Marker.Rotation.Quaternion());

    TArray<FVector> RoomEntryRing, RoomExitRing;

    if (Marker.Shape == ECaveRoomShape::Circular)
    {
        RoomEntryRing = CreateVertexRing(0, TunnelSides, Marker.Size.X, &EntryTransform);
        RoomExitRing = CreateVertexRing(0, TunnelSides, Marker.Size.X, &ExitTransform);
        StitchVertexRings(MeshData, RoomEntryRing, RoomExitRing);
    }
    else if (Marker.Shape == ECaveRoomShape::Rectangular)
    {
        const float HalfWidth = Marker.Size.X / 2.0f;
        const float HalfHeight = Marker.Size.Z / 2.0f;

        TArray<FVector> Corners;
        auto AddCorner = [&](const FTransform& T, float X, float Y){
            FVector Pos = T.TransformPosition(FVector(0, X, Y));
            float Noise = FMath::PerlinNoise3D(GetComponentTransform().TransformPosition(Pos) / RuggednessScale);
            FVector Normal = T.TransformVector(FVector(0, X, Y).GetSafeNormal());
            Corners.Add(Pos + Normal * Noise * RuggednessAmount);
        };
        AddCorner(EntryTransform, -HalfWidth, -HalfHeight); AddCorner(EntryTransform, HalfWidth, -HalfHeight);
        AddCorner(EntryTransform, HalfWidth, HalfHeight); AddCorner(EntryTransform, -HalfWidth, HalfHeight);
        AddCorner(ExitTransform, -HalfWidth, -HalfHeight); AddCorner(ExitTransform, HalfWidth, -HalfHeight);
        AddCorner(ExitTransform, HalfWidth, HalfHeight); AddCorner(ExitTransform, -HalfWidth, HalfHeight);

        int32 StartIndex = MeshData.Vertices.Num(); MeshData.Vertices.Append(Corners);
        auto AddQuad = [&](int32 A, int32 B, int32 C, int32 D){
            MeshData.Triangles.Add(StartIndex+A); MeshData.Triangles.Add(StartIndex+C); MeshData.Triangles.Add(StartIndex+B);
            MeshData.Triangles.Add(StartIndex+A); MeshData.Triangles.Add(StartIndex+D); MeshData.Triangles.Add(StartIndex+C);
        };
        AddQuad(0, 4, 7, 3); AddQuad(1, 2, 6, 5); AddQuad(0, 1, 5, 4); AddQuad(3, 7, 6, 2);

        for(int32 i=0; i<TunnelSides; ++i) {
            float Alpha = (float)i / TunnelSides;
            if(Alpha < 0.25f) { RoomEntryRing.Add(FMath::Lerp(Corners[0], Corners[3], Alpha*4)); RoomExitRing.Add(FMath::Lerp(Corners[4], Corners[7], Alpha*4)); }
            else if(Alpha < 0.5f) { RoomEntryRing.Add(FMath::Lerp(Corners[3], Corners[2], (Alpha-0.25f)*4)); RoomExitRing.Add(FMath::Lerp(Corners[7], Corners[6], (Alpha-0.25f)*4)); }
            else if(Alpha < 0.75f) { RoomEntryRing.Add(FMath::Lerp(Corners[2], Corners[1], (Alpha-0.5f)*4)); RoomExitRing.Add(FMath::Lerp(Corners[6], Corners[5], (Alpha-0.5f)*4)); }
            else { RoomEntryRing.Add(FMath::Lerp(Corners[1], Corners[0], (Alpha-0.75f)*4)); RoomExitRing.Add(FMath::Lerp(Corners[5], Corners[4], (Alpha-0.75f)*4)); }
        }
    }

    StitchVertexRings(MeshData, EntryRing, RoomEntryRing);
    OutExitRing = RoomExitRing;
}

void UCaveGeneratorComponent::PlaceAssets(const FCompleteMeshData& CaveMeshData, const TArray<UStaticMesh*>& Assets)
{
    if (Assets.Num() == 0 || AssetDensity <= 0.0f) return;

    FRandomStream RandStream(RandomSeed);

    for (UStaticMesh* Mesh : Assets)
    {
        UInstancedStaticMeshComponent* ISMC = NewObject<UInstancedStaticMeshComponent>(this);
        ISMC->RegisterComponentWithWorld(GetWorld());
        ISMC->SetStaticMesh(Mesh);
        ISMC->SetupAttachment(this);
        InstancedMeshComponents.Add(Mesh, ISMC);
    }

    TArray<int32> FloorTriangleIndices;
    FloorTriangleIndices.Reserve(CaveMeshData.Triangles.Num() / 3);
    for (int32 i = 0; i < CaveMeshData.Triangles.Num(); i += 3)
    {
        const FVector& Normal = CaveMeshData.Normals[CaveMeshData.Triangles[i]];
        if (Normal.Z > 0.5f)
        {
            FloorTriangleIndices.Add(i);
        }
    }

    if (FloorTriangleIndices.Num() == 0) return;

    const float AreaConversionFactor = 1.0f / 10000.0f;

    for (int32 StartIndex : FloorTriangleIndices)
    {
        const FVector& V0 = CaveMeshData.Vertices[CaveMeshData.Triangles[StartIndex]];
        const FVector& V1 = CaveMeshData.Vertices[CaveMeshData.Triangles[StartIndex + 1]];
        const FVector& V2 = CaveMeshData.Vertices[CaveMeshData.Triangles[StartIndex + 2]];

        const float Area = FVector::CrossProduct(V1 - V0, V2 - V0).Size() / 2.0f;
        const float NumAssetsToSpawnFloat = (Area * AreaConversionFactor) * AssetDensity;
        const int32 NumAssetsToSpawn = FMath::FloorToInt(NumAssetsToSpawnFloat + RandStream.GetFraction());

        for (int32 j = 0; j < NumAssetsToSpawn; ++j)
        {
            float B = RandStream.FRand(), C = RandStream.FRand();
            if (B + C > 1.f) { B = 1.f - B; C = 1.f - C; }
            const FVector SpawnPos = ((1.f - B - C) * V0) + (B * V1) + (C * V2);

            UStaticMesh* RandomMesh = Assets[RandStream.RandRange(0, Assets.Num() - 1)];
            const float RandomScale = RandStream.FRandRange(ScaleRange.X, ScaleRange.Y);
            const FRotator RandomRotation = FRotator(0, RandStream.FRandRange(0, 360), 0);

            FTransform InstanceTransform(RandomRotation, SpawnPos + FVector(0, 0, ZOffset), FVector(RandomScale));

            if (UInstancedStaticMeshComponent** ISMC_Ptr = InstancedMeshComponents.Find(RandomMesh))
            {
                (*ISMC_Ptr)->AddInstance(InstanceTransform);
            }
        }
    }
    UE_LOG(LogCaveGenerator, Log, TEXT("Asset placement complete."));
}

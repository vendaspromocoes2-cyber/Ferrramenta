// Copyright [Your Name]

#include "CaveGeneratorComponent.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY_STATIC(LogCaveGenerator, Log, All);

// Forward declaration
void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals);

// Struct to hold temporary mesh data
struct FMeshData
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
};

// --- Funções de Geração de Salas (com lógica de entrada) ---
// Retorna os índices dos vértices da "entrada" para conexão.
TArray<int32> GenerateCircularRoom(const FCaveRoomMarker& Marker, const FTransform& RoomTransform, FMeshData& OutMeshData, int32 TunnelSides)
{
    const float Radius = Marker.Size.X;
    const float Height = Marker.Size.Z;
    const int32 VertexStartIndex = OutMeshData.Vertices.Num();
    TArray<int32> EntranceIndices;

    // Vértices do chão e teto
    for (int32 i = 0; i < TunnelSides; ++i)
    {
        float Angle = (360.0f / TunnelSides) * i;
        FVector Pos = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius, FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius, 0);
        OutMeshData.Vertices.Add(RoomTransform.TransformPosition(Pos)); // Chão
        OutMeshData.Vertices.Add(RoomTransform.TransformPosition(Pos + FVector(0,0,Height))); // Teto

        // Assumimos que a entrada está alinhada com o início/fim do anel
        if(i == 0) EntranceIndices.Add(VertexStartIndex + i*2);
    }
    EntranceIndices.Add(VertexStartIndex + 1); // Vértice do teto para a entrada

    // Adiciona o restante dos vértices da entrada (simulação)
    for(int32 i=1; i<TunnelSides; ++i) EntranceIndices.Add(VertexStartIndex);


    // Triângulos (simplificado, sem chão/teto para focar na conexão)
    for (int32 i = 0; i < TunnelSides; ++i)
    {
        // Pula a face da entrada
        if(i == 0) continue;

        int32 V0_F = VertexStartIndex + i * 2;
        int32 V1_F = VertexStartIndex + ((i + 1) % TunnelSides) * 2;
        int32 V0_C = VertexStartIndex + i * 2 + 1;
        int32 V1_C = VertexStartIndex + ((i + 1) % TunnelSides) * 2 + 1;

        // Parede
        OutMeshData.Triangles.Add(V0_F); OutMeshData.Triangles.Add(V1_F); OutMeshData.Triangles.Add(V0_C);
        OutMeshData.Triangles.Add(V0_C); OutMeshData.Triangles.Add(V1_F); OutMeshData.Triangles.Add(V1_C);
    }
    return EntranceIndices;
}


UCaveGeneratorComponent::UCaveGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PathSpline = nullptr;
	TunnelMaterial = nullptr;
	TunnelRadiusCurve = nullptr;
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProceduralMesh->SetupAttachment(this);
}

void UCaveGeneratorComponent::OnComponentCreated() { Super::OnComponentCreated(); }

void UCaveGeneratorComponent::Generate()
{
	if (!PathSpline || !TunnelRadiusCurve || TunnelSides < 3) return;

	ProceduralMesh->ClearAllMeshSections();

	FMeshData CombinedMeshData;
	TArray<FVector> CombinedNormals;
	TArray<FVector2D> CombinedUVs;

	const float SplineLength = PathSpline->GetSplineLength();
	if (SplineLength < 1.0f) return;

	RoomMarkers.Sort([](const FCaveRoomMarker& A, const FCaveRoomMarker& B) { return A.Position < B.Position; });

	const int32 SplinePointsCount = PathSpline->GetNumberOfSplinePoints();
	if (SplinePointsCount < 2) return;

	int32 CurrentRoomMarkerIndex = 0;
    TArray<int32> PreviousRingIndices;

	for (int32 i = 0; i < SplinePointsCount; ++i)
	{
		const float NormalizedDistance = PathSpline->GetDistanceAlongSplineAtSplinePoint(i) / SplineLength;

        TArray<int32> CurrentRingIndices;
        int32 RingVertexStartIndex = CombinedMeshData.Vertices.Num();
        const FVector SplineLocation = PathSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        const FVector SplineDirection = PathSpline->GetDirectionAtSplinePoint(i, ESplineCoordinateSpace::Local);
        const FVector SplineUpVector = PathSpline->GetUpVectorAtSplinePoint(i, ESplineCoordinateSpace::Local);
        const float CurrentRadius = TunnelRadiusCurve->GetFloatValue(NormalizedDistance);
        for (int32 j = 0; j < TunnelSides; ++j) {
            const float CurrentAngle = (360.0f / TunnelSides) * j;
            FVector RingVertexDirection = SplineUpVector.RotateAngleAxis(CurrentAngle, SplineDirection);
            FVector BaseVertexPosition = SplineLocation + (RingVertexDirection * CurrentRadius);
            float NoiseValue = 0.f;
            if (RuggednessAmount > 0.0f && RuggednessScale > 0.0f) {
                NoiseValue = FMath::PerlinNoise3D(GetComponentTransform().TransformPosition(BaseVertexPosition) / RuggednessScale);
            }
            CombinedMeshData.Vertices.Add(BaseVertexPosition + (RingVertexDirection * NoiseValue * RuggednessAmount));
            CurrentRingIndices.Add(RingVertexStartIndex + j);
        }

        bool bShouldConnectTunnelSegment = true;

        if (CurrentRoomMarkerIndex < RoomMarkers.Num())
        {
            const FCaveRoomMarker& Room = RoomMarkers[CurrentRoomMarkerIndex];
            float RoomZoneHalfWidth = (Room.Size.X * 0.25f) / SplineLength; // Zona de influência

            if (FMath::Abs(NormalizedDistance - Room.Position) < RoomZoneHalfWidth)
            {
                bShouldConnectTunnelSegment = false;
            }

            // Verifica se acabamos de passar da posição da sala
            float PrevNormalizedDistance = (i > 0) ? PathSpline->GetDistanceAlongSplineAtSplinePoint(i - 1) / SplineLength : 0.0f;
            if (PrevNormalizedDistance < Room.Position && NormalizedDistance >= Room.Position)
            {
                // Gera a malha da sala
                FTransform RoomTransform(PathSpline->GetRotationAtDistanceAlongSpline(Room.Position * SplineLength, ESplineCoordinateSpace::Local) + Room.Rotation,
                                         PathSpline->GetLocationAtDistanceAlongSpline(Room.Position * SplineLength, ESplineCoordinateSpace::Local));

                // Conecta o anel ANTERIOR à sala
                for (int32 j = 0; j < TunnelSides; ++j)
                {
                    int32 TunnelV0 = PreviousRingIndices[j];
                    int32 TunnelV1 = PreviousRingIndices[(j + 1) % TunnelSides];
                    // Esta é uma conexão muito simplificada
                    int32 RoomV0 = RingVertexStartIndex + j; // Usa o anel atual como proxy para a entrada da sala
                    int32 RoomV1 = RingVertexStartIndex + (j + 1) % TunnelSides;

                    CombinedMeshData.Triangles.Add(TunnelV0); CombinedMeshData.Triangles.Add(TunnelV1); CombinedMeshData.Triangles.Add(RoomV0);
                    CombinedMeshData.Triangles.Add(RoomV0); CombinedMeshData.Triangles.Add(TunnelV1); CombinedMeshData.Triangles.Add(RoomV1);
                }

                CurrentRoomMarkerIndex++;
            }
        }

        if (i > 0 && bShouldConnectTunnelSegment)
        {
            for (int32 j = 0; j < TunnelSides; ++j)
            {
                CombinedMeshData.Triangles.Add(PreviousRingIndices[j]);
                CombinedMeshData.Triangles.Add(CurrentRingIndices[(j + 1) % TunnelSides]);
                CombinedMeshData.Triangles.Add(CurrentRingIndices[j]);

                CombinedMeshData.Triangles.Add(PreviousRingIndices[j]);
                CombinedMeshData.Triangles.Add(PreviousRingIndices[(j + 1) % TunnelSides]);
                CombinedMeshData.Triangles.Add(CurrentRingIndices[(j + 1) % TunnelSides]);
            }
        }
        PreviousRingIndices = CurrentRingIndices;
	}


	RecalculateNormals(CombinedMeshData.Vertices, CombinedMeshData.Triangles, CombinedNormals);
    CombinedUVs.Init(FVector2D::ZeroVector, CombinedMeshData.Vertices.Num());
	ProceduralMesh->CreateMeshSection_LinearColor(0, CombinedMeshData.Vertices, CombinedMeshData.Triangles, CombinedNormals, CombinedUVs, {}, {}, true);

	if (TunnelMaterial) ProceduralMesh->SetMaterial(0, TunnelMaterial);

	UE_LOG(LogCaveGenerator, Log, TEXT("Malha final gerada com %d vértices e %d triângulos."), CombinedMeshData.Vertices.Num(), CombinedTriangles.Num() / 3);
}

void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals)
{
	OutNormals.Init(FVector::ZeroVector, Vertices.Num());
	for (int32 i = 0; i < Triangles.Num(); i += 3) {
		const FVector& V0 = Vertices[Triangles[i]];
		const FVector& V1 = Vertices[Triangles[i + 1]];
		const FVector& V2 = Vertices[Triangles[i + 2]];
		const FVector FaceNormal = FVector::CrossProduct(V2 - V0, V1 - V0).GetSafeNormal();
		OutNormals[Triangles[i]] += FaceNormal;
		OutNormals[Triangles[i + 1]] += FaceNormal;
		OutNormals[Triangles[i + 2]] += FaceNormal;
	}
	for (FVector& Normal : OutNormals) Normal.Normalize();
}

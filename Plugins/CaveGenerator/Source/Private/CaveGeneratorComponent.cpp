// Copyright [Your Name]

#include "CaveGeneratorComponent.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"

DEFINE_LOG_CATEGORY_STATIC(LogCaveGenerator, Log, All);

// --- NOVA FUNÇÃO HELPER PARA CALCULAR AS NORMAIS ---
// Esta função calcula a normal de cada vértice somando as normais das faces adjacentes.
// Isso produz uma iluminação suave e correta em malhas deformadas.
void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals)
{
	OutNormals.Init(FVector::ZeroVector, Vertices.Num());

	// Itera sobre cada triângulo (face) da malha.
	for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		// Pega os três vértices que compõem o triângulo.
		const FVector& V0 = Vertices[Triangles[i]];
		const FVector& V1 = Vertices[Triangles[i + 1]];
		const FVector& V2 = Vertices[Triangles[i + 2]];

		// Calcula a normal da face usando o produto vetorial (cross product) de duas arestas.
		// O resultado é um vetor perpendicular à superfície do triângulo.
		const FVector FaceNormal = FVector::CrossProduct(V2 - V0, V1 - V0).GetSafeNormal();

		// Adiciona a normal da face a cada um dos três vértices que a compõem.
		OutNormals[Triangles[i]] += FaceNormal;
		OutNormals[Triangles[i + 1]] += FaceNormal;
		OutNormals[Triangles[i + 2]] += FaceNormal;
	}

	// Normaliza todas as normais dos vértices.
	// Como cada vértice agora tem a soma das normais de todas as faces que toca,
	// normalizá-lo resulta em uma média, que dá a direção correta para a iluminação.
	for (FVector& Normal : OutNormals)
	{
		Normal.Normalize();
	}
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

void UCaveGeneratorComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
}

void UCaveGeneratorComponent::Generate()
{
	// --- 1. Validação (sem mudanças) ---
	if (!PathSpline) { UE_LOG(LogCaveGenerator, Error, TEXT("PathSpline não está definido!")); return; }
	if (!TunnelRadiusCurve) { UE_LOG(LogCaveGenerator, Error, TEXT("TunnelRadiusCurve não está definida!")); return; }
	if (TunnelSides < 3) { UE_LOG(LogCaveGenerator, Warning, TEXT("TunnelSides deve ser pelo menos 3.")); return; }

	ProceduralMesh->ClearAllMeshSections();

	// --- 2. Preparação de Dados (sem mudanças) ---
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	const int32 SplinePointsCount = PathSpline->GetNumberOfSplinePoints();
	if (SplinePointsCount < 2) { UE_LOG(LogCaveGenerator, Warning, TEXT("A Spline precisa de pelo menos 2 pontos.")); return; }
	const float SplineLength = PathSpline->GetSplineLength();

	// --- 3. Geração de Vértices (sem mudanças) ---
	for (int32 i = 0; i < SplinePointsCount; ++i)
	{
		const FVector SplineLocation = PathSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector SplineDirection = PathSpline->GetDirectionAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector SplineUpVector = PathSpline->GetUpVectorAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const float NormalizedDistance = PathSpline->GetDistanceAlongSplineAtSplinePoint(i) / SplineLength;
		const float CurrentRadius = TunnelRadiusCurve->GetFloatValue(NormalizedDistance);

		for (int32 j = 0; j < TunnelSides; ++j)
		{
			const float CurrentAngle = (360.0f / TunnelSides) * j;
			FVector RingVertexDirection = SplineUpVector.RotateAngleAxis(CurrentAngle, SplineDirection);
			FVector BaseVertexPosition = SplineLocation + (RingVertexDirection * CurrentRadius);

			float NoiseValue = 0.0f;
			if (RuggednessAmount > 0.0f && RuggednessScale > 0.0f)
			{
				FVector WorldVertexPosition = GetComponentTransform().TransformPosition(BaseVertexPosition);
				NoiseValue = FMath::PerlinNoise3D(WorldVertexPosition / RuggednessScale);
			}

			FVector FinalVertexPosition = BaseVertexPosition + (RingVertexDirection * NoiseValue * RuggednessAmount);
			Vertices.Add(FinalVertexPosition);
			UVs.Add(FVector2D(NormalizedDistance, static_cast<float>(j) / TunnelSides));
		}
	}

	// --- 4. Geração de Triângulos (sem mudanças) ---
	for (int32 i = 0; i < SplinePointsCount - 1; ++i)
	{
		for (int32 j = 0; j < TunnelSides; ++j)
		{
			int32 V0 = (i * TunnelSides) + j;
			int32 V1 = (i * TunnelSides) + (j + 1) % TunnelSides;
			int32 V2 = ((i + 1) * TunnelSides) + j;
			int32 V3 = ((i + 1) * TunnelSides) + (j + 1) % TunnelSides;
			Triangles.Add(V0); Triangles.Add(V2); Triangles.Add(V1);
			Triangles.Add(V1); Triangles.Add(V2); Triangles.Add(V3);
		}
	}

    // --- NOVO PASSO: Recalcular Normais ---
    // Após a geração dos vértices e triângulos, chamamos nossa nova função
    // para calcular as normais corretas com base na geometria final.
    RecalculateNormals(Vertices, Triangles, Normals);

	// --- 5. Criação da Malha ---
	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);

	if (TunnelMaterial)
	{
		ProceduralMesh->SetMaterial(0, TunnelMaterial);
	}

	UE_LOG(LogCaveGenerator, Log, TEXT("Malha do túnel gerada com %d vértices e %d triângulos."), Vertices.Num(), Triangles.Num() / 3);
}

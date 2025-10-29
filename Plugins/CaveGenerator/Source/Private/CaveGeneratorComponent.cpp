// Copyright [Your Name]

#include "CaveGeneratorComponent.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetMathLibrary.h" // Apenas para RandStream

DEFINE_LOG_CATEGORY_STATIC(LogCaveGenerator, Log, All);

// Forward declarations e structs (sem mudanças)...
void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& OutNormals);
struct FCompleteMeshData { /* ... */ };
void StitchVertexRings(FCompleteMeshData& MeshData, const TArray<FVector>& RingA, const TArray<FVector>& RingB);


UCaveGeneratorComponent::UCaveGeneratorComponent()
{
	// ... (Construtor sem mudanças) ...
}

void UCaveGeneratorComponent::OnComponentCreated() { Super::OnComponentCreated(); }

TArray<UStaticMesh*> LoadAssetsFromFolder(const FString& FolderPath)
{
    // ... (Função LoadAssetsFromFolder sem mudanças) ...
}

void UCaveGeneratorComponent::Generate()
{
    // ... (Início da função Generate sem mudanças, até a chamada de PlaceAssets) ...

    if (SpawnableMeshes.Num() > 0)
    {
        PlaceAssets(CombinedMeshData, SpawnableMeshes);
    }
}

// --- LÓGICA DE POSICIONAMENTO CORRIGIDA ---
void UCaveGeneratorComponent::PlaceAssets(const FCompleteMeshData& CaveMeshData, const TArray<UStaticMesh*>& Assets)
{
    if (Assets.Num() == 0 || AssetDensity <= 0.0f) return;

    FRandomStream RandStream(RandomSeed);

    for (auto& Elem : InstancedMeshComponents) {
        if(Elem.Value) Elem.Value->DestroyComponent();
    }
    InstancedMeshComponents.Empty();

    for (UStaticMesh* Mesh : Assets) {
        UInstancedStaticMeshComponent* ISMC = NewObject<UInstancedStaticMeshComponent>(this);
        ISMC->RegisterComponentWithWorld(GetWorld());
        ISMC->SetStaticMesh(Mesh);
        ISMC->SetupAttachment(this);
        InstancedMeshComponents.Add(Mesh, ISMC);
    }

    for (int32 i = 0; i < CaveMeshData.Triangles.Num(); i += 3)
    {
        const FVector& V0 = CaveMeshData.Vertices[CaveMeshData.Triangles[i]];
        const FVector& V1 = CaveMeshData.Vertices[CaveMeshData.Triangles[i + 1]];
        const FVector& V2 = CaveMeshData.Vertices[CaveMeshData.Triangles[i + 2]];
        const FVector& N0 = CaveMeshData.Normals[CaveMeshData.Triangles[i]];

        if (N0.Z > 0.5f) // Considera "chão"
        {
            const float Area = FVector::CrossProduct(V1 - V0, V2 - V0).Size() / 2.0f;
            const float NumAssetsToSpawnFloat = (Area / 10000.0f) * AssetDensity;
            const int32 NumAssetsToSpawn = FMath::FloorToInt(NumAssetsToSpawnFloat + RandStream.GetFraction());

            for (int32 j = 0; j < NumAssetsToSpawn; ++j)
            {
                // *** CORREÇÃO: Lógica de ponto aleatório em triângulo usando coordenadas baricêntricas ***
                float Bary_B = RandStream.FRand();
                float Bary_C = RandStream.FRand();
                if (Bary_B + Bary_C > 1.0f)
                {
                    Bary_B = 1.0f - Bary_B;
                    Bary_C = 1.0f - Bary_C;
                }
                const float Bary_A = 1.0f - Bary_B - Bary_C;
                const FVector SpawnPos = (Bary_A * V0) + (Bary_B * V1) + (Bary_C * V2);

                UStaticMesh* RandomMesh = Assets[RandStream.RandRange(0, Assets.Num() - 1)];

                const float RandomScale = RandStream.FRandRange(ScaleRange.X, ScaleRange.Y);
                const FRotator RandomRotation = FRotator(0, RandStream.FRandRange(0, 360), 0);

                FTransform InstanceTransform;
                InstanceTransform.SetLocation(SpawnPos + FVector(0,0,ZOffset));
                InstanceTransform.SetRotation(RandomRotation.Quaternion());
                InstanceTransform.SetScale3D(FVector(RandomScale));

                if(InstancedMeshComponents.Contains(RandomMesh))
                {
                    InstancedMeshComponents[RandomMesh]->AddInstance(InstanceTransform);
                }
            }
        }
    }
    UE_LOG(LogCaveGenerator, Log, TEXT("Distribuição de assets concluída."));
}


// ... (O restante do código, como CreateVertexRing, GenerateTunnelSection, GenerateRoom, etc., permanece o mesmo) ...
// (O código completo não é necessário pois a única mudança foi em PlaceAssets e a chamada em Generate)

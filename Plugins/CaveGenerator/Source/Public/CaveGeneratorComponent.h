// Copyright [Your Name]

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h"
#include "CaveGeneratorComponent.generated.h"

// Forward declarations
class USplineComponent;
class UProceduralMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAVEGENERATOR_API UCaveGeneratorComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UCaveGeneratorComponent();

    // --- PROPRIEDADES DE ENTRADA ---

    // A Spline que define o caminho central do túnel.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cave Generation|Setup")
    USplineComponent* PathSpline;

    // Uma CURVA que controla o raio do túnel ao longo do seu comprimento.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Shape")
    UCurveFloat* TunnelRadiusCurve;

    // O número de lados do túnel.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Shape", meta = (ClampMin = "3", ClampMax = "64"))
    int32 TunnelSides = 12;

    // --- NOVAS PROPRIEDADES DE RUGOSIDADE ---

    // A força da deformação. Um valor de 0 significa sem deformação (um túnel liso).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Ruggedness", meta = (ClampMin = "0.0"))
    float RuggednessAmount = 50.0f;

    // A escala do ruído. Valores menores criam detalhes finos e irregulares.
    // Valores maiores criam deformações mais suaves e amplas.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Ruggedness", meta = (ClampMin = "0.01"))
    float RuggednessScale = 100.0f;

    // O material a ser aplicado no túnel gerado.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Visuals")
    UMaterialInterface* TunnelMaterial;


    // --- AÇÕES ---

    // A função principal que gera a malha do túnel.
    UFUNCTION(BlueprintCallable, Category = "Cave Generation|Actions")
    void Generate();

private:
    // O componente que irá conter e renderizar nossa malha gerada proceduralmente.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cave Generation|Components", meta = (AllowPrivateAccess = "true"))
    UProceduralMeshComponent* ProceduralMesh;

protected:
    // Chamado quando o componente é inicializado.
    virtual void OnComponentCreated() override;
};

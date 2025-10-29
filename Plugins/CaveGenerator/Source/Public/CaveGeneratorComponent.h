// Copyright [Your Name]

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Curves/CurveFloat.h"
#include "CaveGeneratorComponent.generated.h"

// Forward declarations
class USplineComponent;
class UProceduralMeshComponent;

// --- ESTRUTURAS DE DADOS PARA AS SALAS ---

// Enum para definir os formatos de sala disponíveis.
// 'BlueprintType' permite que seja usado em Blueprints.
UENUM(BlueprintType)
enum class ECaveRoomShape : uint8
{
    Circular,
    Rectangular
};

// Estrutura que define um marcador de sala.
// 'BlueprintType' permite que seja usado em Blueprints.
USTRUCT(BlueprintType)
struct FCaveRoomMarker
{
    GENERATED_BODY()

    // A posição da sala ao longo da spline (0.0 = início, 1.0 = fim).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Position = 0.5f;

    // O formato da sala.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    ECaveRoomShape Shape = ECaveRoomShape::Circular;

    // As dimensões da sala.
    // Para Circular: X = Raio, Y = Altura.
    // Para Retangular: X = Largura, Y = Comprimento, Z = Altura.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    FVector Size = FVector(1000.0f, 1000.0f, 500.0f);

    // Rotação adicional da sala em torno do seu próprio eixo.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Marker")
    FRotator Rotation = FRotator::ZeroRotator;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAVEGENERATOR_API UCaveGeneratorComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UCaveGeneratorComponent();

    // --- PROPRIEDADES DE ENTRADA ---
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

    // --- NOVA PROPRIEDADE PARA MARCADORES DE SALA ---
    // Um array onde você pode definir todas as salas que quer gerar.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Rooms")
    TArray<FCaveRoomMarker> RoomMarkers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cave Generation|Visuals")
    UMaterialInterface* TunnelMaterial;


    // --- AÇÕES ---
    UFUNCTION(BlueprintCallable, Category = "Cave Generation|Actions")
    void Generate();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cave Generation|Components", meta = (AllowPrivateAccess = "true"))
    UProceduralMeshComponent* ProceduralMesh;

protected:
    virtual void OnComponentCreated() override;
};

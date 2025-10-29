// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Public/CaveGeneratorComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCaveGeneratorComponent() {}

// Begin Cross Module References
CAVEGENERATOR_API UClass* Z_Construct_UClass_UCaveGeneratorComponent();
CAVEGENERATOR_API UClass* Z_Construct_UClass_UCaveGeneratorComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UMaterialInterface_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
ENGINE_API UClass* Z_Construct_UClass_USplineComponent_NoRegister();
PROCEDURALMESHCOMPONENT_API UClass* Z_Construct_UClass_UProceduralMeshComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_CaveGenerator();
// End Cross Module References

// Begin Class UCaveGeneratorComponent Function Generate
struct Z_Construct_UFunction_UCaveGeneratorComponent_Generate_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Cave Generation|Actions" },
		{ "Comment", "// A fun\xc3\xa7\xc3\xa3o principal que gera a malha do t\xc3\xbanel.\n" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "A fun\xc3\xa7\xc3\xa3o principal que gera a malha do t\xc3\xbanel." },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UCaveGeneratorComponent_Generate_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCaveGeneratorComponent, nullptr, "Generate", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UCaveGeneratorComponent_Generate_Statics::Function_MetaDataParams), Z_Construct_UFunction_UCaveGeneratorComponent_Generate_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UCaveGeneratorComponent_Generate()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UCaveGeneratorComponent_Generate_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UCaveGeneratorComponent::execGenerate)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->Generate();
	P_NATIVE_END;
}
// End Class UCaveGeneratorComponent Function Generate

// Begin Class UCaveGeneratorComponent
void UCaveGeneratorComponent::StaticRegisterNativesUCaveGeneratorComponent()
{
	UClass* Class = UCaveGeneratorComponent::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "Generate", &UCaveGeneratorComponent::execGenerate },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCaveGeneratorComponent);
UClass* Z_Construct_UClass_UCaveGeneratorComponent_NoRegister()
{
	return UCaveGeneratorComponent::StaticClass();
}
struct Z_Construct_UClass_UCaveGeneratorComponent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "HideCategories", "Trigger PhysicsVolume" },
		{ "IncludePath", "CaveGeneratorComponent.h" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PathSpline_MetaData[] = {
		{ "Category", "Cave Generation|Setup" },
		{ "Comment", "// A Spline que define o caminho central do t\xc3\xbanel.\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "A Spline que define o caminho central do t\xc3\xbanel." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TunnelRadiusCurve_MetaData[] = {
		{ "Category", "Cave Generation|Shape" },
		{ "Comment", "// Uma CURVA que controla o raio do t\xc3\xbanel ao longo do seu comprimento.\n" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "Uma CURVA que controla o raio do t\xc3\xbanel ao longo do seu comprimento." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TunnelSides_MetaData[] = {
		{ "Category", "Cave Generation|Shape" },
		{ "ClampMax", "64" },
		{ "ClampMin", "3" },
		{ "Comment", "// O n\xc3\xbamero de lados do t\xc3\xbanel.\n" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "O n\xc3\xbamero de lados do t\xc3\xbanel." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RuggednessAmount_MetaData[] = {
		{ "Category", "Cave Generation|Ruggedness" },
		{ "ClampMin", "0.0" },
		{ "Comment", "// A for\xc3\xa7""a da deforma\xc3\xa7\xc3\xa3o. Um valor de 0 significa sem deforma\xc3\xa7\xc3\xa3o (um t\xc3\xbanel liso).\n" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "A for\xc3\xa7""a da deforma\xc3\xa7\xc3\xa3o. Um valor de 0 significa sem deforma\xc3\xa7\xc3\xa3o (um t\xc3\xbanel liso)." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RuggednessScale_MetaData[] = {
		{ "Category", "Cave Generation|Ruggedness" },
		{ "ClampMin", "0.01" },
		{ "Comment", "// A escala do ru\xc3\xad""do. Valores menores criam detalhes finos e irregulares.\n// Valores maiores criam deforma\xc3\xa7\xc3\xb5""es mais suaves e amplas.\n" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "A escala do ru\xc3\xad""do. Valores menores criam detalhes finos e irregulares.\nValores maiores criam deforma\xc3\xa7\xc3\xb5""es mais suaves e amplas." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TunnelMaterial_MetaData[] = {
		{ "Category", "Cave Generation|Visuals" },
		{ "Comment", "// O material a ser aplicado no t\xc3\xbanel gerado.\n" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "O material a ser aplicado no t\xc3\xbanel gerado." },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ProceduralMesh_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Cave Generation|Components" },
		{ "Comment", "// O componente que ir\xc3\xa1 conter e renderizar nossa malha gerada proceduralmente.\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/CaveGeneratorComponent.h" },
		{ "ToolTip", "O componente que ir\xc3\xa1 conter e renderizar nossa malha gerada proceduralmente." },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PathSpline;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TunnelRadiusCurve;
	static const UECodeGen_Private::FIntPropertyParams NewProp_TunnelSides;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RuggednessAmount;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_RuggednessScale;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TunnelMaterial;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ProceduralMesh;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UCaveGeneratorComponent_Generate, "Generate" }, // 4280748918
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCaveGeneratorComponent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_PathSpline = { "PathSpline", nullptr, (EPropertyFlags)0x001000000008001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCaveGeneratorComponent, PathSpline), Z_Construct_UClass_USplineComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PathSpline_MetaData), NewProp_PathSpline_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_TunnelRadiusCurve = { "TunnelRadiusCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCaveGeneratorComponent, TunnelRadiusCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TunnelRadiusCurve_MetaData), NewProp_TunnelRadiusCurve_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_TunnelSides = { "TunnelSides", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCaveGeneratorComponent, TunnelSides), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TunnelSides_MetaData), NewProp_TunnelSides_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_RuggednessAmount = { "RuggednessAmount", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCaveGeneratorComponent, RuggednessAmount), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RuggednessAmount_MetaData), NewProp_RuggednessAmount_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_RuggednessScale = { "RuggednessScale", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCaveGeneratorComponent, RuggednessScale), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RuggednessScale_MetaData), NewProp_RuggednessScale_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_TunnelMaterial = { "TunnelMaterial", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCaveGeneratorComponent, TunnelMaterial), Z_Construct_UClass_UMaterialInterface_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TunnelMaterial_MetaData), NewProp_TunnelMaterial_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_ProceduralMesh = { "ProceduralMesh", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCaveGeneratorComponent, ProceduralMesh), Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ProceduralMesh_MetaData), NewProp_ProceduralMesh_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCaveGeneratorComponent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_PathSpline,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_TunnelRadiusCurve,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_TunnelSides,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_RuggednessAmount,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_RuggednessScale,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_TunnelMaterial,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCaveGeneratorComponent_Statics::NewProp_ProceduralMesh,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCaveGeneratorComponent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UCaveGeneratorComponent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_USceneComponent,
	(UObject* (*)())Z_Construct_UPackage__Script_CaveGenerator,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCaveGeneratorComponent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCaveGeneratorComponent_Statics::ClassParams = {
	&UCaveGeneratorComponent::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UCaveGeneratorComponent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UCaveGeneratorComponent_Statics::PropPointers),
	0,
	0x00B000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCaveGeneratorComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UCaveGeneratorComponent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCaveGeneratorComponent()
{
	if (!Z_Registration_Info_UClass_UCaveGeneratorComponent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCaveGeneratorComponent.OuterSingleton, Z_Construct_UClass_UCaveGeneratorComponent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCaveGeneratorComponent.OuterSingleton;
}
template<> CAVEGENERATOR_API UClass* StaticClass<UCaveGeneratorComponent>()
{
	return UCaveGeneratorComponent::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCaveGeneratorComponent);
UCaveGeneratorComponent::~UCaveGeneratorComponent() {}
// End Class UCaveGeneratorComponent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_romar_OneDrive_Documentos_Unreal_Projects_testeplugin_Plugins_CaveGenerator__Built_HostProject_Plugins_CaveGenerator_Source_Public_CaveGeneratorComponent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCaveGeneratorComponent, UCaveGeneratorComponent::StaticClass, TEXT("UCaveGeneratorComponent"), &Z_Registration_Info_UClass_UCaveGeneratorComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCaveGeneratorComponent), 2854023578U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_romar_OneDrive_Documentos_Unreal_Projects_testeplugin_Plugins_CaveGenerator__Built_HostProject_Plugins_CaveGenerator_Source_Public_CaveGeneratorComponent_h_1010548791(TEXT("/Script/CaveGenerator"),
	Z_CompiledInDeferFile_FID_Users_romar_OneDrive_Documentos_Unreal_Projects_testeplugin_Plugins_CaveGenerator__Built_HostProject_Plugins_CaveGenerator_Source_Public_CaveGeneratorComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_romar_OneDrive_Documentos_Unreal_Projects_testeplugin_Plugins_CaveGenerator__Built_HostProject_Plugins_CaveGenerator_Source_Public_CaveGeneratorComponent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS

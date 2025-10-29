// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CaveGenerator : ModuleRules
{
	public CaveGenerator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "ProceduralMeshComponent",
                "AssetRegistry" // <-- ADICIONADO: Módulo necessário para escanear assets
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}

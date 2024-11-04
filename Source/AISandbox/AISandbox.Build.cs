// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AISandbox : ModuleRules
{
	public AISandbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

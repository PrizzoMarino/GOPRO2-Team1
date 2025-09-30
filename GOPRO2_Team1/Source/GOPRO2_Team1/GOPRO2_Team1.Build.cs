// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GOPRO2_Team1 : ModuleRules
{
	public GOPRO2_Team1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

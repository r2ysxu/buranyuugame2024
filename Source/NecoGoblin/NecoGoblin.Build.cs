// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NecoGoblin : ModuleRules
{
	public NecoGoblin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "Niagara" });
	}
}

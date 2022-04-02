// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class orbit1 : ModuleRules
{
	public orbit1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UsingCameras : ModuleRules
{
	public UsingCameras(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}

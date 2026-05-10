// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnderCore : ModuleRules
{
	public UnderCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"MotionWarping",
			"GameplayTasks",
			"GameplayTags",
			"Niagara",
            "AnimGraphRuntime",
			"UMG",
			"CommonInput",
			"CommonUI",
			"PropertyPath",
			"PreLoadScreen",
			"MotionTrajectory",
            "NavigationSystem"
            });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}

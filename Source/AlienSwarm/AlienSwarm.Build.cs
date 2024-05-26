// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlienSwarm : ModuleRules
{
	public AlienSwarm(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

<<<<<<< Updated upstream
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "Niagara" });
=======
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem" });
>>>>>>> Stashed changes
	}
}

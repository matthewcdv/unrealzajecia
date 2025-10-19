// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ue5_zajecia : ModuleRules
{
	public ue5_zajecia(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ue5_zajecia",
			"ue5_zajecia/Variant_Platforming",
			"ue5_zajecia/Variant_Platforming/Animation",
			"ue5_zajecia/Variant_Combat",
			"ue5_zajecia/Variant_Combat/AI",
			"ue5_zajecia/Variant_Combat/Animation",
			"ue5_zajecia/Variant_Combat/Gameplay",
			"ue5_zajecia/Variant_Combat/Interfaces",
			"ue5_zajecia/Variant_Combat/UI",
			"ue5_zajecia/Variant_SideScrolling",
			"ue5_zajecia/Variant_SideScrolling/AI",
			"ue5_zajecia/Variant_SideScrolling/Gameplay",
			"ue5_zajecia/Variant_SideScrolling/Interfaces",
			"ue5_zajecia/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

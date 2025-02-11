// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sandoria : ModuleRules
{
	public Sandoria(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Networking", "Sockets", "UMG", "Slate", "SlateCore", "NetCore" });

        PublicIncludePaths.AddRange(new string[] { "Sandoria" });
    }
}

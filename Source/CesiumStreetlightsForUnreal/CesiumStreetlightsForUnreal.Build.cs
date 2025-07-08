using System;
using System.IO;
using UnrealBuildTool;

public class CesiumStreetlightsForUnreal : ModuleRules
{
	public CesiumStreetlightsForUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		var ThirdPartyDirectory = Path.Combine(ModuleDirectory, "ThirdParty");
		PublicIncludePaths.Add(Path.Combine(ThirdPartyDirectory, "include"));
		PublicAdditionalLibraries.AddRange(Directory.GetFiles(Path.Combine(ThirdPartyDirectory, "lib"), "*.lib"));
		foreach (var dll in Directory.GetFiles(Path.Combine(ThirdPartyDirectory, "bin"), "*.dll"))
		{
			RuntimeDependencies.Add(Path.Combine("$(BinaryOutputDir)", Path.GetFileName(dll)), dll);
		}

		PublicDependencyModuleNames.AddRange([
			"Core",
			"Engine", 
			"CesiumStreetlightsForUnreal"
		]);
		
		PrivateDependencyModuleNames.AddRange([
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"CesiumRuntime",
		]);
	}
}

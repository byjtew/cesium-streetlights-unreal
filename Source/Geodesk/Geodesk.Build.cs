using UnrealBuildTool;
using System.IO;

public class Geodesk : ModuleRules
{
    public Geodesk(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
        bEnableExceptions = true;
        
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "include"));
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty", "lib", "geodesk.lib"));

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Engine"
        });
    }
}
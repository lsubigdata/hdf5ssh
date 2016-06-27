#!/bin/sh

project=$1
shift
relpath=${1%/}
shift

if [ $# -lt 1 ]
then
   echo 'no files?'
   echo syntax:
   echo 'make-vcproject <targetname> <relativepath>  *.cpp' 
   echo
   exit
fi



cat <<EOF
<?xml version="1.0" encoding="Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="8.00"
	Name="$project"
	ProjectGUID="{3B13FCC3-DC6E-469A-895B-D7F48D4CFB00}"
	>
	<Platforms>
		<Platform
			Name="Win32"
		/>
	</Platforms>
	<ToolFiles>
	</ToolFiles>
	<Configurations>
		<Configuration
			Name="Release|Win32"
			OutputDirectory=".\\${relpath}/lib/arch-win32-Optimize"
			IntermediateDirectory=".\\${relpath}/obj/arch-win32-Optimize/$project"
			ConfigurationType="2"
			InheritedPropertySheets="\$(VCInstallDir)VCProjectDefaults\UpgradeFromVC60.vsprops"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="false"
			>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions="NDEBUG"
				MkTypLibCompatible="true"
				SuppressStartupBanner="true"
				TargetEnvironment="1"
				TypeLibraryName=".\\${relpath}/lib/arch-win32-Optimize/$project.tlb"
				HeaderFileName=""
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="2"
				InlineFunctionExpansion="2"
				PreprocessorDefinitions="NDEBUG;WIN32;_WINDOWS;${project}_EXPORTS"
				StringPooling="true"
				RuntimeLibrary="2"
				EnableFunctionLevelLinking="true"
				RuntimeTypeInfo="true"
                                AdditionalIncludeDirectories="${relpath}"
				PrecompiledHeaderFile=".\\${relpath}/obj/arch-win32-Optimize/$project/$project.pch"
				AssemblerListingLocation=".${relpath}/obj/arch-win32-Optimize/$project/"
				ObjectFile=".\\${relpath}/obj/arch-win32-Optimize/$project/"
				ProgramDataBaseFileName=".\\${relpath}/obj/arch-win32-Optimize/$project/"
				BrowseInformation="1"
				WarningLevel="3"
				SuppressStartupBanner="true"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
				PreprocessorDefinitions="NDEBUG"
				Culture="1033"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="odbc32.lib odbccp32.lib"
				OutputFile=".\\${relpath}/lib/arch-win32-Optimize/$project.dll"
				LinkIncremental="1"
				SuppressStartupBanner="true"
				ProgramDatabaseFile=".\\${relpath}/lib/arch-win32-Optimize/$project.pdb"
				SubSystem="2"
				ImportLibrary=".\\${relpath}/lib/arch-win32-Optimize/$project.lib"
				TargetMachine="1"
			/>
			<Tool
				Name="VCALinkTool"
			/>
			<Tool
				Name="VCManifestTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
				SuppressStartupBanner="true"
				OutputFile=".\\${relpath}/lib/arch-win32-Optimize/$project.bsc"
			/>
			<Tool
				Name="VCFxCopTool"
			/>
			<Tool
				Name="VCAppVerifierTool"
			/>
			<Tool
				Name="VCWebDeploymentTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
		</Configuration>
		<Configuration
			Name="Debug|Win32"
			OutputDirectory=".\\${relpath}/lib/arch-win32-Debug"
			IntermediateDirectory=".\\${relpath}/obj/arch-win32-Debug"
			ConfigurationType="2"
			InheritedPropertySheets="\$(VCInstallDir)VCProjectDefaults\UpgradeFromVC60.vsprops"
			UseOfMFC="0"
			ATLMinimizesCRunTimeLibraryUsage="false"
			>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions="_DEBUG"
				MkTypLibCompatible="true"
				SuppressStartupBanner="true"
				TargetEnvironment="1"
				TypeLibraryName=".\\${relpath}/lib/arch-win32-Debug/$project.tlb"
				HeaderFileName=""
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				PreprocessorDefinitions="_DEBUG;WIN32;_WINDOWS;${project}_EXPORTS"
				BasicRuntimeChecks="3"
				RuntimeLibrary="3"
				RuntimeTypeInfo="true"
                                AdditionalIncludeDirectories="${relpath}"
				PrecompiledHeaderFile=".\\${relpath}/obj/arch-win32-Debug/$project.pch"
				AssemblerListingLocation=".\\${relpath}/obj/arch-win32-Debug/"
				ObjectFile=".\\${relpath}/obj/arch-win32-Debug/"
				ProgramDataBaseFileName=".\\${relpath}/obj/arch-win32-Debug/"
				WarningLevel="3"
				SuppressStartupBanner="true"
				DebugInformationFormat="4"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
				PreprocessorDefinitions="_DEBUG"
				Culture="1033"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="odbc32.lib odbccp32.lib"
				OutputFile=".\\${relpath}/lib/arch-win32-Debug/$project.dll"
				LinkIncremental="2"
				SuppressStartupBanner="true"
				GenerateDebugInformation="true"
				ProgramDatabaseFile=".\\${relpath}/lib/arch-win32-Debug/$project.pdb"
				SubSystem="2"
				ImportLibrary=".\\${relpath}/lib/arch-win32-Debug/$project.lib"
				TargetMachine="1"
			/>
			<Tool
				Name="VCALinkTool"
			/>
			<Tool
				Name="VCManifestTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
				SuppressStartupBanner="true"
				OutputFile=".\\${relpath}/lib/arch-win32-Debug/$project.bsc"
			/>
			<Tool
				Name="VCFxCopTool"
			/>
			<Tool
				Name="VCAppVerifierTool"
			/>
			<Tool
				Name="VCWebDeploymentTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
		</Configuration>
	</Configurations>
	<References>
	</References>
EOF

cat <<EOF
	<Files>
		<Filter
			Name="Source Files"
			Filter="cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
			>
EOF


for f in $* 
do
 if [ ! -r $f ]
 then
    continue;
 fi
cat <<EOF
			<File
				RelativePath="$f"
				>
				<FileConfiguration
					Name="Release|Win32"
					>
					<Tool
						Name="VCCLCompilerTool"
						PreprocessorDefinitions=""
					/>
				</FileConfiguration>
				<FileConfiguration
					Name="Debug|Win32"
					>
					<Tool
						Name="VCCLCompilerTool"
						PreprocessorDefinitions=""
					/>
				</FileConfiguration>
			</File>
EOF
done

cat <<EOF
		</Filter>
		<Filter
			Name="Header Files"
			Filter="h;hpp;hxx;hm;inl"
			>
EOF
for f in *.hpp *.h
do
cat <<EOF
			<File
				RelativePath="$f"
				>
			</File>
EOF
done

cat <<EOF
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>

EOF

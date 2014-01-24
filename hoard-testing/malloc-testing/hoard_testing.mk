##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=hoard_testing
ConfigurationName      :=Debug
IntermediateDirectory  :=Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/ubuntu/Workspace/hoard_codelite/hoard-testing/malloc-testing"
ProjectPath            := "/home/ubuntu/Workspace/hoard_codelite/hoard-testing/malloc-testing"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=ubuntu
Date                   :=01/21/2014
CodeLitePath           :="/home/ubuntu/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := -g -std=c++0x -fPIC $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/NikitaMTtestV2$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d Debug || $(MakeDirCommand) Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/NikitaMTtestV2$(ObjectSuffix): NikitaMTtestV2.cpp $(IntermediateDirectory)/NikitaMTtestV2$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard_codelite/hoard-testing/malloc-testing/NikitaMTtestV2.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/NikitaMTtestV2$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NikitaMTtestV2$(DependSuffix): NikitaMTtestV2.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/NikitaMTtestV2$(ObjectSuffix) -MF$(IntermediateDirectory)/NikitaMTtestV2$(DependSuffix) -MM "/home/ubuntu/Workspace/hoard_codelite/hoard-testing/malloc-testing/NikitaMTtestV2.cpp"

$(IntermediateDirectory)/NikitaMTtestV2$(PreprocessSuffix): NikitaMTtestV2.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NikitaMTtestV2$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard_codelite/hoard-testing/malloc-testing/NikitaMTtestV2.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/NikitaMTtestV2$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/NikitaMTtestV2$(DependSuffix)
	$(RM) $(IntermediateDirectory)/NikitaMTtestV2$(PreprocessSuffix)
	$(RM) $(OutputFile)



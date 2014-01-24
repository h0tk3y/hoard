##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=hoard
ConfigurationName      :=Release
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/ubuntu/Workspace/hoard/hoard-allocator"
ProjectPath            := "/home/ubuntu/Workspace/hoard/hoard-allocator"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=ubuntu
Date                   :=01/24/2014
CodeLitePath           :="/home/ubuntu/.codelite"
LinkerName             :=g++-4.8
ArchiveTool            :=ar rcu
SharedObjectLinkerName :=g++-4.8 -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=
PreprocessSuffix       :=.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++-4.8
C_CompilerName         :=gcc-4.8
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).so
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
MakeDirCommand         :=mkdir -p
CmpOptions             := -std=c++0x --shared -fPIC -O2 -pthread $(Preprocessors)
LinkOptions            :=  -O2
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/src_cpu_id$(ObjectSuffix) $(IntermediateDirectory)/src_hoard_primitives$(ObjectSuffix) $(IntermediateDirectory)/src_intercept$(ObjectSuffix) $(IntermediateDirectory)/src_internals$(ObjectSuffix) $(IntermediateDirectory)/src_mstack$(ObjectSuffix) $(IntermediateDirectory)/src_tests$(ObjectSuffix) $(IntermediateDirectory)/src_tracing$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(SharedObjectLinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_cpu_id$(ObjectSuffix): src/cpu_id.cpp 
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/cpu_id.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/src_cpu_id$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_cpu_id$(PreprocessSuffix): src/cpu_id.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_cpu_id$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/cpu_id.cpp"

$(IntermediateDirectory)/src_hoard_primitives$(ObjectSuffix): src/hoard_primitives.cpp 
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/hoard_primitives.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/src_hoard_primitives$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_hoard_primitives$(PreprocessSuffix): src/hoard_primitives.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_hoard_primitives$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/hoard_primitives.cpp"

$(IntermediateDirectory)/src_intercept$(ObjectSuffix): src/intercept.cpp 
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/intercept.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/src_intercept$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_intercept$(PreprocessSuffix): src/intercept.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_intercept$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/intercept.cpp"

$(IntermediateDirectory)/src_internals$(ObjectSuffix): src/internals.cpp 
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/internals.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/src_internals$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_internals$(PreprocessSuffix): src/internals.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_internals$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/internals.cpp"

$(IntermediateDirectory)/src_mstack$(ObjectSuffix): src/mstack.cpp 
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/mstack.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/src_mstack$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_mstack$(PreprocessSuffix): src/mstack.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_mstack$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/mstack.cpp"

$(IntermediateDirectory)/src_tests$(ObjectSuffix): src/tests.cpp 
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/tests.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/src_tests$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_tests$(PreprocessSuffix): src/tests.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_tests$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/tests.cpp"

$(IntermediateDirectory)/src_tracing$(ObjectSuffix): src/tracing.cpp 
	$(CompilerName) $(SourceSwitch) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/tracing.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/src_tracing$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_tracing$(PreprocessSuffix): src/tracing.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_tracing$(PreprocessSuffix) "/home/ubuntu/Workspace/hoard/hoard-allocator/src/tracing.cpp"

##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/src_cpu_id$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_cpu_id$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_cpu_id$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_hoard_primitives$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_hoard_primitives$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_hoard_primitives$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_intercept$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_intercept$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_intercept$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_internals$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_internals$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_internals$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_mstack$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_mstack$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_mstack$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_tests$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_tests$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_tests$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_tracing$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_tracing$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_tracing$(PreprocessSuffix)
	$(RM) $(OutputFile)



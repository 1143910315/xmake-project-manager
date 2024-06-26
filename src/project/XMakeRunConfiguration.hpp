// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <projectexplorer/buildsystem.h>
#include <projectexplorer/desktoprunconfiguration.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/runconfigurationaspects.h>
#include <projectexplorer/target.h>

#include <utils/hostosinfo.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace XMakeProjectManager::Internal {
    class XMakeRunConfiguration final: public ProjectExplorer::RunConfiguration {
      public:
        XMakeRunConfiguration(ProjectExplorer::Target *target, Utils::Id id);

      private:

        EnvironmentAspect environment { this };
        ExecutableAspect executable { this };
        ArgumentsAspect arguments { this };
        WorkingDirectoryAspect workingDir { this };
        TerminalAspect terminal { this };
        UseLibraryPathsAspect useLibraryPaths { this };
        UseDyldSuffixAspect useDyldSuffix { this };
    };

    class XMakeRunConfigurationFactory final: public ProjectExplorer::RunConfigurationFactory {
      public:
        XMakeRunConfigurationFactory();
    };
} // namespace XMakeProjectManager::Internal

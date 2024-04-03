// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "XMakeRunConfiguration.hpp"

#include <XMakeProjectConstant.hpp>

#include <qtsupport/qtcppkitinfo.h>

#include <projectexplorer/buildsystem.h>
#include <projectexplorer/desktoprunconfiguration.h>
#include <projectexplorer/environmentaspect.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/runconfigurationaspects.h>

#include <utils/environment.h>
#include <utils/hostosinfo.h>

namespace XMakeProjectManager::Internal {
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    XMakeRunConfiguration::XMakeRunConfiguration(ProjectExplorer::Target *target, Utils::Id id)
        : ProjectExplorer::RunConfiguration { target, id } {
        environment.setSupportForBuildEnvironment(target);

        executable.setDeviceSelector(target, ExecutableAspect::RunDevice);

        arguments.setMacroExpander(macroExpander());

        workingDir.setMacroExpander(macroExpander());
        workingDir.setEnvironment(&environment);

        connect(&useLibraryPaths, &BaseAspect::changed,
                &environment, &EnvironmentAspect::environmentChanged);

        if (HostOsInfo::isMacHost()) {
            connect(&useDyldSuffix, &BaseAspect::changed,
                    &environment, &EnvironmentAspect::environmentChanged);
            environment.addModifier([this](Environment &env) {
                if (useDyldSuffix())
                    env.set(QLatin1String("DYLD_IMAGE_SUFFIX"), QLatin1String("_debug"));
            });
        } else {
            useDyldSuffix.setVisible(false);
        }

        environment.addModifier([this](Environment &env) {
            BuildTargetInfo bti = buildTargetInfo();
            if (bti.runEnvModifier)
                bti.runEnvModifier(env, useLibraryPaths());
        });

        setUpdater([this] {
            if (!activeBuildSystem())
                return;

            BuildTargetInfo bti = buildTargetInfo();
            terminal.setUseTerminalHint(bti.usesTerminal);
            executable.setExecutable(bti.targetFilePath);
            workingDir.setDefaultWorkingDirectory(bti.workingDirectory);
            emit environment.environmentChanged();
        });

        connect(target, &Target::buildSystemUpdated, this, &RunConfiguration::update);
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeRunConfiguration::updateTargetInformation() -> void {
        if (!activeBuildSystem()) return;

        auto b_ti            = buildTargetInfo();
        auto terminal_aspect = aspect<ProjectExplorer::TerminalAspect>();
        terminal_aspect->setUseTerminalHint(b_ti.usesTerminal);

        aspect<ProjectExplorer::ExecutableAspect>()->setExecutable(b_ti.targetFilePath);
        aspect<ProjectExplorer::WorkingDirectoryAspect>()->setDefaultWorkingDirectory(
            b_ti.workingDirectory);
        Q_EMIT aspect<ProjectExplorer::LocalEnvironmentAspect>()->environmentChanged();
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    XMakeRunConfigurationFactory::XMakeRunConfigurationFactory() {
        registerRunConfiguration<XMakeRunConfiguration>(
            "XMakeProjectManager.XMakeRunConfiguration");

        addSupportedProjectType(Constants::Project::ID);
        addSupportedTargetDeviceType(ProjectExplorer::Constants::DESKTOP_DEVICE_TYPE);
    }
} // namespace XMakeProjectManager::Internal

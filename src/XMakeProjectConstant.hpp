// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <qplatformdefs.h>

namespace XMakeProjectManager::Constants {
    namespace Icons {
        static constexpr const char * XMAKE              = ":/xmakeproject/icons/xmake_logo.png";
        static constexpr const char * XMAKE_FILE_OVERLAY = ":/xmakeproject/icons/xmake_file_overlay.png";
        static constexpr const char * XMAKE_BW           = ":/xmakeproject/icons/xmake_bw_logo.png";
    } // namespace Icons

    namespace Project {
        static constexpr const char * MIMETYPE = "text/x-xmake-project";
        static constexpr const char * ID       = "XMakeProjectManager.XMakeProject";
    } // namespace Project

    namespace SettingsPage {
        static constexpr const char * GENERAL_ID = "A.XMakeProjectManager.SettingsPage.General";
        static constexpr const char * TOOLS_ID   = "Z.XMakeProjectManager.SettingsPage.Tools";
        static constexpr const char * CATEGORY   = "Z.XMake";
    } // namespace SettingsPage

    namespace ToolsSettings {
        static constexpr const char * FILENAME         = "xmaketool.xml";
        static constexpr const char * ENTRY_KEY        = "XMakeProjectManager.Tool.";
        static constexpr const char * ENTRY_COUNT      = "XMakeProjectManager.Tools.Count";
        static constexpr const char * EXE_KEY          = "XMakeProjectManager.Tool.Exe";
        static constexpr const char * AUTODETECTED_KEY = "XMakeProjectManager.Tool.AutoDetected";
        static constexpr const char * NAME_KEY         = "XMakeProjectManager.Tool.name";
        static constexpr const char * UUID_KEY         = "XMakeProjectManager.Tool.uuid";
        static constexpr const char * TOOL_TYPE_XMAKE  = "xmake";
        static constexpr const char * TOOL_TYPE_XREPO  = "xrepo"; /* for the future */
        static constexpr const char * TOOL_TYPE_KEY    = "XMakeProjectManager.Tool.Type";
        static constexpr const char * AUTORUN_KEY      = "XMakeProjectManager.Tool.Autorun";
        static constexpr const char * AUTO_ACCEPT_REQUESTS_KEY =
            "XMakeProjectManager.Tool.AutoAcceptRequests";
    } // namespace ToolsSettings

    namespace BuildConfiguration {
        static constexpr const char * BUILD_TYPE_KEY = "XMakeProjectManager.BuildConfig.Type";
        static constexpr const char * PARAMETERS_KEY = "XMakeProjectManager.BuildConfig.Parameters";
    } // namespace BuildConfiguration

    namespace BuildStep {
        static constexpr const char * TARGETS_KEY = "XMakeProjectManager.BuildStep.BuildTargets";
        static constexpr const char * TOOL_ARGUMENTS_KEY =
            "XMakeProjectManager.BuildStep.AdditionalArguments";
    } // namespace BuildStep

    namespace Targets {
        static constexpr const char * ALL   = "all";
        static constexpr const char * CLEAN = "clean";
    } // namespace Targets

    static constexpr const char * XMAKE_INFO_DIR = ".xmake";

#if defined(Q_OS_WINDOWS)
    #if INTPTR_MAX == INT64_MAX
    static constexpr const char * XMAKE_INTRO_CONF = "/windows/x64/xmake.conf";
    #else
    static constexpr const char * XMAKE_INTRO_CONF = "/windows/x32/xmake.conf";
    #endif
#elif defined(Q_OS_LINUX)
    #if INTPTR_MAX == INT64_MAX
    static constexpr const char * XMAKE_INTRO_CONF = "/linux/x64/xmake.conf";
    #else
    static constexpr const char * XMAKE_INTRO_CONF = "/linux/x32/xmake.conf";
    #endif
#elif defined(Q_OS_MACOS)
    #if INTPTR_MAX == INT64_MAX
    static constexpr const char * XMAKE_INTRO_CONF = "/macos/x64/xmake.conf";
    #else
    static constexpr const char * XMAKE_INTRO_CONF = "/macos/x32/xmake.conf";
    #endif
#endif

    static constexpr const char * TOOL_NAME = "xmake";
    static constexpr const char * TOOL_ID   = "XMakeProjectManager.XMakeKitInformation.XMake";

    static constexpr const char * XMAKE_TOOL_MANAGER    = "XMakeProjectManager.Tools";
    static constexpr const char * XMAKE_BUILD_STEP_ID   = "XMakeProjectManager.BuildStep";
    static constexpr const char * XMAKE_BUILD_CONFIG_ID = "XMakeProjectManager.BuildConfiguration";
} // namespace XMakeProjectManager::Constants

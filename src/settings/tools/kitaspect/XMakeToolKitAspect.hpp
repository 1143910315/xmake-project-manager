// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <exewrappers/XMakeTools.hpp>
#include <exewrappers/XMakeWrapper.hpp>

#include <utils/id.h>

#include <projectexplorer/kit.h>
#include <projectexplorer/kitmanager.h>

namespace XMakeProjectManager::Internal {    
    class XMakeToolKitAspect final
    {
    public:
        static void setXMakeTool(ProjectExplorer::Kit *kit, Utils::Id id);
        static Utils::Id xmakeToolId(const ProjectExplorer::Kit *kit);

        static inline decltype(auto) xmakeTool(const ProjectExplorer::Kit *kit)
        {
            return XMakeTools::xmakeWrapper(XMakeToolKitAspect::xmakeToolId(kit));
        }

        static inline bool isValid(const ProjectExplorer::Kit *kit)
        {
            auto tool = xmakeToolId(kit);
            return (tool && tool->isValid());
        }
    };
} // namespace XMakeProjectManager::Internal

#include "XMakeToolKitAspect.inl"

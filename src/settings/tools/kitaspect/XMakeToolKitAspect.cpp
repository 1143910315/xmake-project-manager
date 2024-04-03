// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "XMakeToolKitAspect.hpp"

#include <XMakeProjectConstant.hpp>

#include <settings/tools/kitaspect/ToolKitAspectWidget.hpp>

#include <utils/qtcassert.h>

#include <projectexplorer/kitmanager.h>
#include <projectexplorer/task.h>

using namespace ProjectExplorer;

namespace XMakeProjectManager::Internal {

void XMakeToolKitAspect::setXMakeTool(Kit *kit, Utils::Id id)
{
    QTC_ASSERT(kit && id.isValid(), return );
    kit->setValue(Constants::TOOL_ID, id.toSetting());
}

Utils::Id XMakeToolKitAspect::xmakeToolId(const Kit *kit)
{
    QTC_ASSERT(kit, return {});
    return Utils::Id::fromSetting(kit->value(Constants::TOOL_ID));
}

// XMakeToolKitAspectFactory

class XMakeToolKitAspectFactory final : public KitAspectFactory
{
public:
    XMakeToolKitAspectFactory()
    {
        setObjectName(QLatin1String("XMakeKitAspect"));
        setId(Constants::TOOL_ID);
        setDisplayName(Tr::tr("XMake Tool"));
        setDescription(Tr::tr("The XMake tool to use when building a project with XMake.<br>"
                              "This setting is ignored when using other build systems."));
        setPriority(9000);
    }

    Tasks validate(const Kit *k) const final
    {
        Tasks tasks;
        const auto tool = XMakeToolKitAspect::xmakeTool(k);
        if (tool && !tool->isValid())
            tasks << BuildSystemTask{Task::Warning, Tr::tr("Cannot validate this xmake executable.")};
        return tasks;
    }

    void setup(Kit *k) final
    {
        const auto tool = XMakeToolKitAspect::xmakeTool(k);
        if (!tool) {
            const auto autoDetected = XMakeTools::xmakeWrapper();
            if (autoDetected)
                XMakeToolKitAspect::setXMakeTool(k, autoDetected->id());
        }
    }
    void fix(Kit *k) final
    {
        setup(k);
    }

    KitAspect *createKitAspect(Kit *k) const
    {
        return new ToolKitAspectWidget{k, this, ToolKitAspectWidget::ToolType::XMake};
    }

    ItemList toUserOutput( const Kit *k) const
    {
        const auto tool = XMakeToolKitAspect::xmakeTool(k);
        if (tool)
            return {{Tr::tr("XMake"), tool->name()}};
        return {{Tr::tr("XMake"), Tr::tr("Unconfigured")}};
    }
};

const XMakeToolKitAspectFactory theXMakeKitAspectFactory;


} // namespace XMakeProjectManager::Internal

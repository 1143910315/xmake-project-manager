// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "ToolsSettingsAccessor.hpp"

#include <XMakeProjectConstant.hpp>

#include <exewrappers/XMakeWrapper.hpp>

#include <coreplugin/icore.h>

#include <utils/fileutils.h>

#include <QCoreApplication>
#include <QVariantMap>
#include <QGuiApplication>

namespace XMakeProjectManager::Internal {
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto entryName(int index) {
        return QString { QStringLiteral("%1%2") }
            .arg(QLatin1String { Constants::ToolsSettings::ENTRY_KEY })
            .arg(index);
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ToolsSettingsAccessor::ToolsSettingsAccessor() {
        setDocType("QtCreatorXMakeTools");
        setApplicationDisplayName(QGuiApplication::applicationDisplayName());

        addVersionUpgrader(std::make_unique<CMakeToolSettingsUpgraderV0>());
        setBaseFilePath(
            Core::ICore::userResourcePath(QLatin1String { Constants::ToolsSettings::FILENAME }));
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto
        ToolsSettingsAccessor::saveXMakeTools(const std::vector<XMakeTools::XMakeWrapperPtr> &tools,
                                              QWidget *parent) -> void {
        auto data = Utils::Store {};

        auto entry_count = 0;

        for (const auto &tool : tools) {
            data.insert(entryName(entry_count).utf8(), toVariantMap(*tool));

            ++entry_count;
        }

        data.insert(Constants::ToolsSettings::ENTRY_COUNT, entry_count);
        saveSettings(data, parent);
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto ToolsSettingsAccessor::loadXMakeTools(QWidget *parent)
        -> std::vector<XMakeTools::XMakeWrapperPtr> {
        const auto data = restoreSettings(parent);

        const auto entry_count =
            data.value(Constants::ToolsSettings::ENTRY_COUNT, 0).toInt();

        auto result = std::vector<XMakeTools::XMakeWrapperPtr> {};
        result.reserve(entry_count);

        for (auto tool_index = 0; tool_index < entry_count; ++tool_index) {
            auto name = entryName(tool_index);

            if (data.contains(name.uft8())) {
                const auto map = data[name].toMap();
                const auto type =
                    map.value(Constants::ToolsSettings::TOOL_TYPE_KEY,
                              Constants::ToolsSettings::TOOL_TYPE_XMAKE); // TODO xrepo

                if (type == Constants::ToolsSettings::TOOL_TYPE_XMAKE)
                    result.emplace_back(fromVariantMap(data[name].toMap()));
            }
        }

        return result;
    }
} // namespace XMakeProjectManager::Internal

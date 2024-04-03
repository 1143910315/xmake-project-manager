// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "XMakeTools.hpp"

#include <XMakeProjectConstant.hpp>

#include <exewrappers/XMakeWrapper.hpp>

#include <utils/algorithm.h>
#include <utils/filepath.h>
#include <utils/qtcassert.h>

namespace XMakeProjectManager::Internal {

    template<typename T>
    inline bool is(const XMakeTools::XMakeWrapperPtr &tool) {
        return bool(std::dynamic_pointer_cast<T>(tool));
    }

    template<typename T>
    std::shared_ptr<T> tool(const Utils::Id &id, const std::vector<XMakeTools::XMakeWrapperPtr> &tools) {
        static_assert(std::is_base_of<ToolWrapper, T>::value, "Type must derive from ToolWrapper");
        const auto tool =
            std::find_if(std::cbegin(tools),
                         std::cend(tools),
                         [&id](const XMakeTools::XMakeWrapperPtr &tool) { return tool->id() == id; });
        if (tool != std::cend(tools) && is<T>(*tool)) return std::dynamic_pointer_cast<T>(*tool);
        return nullptr;
    }

    template<typename T>
    std::shared_ptr<T> autoDetected(const std::vector<XMakeTools::XMakeWrapperPtr> &tools) {
        static_assert(std::is_base_of<ToolWrapper, T>::value, "Type must derive from ToolWrapper");
        for (const auto &tool : tools) {
            if (tool->autoDetected() && is<T>(tool)) { return std::dynamic_pointer_cast<T>(tool); }
        }
        return nullptr;
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    XMakeTools::XMakeTools() = default;

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    XMakeTools::~XMakeTools() = default;

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeTools::addTool(Utils::Id id,
                             QString name,
                             Utils::FilePath exe,
                             bool autorun,
                             bool auto_accept_requests) -> void {
        addTool(std::make_unique<XMakeWrapper>(std::move(name),
                                               std::move(exe),
                                               std::move(id),
                                               autorun,
                                               auto_accept_requests));
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeTools::addTool(XMakeWrapperPtr &&xmake) -> void {
        auto &self = instance();
        self.m_tools.emplace_back(std::move(xmake));

        Q_EMIT self.toolAdded(*self.m_tools.back());
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeTools::setTools(std::vector<XMakeWrapperPtr> &&tools) -> void {
        auto &self = instance();

        self.m_tools = std::move(tools);

        auto wrapper = xmakeWrapper();
        if (wrapper) return;

        auto path = XMakeWrapper::findTool();
        if (path)
            self.m_tools.emplace_back(std::make_shared<XMakeWrapper>(QStringLiteral("System XMake"),
                                                                     std::move(path.value()),
                                                                     true));
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeTools::updateTool(Utils::Id id,
                                QString name,
                                Utils::FilePath exe,
                                bool autorun,
                                bool auto_accept_requests) -> void {
        auto &self = instance();

        auto item = std::find_if(std::begin(self.m_tools),
                                 std::end(self.m_tools),
                                 [&id](const auto &tool) { return tool->id() == id; });

        if (item != std::cend(self.m_tools)) {
            (*item)->setExe(std::move(exe));
            (*item)->setName(std::move(name));
            (*item)->setAutorun(autorun);
            (*item)->setAutoAcceptRequests(auto_accept_requests);
        } else
            addTool(std::move(id), std::move(name), std::move(exe), autorun, auto_accept_requests);
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeTools::removeTool(Utils::Id id) -> void {
        auto &self = instance();

        auto item = Utils::take(self.m_tools, [&id](const auto &tool) { return tool->id() == id; });

        QTC_ASSERT(item, return);

        Q_EMIT self.toolRemoved(*item->get());
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeTools::xmakeWrapper() -> std::shared_ptr<XMakeWrapper> {
        return autoDetected<XMakeWrapper>(instance()->m_tools);
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto XMakeTools::xmakeWrapper(Utils::Id id) -> std::shared_ptr<XMakeWrapper> {
        return tool<XMakeWrapper>(id, instance()->m_tools);
    }
} // namespace XMakeProjectManager::Internal

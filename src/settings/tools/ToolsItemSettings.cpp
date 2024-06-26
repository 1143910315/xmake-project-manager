// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include "ToolsItemSettings.hpp"
#include "ui_ToolsItemSettings.h"

#include <settings/tools/ToolTreeItem.hpp>

namespace XMakeProjectManager::Internal {
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ToolItemSettings::ToolItemSettings(QWidget *parent) : QWidget { parent } {
        m_ui = std::make_unique<Ui::ToolItemSettings>();

        m_ui->setupUi(this);

        m_ui->xmake_path_chooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
        m_ui->xmake_path_chooser->setHistoryCompleter("XMake.Command.History");

        connect(m_ui->xmake_path_chooser,
                &Utils::PathChooser::rawPathChanged,
                this,
                &ToolItemSettings::store);
        connect(m_ui->xmake_name_line_edit,
                &QLineEdit::textChanged,
                this,
                &ToolItemSettings::store);
        connect(m_ui->autorun_checkbox, &QCheckBox::toggled, this, &ToolItemSettings::store);
        connect(m_ui->auto_accept_requests_checkbox,
                &QCheckBox::toggled,
                this,
                &ToolItemSettings::store);
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ToolItemSettings::~ToolItemSettings() = default;

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto ToolItemSettings::load(ToolTreeItem *item) -> void {
        if (item) {
            m_current_id = std::nullopt;

            m_ui->xmake_name_line_edit->setDisabled(item->isAutoDetected());
            m_ui->xmake_name_line_edit->setText(item->name());

            m_ui->xmake_path_chooser->setDisabled(item->isAutoDetected());
            m_ui->xmake_path_chooser->setFilePath(item->executable());

            m_ui->autorun_checkbox->setChecked(item->autorun());
            m_ui->auto_accept_requests_checkbox->setChecked(item->autoAcceptRequests());

            m_current_id = item->id();
        } else
            m_current_id = std::nullopt;
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    auto ToolItemSettings::store() -> void {
        if (m_current_id)
            Q_EMIT applyChanges(*m_current_id,
                                m_ui->xmake_name_line_edit->text(),
                                m_ui->xmake_path_chooser->filePath(),
                                m_ui->autorun_checkbox->isChecked(),
                                m_ui->auto_accept_requests_checkbox->isChecked());
    }
} // namespace XMakeProjectManager::Internal

#include "ToolsItemSettings.moc"

// Copyright (C) 2022 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once

#include <XMakeProjectConstant.hpp>
#include <XMakeProjectPlugin.hpp>

#include <utils/environment.h>
#include <utils/filepath.h>
#include <utils/fileutils.h>
#include <utils/id.h>
#include <optional>
#include <utils/process.h>

#include <QFile>
#include <QFileInfo>
#include <QTemporaryFile>

#include <tuple>

namespace XMakeProjectManager::Internal {
    template<typename FileT>
    bool containsFiles(const QString &path, const FileT &file);

    template<typename FileT, typename... T>
    bool containesFiles(const QString &path, const FileT &file, const T &...files);

    class Command {
      public:
        Command() = default;
        Command(const Utils::FilePath &exe,
                const Utils::FilePath &workDir,
                const QStringList &args);

        Command(Command &&);
        Command &operator=(Command &&);

        Command(const Command &);
        Command &operator=(const Command &);

        const Utils::CommandLine &cmdLine() const;

        const Utils::FilePath &workDir() const;

        Utils::FilePath executable() const;

        QStringList arguments() const;

        QString toUserOutput() const;

      private:
        Utils::CommandLine m_cmd;
        Utils::FilePath m_work_dir;
    };

    bool runXMake(const Command &command, QIODevice *output = nullptr);
    bool isSetup(const Utils::FilePath &build_path);

    class XMakeWrapper {
      public:
        XMakeWrapper() = delete;
        XMakeWrapper(QString name,
                     Utils::FilePath path,
                     bool auto_detected        = false,
                     bool autorun              = false,
                     bool auto_accept_requests = false);
        XMakeWrapper(QString name,
                     Utils::FilePath path,
                     Utils::Id id,
                     bool auto_detected        = false,
                     bool autorun              = false,
                     bool auto_accept_requests = false);
        ~XMakeWrapper();

        XMakeWrapper(XMakeWrapper &&);
        XMakeWrapper(const XMakeWrapper &);

        XMakeWrapper &operator=(const XMakeWrapper &);
        XMakeWrapper &operator=(XMakeWrapper &&);

        bool isValid() const noexcept;
        bool autoDetected() const noexcept;
        const Utils::Id &id() const noexcept;
        QString name() const noexcept;
        const Utils::FilePath &exe() const noexcept;
        bool autorun() const noexcept;
        bool autoAcceptRequests() const noexcept;

        void setName(QString value) noexcept;
        void setExe(Utils::FilePath value);
        void setAutorun(bool value) noexcept;
        void setAutoAcceptRequests(bool value) noexcept;

        static std::optional<Utils::FilePath> findTool();

        Command configure(const Utils::FilePath &source_directory,
                          const Utils::FilePath &build_directory,
                          const QStringList &options = {},
                          bool wipe                  = false) const;

        Command introspect(const Utils::FilePath &source_directory);

        static QString toolName();

      private:
        QString decompressIntrospectLuaIfNot();

        bool m_is_valid;
        bool m_autodetected;
        Utils::Id m_id;
        Utils::FilePath m_exe;
        QString m_name;

        bool m_autorun;
        bool m_auto_accept_requests;
    };

    Utils::Store toVariantMap(const XMakeWrapper &xmake);
    XMakeWrapper *fromVariantMap(const Utils::Store &data);
} // namespace XMakeProjectManager::Internal

#include "XMakeWrapper.inl"

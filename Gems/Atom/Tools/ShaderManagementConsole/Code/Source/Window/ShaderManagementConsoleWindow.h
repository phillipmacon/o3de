/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <Atom/Document/ShaderManagementConsoleDocumentNotificationBus.h>
#include <AzCore/Memory/SystemAllocator.h>

#include <Atom/RPI.Edit/Shader/ShaderVariantListSourceData.h>
#include <Atom/RPI.Public/Shader/Shader.h>
#include <AtomToolsFramework/Window/AtomToolsMainWindow.h>

AZ_PUSH_DISABLE_WARNING(4251 4800, "-Wunknown-warning-option") // disable warnings spawned by QT
#include <AzQtComponents/Components/DockMainWindow.h>
#include <AzQtComponents/Components/FancyDocking.h>
#include <AzQtComponents/Components/StyledDockWidget.h>
#include <AzQtComponents/Components/Widgets/TabWidget.h>

#include <Window/ShaderManagementConsoleBrowserWidget.h>
#include <Window/ToolBar/ShaderManagementConsoleToolBar.h>

#include <QMenuBar>
#include <QStandardItemModel>
#include <QToolBar>
AZ_POP_DISABLE_WARNING
#endif

namespace AzToolsFramework
{
    class CScriptTermDialog;
}

namespace ShaderManagementConsole
{
    /**
     * ShaderManagementConsoleWindow is the main class. Its responsibility is limited to initializing and connecting
     * its panels, managing selection of assets, and performing high-level actions like saving. It contains...
     */
    class ShaderManagementConsoleWindow
        : public AtomToolsFramework::AtomToolsMainWindow
        , private ShaderManagementConsoleDocumentNotificationBus::Handler
    {
        Q_OBJECT
    public:
        AZ_CLASS_ALLOCATOR(ShaderManagementConsoleWindow, AZ::SystemAllocator, 0);

        using Base = AtomToolsFramework::AtomToolsMainWindow;

        ShaderManagementConsoleWindow(QWidget* parent = 0);
        ~ShaderManagementConsoleWindow();

    private:
        // ShaderManagementConsoleDocumentNotificationBus::Handler overrides...
        void OnDocumentOpened(const AZ::Uuid& documentId) override;
        void OnDocumentClosed(const AZ::Uuid& documentId) override;
        void OnDocumentModified(const AZ::Uuid& documentId) override;
        void OnDocumentUndoStateChanged(const AZ::Uuid& documentId) override;
        void OnDocumentSaved(const AZ::Uuid& documentId) override;

        void SetupMenu() override;

        void SetupTabs() override;
        void AddTabForDocumentId(const AZ::Uuid& documentId) override;
        void UpdateTabForDocumentId(const AZ::Uuid& documentId) override;

        void OpenTabContextMenu() override;

        void SelectDocumentForTab(const int tabIndex);
        void CloseDocumentForTab(const int tabIndex);
        void CloseAllExceptDocumentForTab(const int tabIndex);

        void closeEvent(QCloseEvent* closeEvent) override;

        void CreateDocumentContent(const AZ::Uuid& documentId, QStandardItemModel* model);

        ShaderManagementConsoleToolBar* m_toolBar = nullptr;

        QMenu* m_menuNew = {};
        QAction* m_actionOpen = {};
        QAction* m_actionOpenRecent = {};
        QAction* m_actionClose = {};
        QAction* m_actionCloseAll = {};
        QAction* m_actionCloseOthers = {};
        QAction* m_actionSave = {};
        QAction* m_actionSaveAsCopy = {};
        QAction* m_actionSaveAll = {};
        QAction* m_actionExit = {};

        QMenu* m_menuEdit = {};
        QAction* m_actionUndo = {};
        QAction* m_actionRedo = {};
        QAction* m_actionSettings = {};

        QMenu* m_menuView = {};
        QAction* m_actionAssetBrowser = {};
        QAction* m_actionPythonTerminal = {};
        QAction* m_actionNextTab = {};
        QAction* m_actionPreviousTab = {};

        QMenu* m_menuHelp = {};
        QAction* m_actionHelp = {};
        QAction* m_actionAbout = {};
    };
} // namespace ShaderManagementConsole

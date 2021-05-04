/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#if !defined(Q_MOC_RUN)
#include <source/ui/AssetBundlerTabWidget.h>

#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzFramework/Platform/PlatformDefaults.h>

#include <QDir>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QSharedPointer>
#include <QString>
#include <QWidget>
#endif

namespace Ui
{
    class AssetListTabWidget;
}

class QFileSystemModel;
class QStringListModel;

namespace AssetBundler
{
    class GUIApplicationManager;

    class AssetListFileTableModel;
    class AssetListTableModel;

    class AssetListTabWidget
        : public AssetBundlerTabWidget
    {
        Q_OBJECT

    public:
        explicit AssetListTabWidget(QWidget* parent, GUIApplicationManager* guiApplicationManager);
        virtual ~AssetListTabWidget() {}

        //////////////////////////////////////////////////////////////////////////
        // AssetBundlerTabWidget overrides
        QString GetTabTitle() override { return tr("Asset Lists"); }
        QString GetFileTypeDisplayName() override { return tr("Asset List file"); }
        AssetBundlingFileType GetFileType() override { return AssetBundlingFileType::AssetListFileType; }
        bool HasUnsavedChanges() override { return false; };
        void Reload() override;
        bool SaveCurrentSelection() override { return true; };
        bool SaveAll() override { return true; };
        void SetModelDataSource() override;
        AzQtComponents::TableView* GetFileTableView() override;
        QModelIndex GetSelectedFileTableIndex() override;
        AssetBundlerAbstractFileTableModel* GetFileTableModel() override;
        void SetActiveProjectLabel(const QString& labelText) override;
        void ApplyConfig() override;
        void FileSelectionChanged(const QItemSelection& /*selected*/ = QItemSelection(), const QItemSelection& /*deselected*/ = QItemSelection()) override;
        //////////////////////////////////////////////////////////////////////////

    private:
        void OnGenerateBundleButtonPressed();

        QSharedPointer<Ui::AssetListTabWidget> m_ui;

        QSharedPointer<AssetListFileTableModel> m_fileTableModel;
        QModelIndex m_selectedFileTableIndex;

        QSharedPointer<AssetBundler::AssetBundlerFileTableFilterModel> m_assetListContentsFilterModel;
        QSharedPointer<AssetListTableModel> m_assetListContentsModel;
    };
} // namespace AssetBundler

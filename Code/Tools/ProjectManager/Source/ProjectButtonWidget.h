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
#include <ProjectInfo.h>

#include <QLabel>
#endif

QT_FORWARD_DECLARE_CLASS(QPixmap)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QProgressBar)

namespace O3DE::ProjectManager
{
    class LabelButton
        : public QLabel
    {
        Q_OBJECT // AUTOMOC

    public:
        explicit LabelButton(QWidget* parent = nullptr);
        ~LabelButton() = default;

        void SetEnabled(bool enabled);
        void SetOverlayText(const QString& text);

        QLabel* GetOverlayLabel();
        QProgressBar* GetProgressBar();
        QPushButton* GetBuildButton();

    signals:
        void triggered();

    public slots:
        void mousePressEvent(QMouseEvent* event) override;

    private:
        QLabel* m_overlayLabel;
        QProgressBar* m_progressBar;
        QPushButton* m_buildButton;
        bool m_enabled = true;
    };

    class ProjectButton
        : public QFrame
    {
        Q_OBJECT // AUTOMOC

    public:
        explicit ProjectButton(const ProjectInfo& m_projectInfo, QWidget* parent = nullptr, bool processing = false);
        ~ProjectButton() = default;

        void SetLaunchButtonEnabled(bool enabled);
        void ShowBuildButton(bool show);
        void SetButtonOverlayText(const QString& text);
        void SetProgressBarValue(int progress);

    signals:
        void OpenProject(const QString& projectName);
        void EditProject(const QString& projectName);
        void CopyProject(const QString& projectName);
        void RemoveProject(const QString& projectName);
        void DeleteProject(const QString& projectName);
        void BuildProject(const ProjectInfo& projectInfo);

    private:
        void BaseSetup();
        void ProcessingSetup();
        void ReadySetup();

        ProjectInfo m_projectInfo;
        LabelButton* m_projectImageLabel;
        QFrame* m_projectFooter;
    };
} // namespace O3DE::ProjectManager

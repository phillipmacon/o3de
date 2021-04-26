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

#ifndef COLOR_PICKER_DELEGATE_HXX
#define COLOR_PICKER_DELEGATE_HXX

#if !defined(Q_MOC_RUN)
#include <AzCore/Memory/SystemAllocator.h>
#include <QtWidgets/QStyledItemDelegate>
#endif

#pragma once

namespace AzToolsFramework
{
    /**
    * A delegate which handles the double clicking to pop open a color picker dialog, as long as the role is COLOR_PICKER_ROLE.
    * To use it, just add a setData() and a data() function to your model which returns a QColor (or accepts one) whenever the COLOR_PICKER_ROLE is queried.
    **/
    class ColorPickerDelegate
        : public QStyledItemDelegate
    {
        Q_OBJECT;
    public:
        static const int COLOR_PICKER_ROLE = Qt::UserRole + 1;

        AZ_CLASS_ALLOCATOR(ColorPickerDelegate, AZ::SystemAllocator, 0);
        ColorPickerDelegate(QObject* pParent);
        virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
        virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
        virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    };
} // namespace AzToolsFramework

#endif //COLOR_PICKER_DELEGATE_HXX

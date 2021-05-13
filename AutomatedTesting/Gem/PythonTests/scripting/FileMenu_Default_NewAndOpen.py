"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
"""
from PySide2 import QtWidgets

import editor_python_test_tools.pyside_utils as pyside_utils
from editor_python_test_tools.utils import Report

import azlmbr.legacy.general as general


# fmt: off
class Tests():
    new_action  = "File->New action working as expected"
    open_action = "File->Open action working as expected"
# fmt: on


GENERAL_WAIT = 0.5  # seconds


class TestFileMenuDefaultNewOpen:
    """
    Summary:
     When clicked on File->New, new script opens
     File->Open should open the FileBrowser

    Expected Behavior:
     New and Open actions should work as expected.

    Test Steps:
     1) Open Script Canvas window (Tools > Script Canvas)
     2) Get the SC window object
     3) Trigger File->New action
     4) Verify if New tab is opened
     5) Trigger File->Open action
     6) Close Script Canvas window


    Note:
     - This test file must be called from the Open 3D Engine Editor command terminal
     - Any passed and failed tests are written to the Editor.log file.
        Parsing the file or running a log_monitor are required to observe the test results.

    :return: None
    """

    @pyside_utils.wrap_async
    async def run_test(self):
        # 1) Open Script Canvas window (Tools > Script Canvas)
        general.open_pane("Script Canvas")

        # 2) Get the SC window object
        editor_window = pyside_utils.get_editor_main_window()
        sc = editor_window.findChild(QtWidgets.QDockWidget, "Script Canvas")
        sc_main = sc.findChild(QtWidgets.QMainWindow)
        sc_tabs = sc_main.findChild(QtWidgets.QTabWidget, "ScriptCanvasTabs")

        # 3) Trigger File->New action
        initial_tabs_count = sc_tabs.count()
        action = pyside_utils.find_child_by_pattern(
            sc_main, {"objectName": "action_New_Script", "type": QtWidgets.QAction}
        )
        action.trigger()

        # 4) Verify if New tab is opened
        general.idle_wait(GENERAL_WAIT)
        Report.info(f"{Tests.new_action}: {sc_tabs.count() == initial_tabs_count + 1}")

        # 5) Trigger File->Open action
        action = pyside_utils.find_child_by_pattern(sc_main, {"objectName": "action_Open", "type": QtWidgets.QAction})
        pyside_utils.trigger_action_async(action)
        general.idle_wait(GENERAL_WAIT)
        popup = await pyside_utils.wait_for_modal_widget()
        Report.info(f"{Tests.open_action}: {popup and 'Open' in popup.windowTitle()}")
        popup.close()

        # 6) Close Script Canvas window
        general.close_pane("Script Canvas")


test = TestFileMenuDefaultNewOpen()
test.run_test()

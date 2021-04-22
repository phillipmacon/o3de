"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
"""


# Test case ID :  C28798177
# Test Case Title : White Box Tool Component can be added to an Entity
# URL of the test case : https://testrail.agscollab.com/index.php?/cases/view/28798177


# fmt:off
class Tests():
    white_box_entity_created = ("White box entity created",                      "Failed to create white box entity")
    white_box_component_enabled = ("White box component enabled",                "Failed to enable white box component")
# fmt:on


def run():
    import os
    import sys
    import WhiteBoxInit as init
    import ImportPathHelper as imports
    imports.init()

    import azlmbr.bus as bus
    import azlmbr.editor as editor
    import azlmbr.legacy.general as general

    from editor_python_test_tools.utils import Report
    
    from editor_python_test_tools.utils import TestHelper as helper

    # open level
    helper.init_idle()
    general.open_level("EmptyLevel")

    # create white box entity and attach component
    white_box_entity = init.create_white_box_entity()
    white_box_mesh_component = init.create_white_box_component(white_box_entity)
    init.create_white_box_handle(white_box_mesh_component)

    # verify results
    entity_id = general.find_editor_entity('WhiteBox')
    Report.result(Tests.white_box_entity_created, entity_id.IsValid())

    component_enabled = editor.EditorComponentAPIBus(bus.Broadcast, 'IsComponentEnabled', white_box_mesh_component)
    Report.result(Tests.white_box_component_enabled, component_enabled)

    # close editor
    helper.close_editor()


if __name__ == "__main__":
    run()

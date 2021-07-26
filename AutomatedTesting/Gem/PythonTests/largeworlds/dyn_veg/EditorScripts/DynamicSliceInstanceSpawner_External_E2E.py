"""
Copyright (c) Contributors to the Open 3D Engine Project.
For complete copyright and license terms please see the LICENSE at the root of this distribution.

SPDX-License-Identifier: Apache-2.0 OR MIT
"""

import os
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)))
import azlmbr.legacy.general as general
import azlmbr.asset as asset
import azlmbr.bus as bus
import azlmbr.components as components
import azlmbr.entity as entity
import azlmbr.editor as editor
import azlmbr.math as math
import azlmbr.paths

sys.path.append(os.path.join(azlmbr.paths.devroot, 'AutomatedTesting', 'Gem', 'PythonTests'))
import editor_python_test_tools.hydra_editor_utils as hydra
from editor_python_test_tools.editor_test_helper import EditorTestHelper
from largeworlds.large_worlds_utils import editor_dynveg_test_helper as dynveg


class TestDynamicSliceInstanceSpawnerExternalEditor(EditorTestHelper):
    def __init__(self):
        EditorTestHelper.__init__(self, log_prefix="DynamicSliceInstanceSpawnerExternalEditor", args=["level"])

    def run_test(self):
        """
        Summary:
        A new temporary level is created. Surface for planting is created. Simple vegetation area is created using
        Dynamic Slice Instance Spawner type using external assets.

        Expected Behavior:
        Instances plant as expected in the assigned area.

        Test Steps:
         1) Create level
         2) Create a Vegetation Layer Spawner setup using Dynamic Slice Instance Spawner type assets
         3) Create a surface to plant on
         4) Verify expected instance counts
         5) Add a camera component looking at the planting area for visual debugging
         6) Save and export to engine

        Note:
        - This test file must be called from the Open 3D Engine Editor command terminal
        - Any passed and failed tests are written to the Editor.log file.
                Parsing the file or running a log_monitor are required to observe the test results.

        :return: None
        """

        # 1) Create a new, temporary level
        self.test_success = self.create_level(
            self.args["level"],
            heightmap_resolution=1024,
            heightmap_meters_per_pixel=1,
            terrain_texture_resolution=4096,
            use_terrain=False,
        )

        general.set_current_view_position(512.0, 480.0, 38.0)

        # 2) Create a new entity with required vegetation area components and switch the Vegetation Asset List Source
        # Type to External
        entity_position = math.Vector3(512.0, 512.0, 32.0)
        veg_area_required_components = ["Vegetation Layer Spawner", "Box Shape", "Vegetation Asset List",
                                        "Script Canvas"]
        new_entity_id = editor.ToolsApplicationRequestBus(
            bus.Broadcast, "CreateNewEntityAtPosition", entity_position, entity.EntityId()
        )
        if new_entity_id.IsValid():
            self.log("Spawner entity created")
        spawner_entity = hydra.Entity("Spawner Entity", new_entity_id)
        spawner_entity.components = []
        for component in veg_area_required_components:
            spawner_entity.components.append(hydra.add_component(component, new_entity_id))
        hydra.get_set_test(spawner_entity, 2, "Configuration|Source Type", 1)

        # Add a Script Canvas component with instance_counter script for launcher tests
        instance_counter_path = os.path.join("scriptcanvas", "instance_counter.scriptcanvas")
        instance_counter_script = asset.AssetCatalogRequestBus(bus.Broadcast, "GetAssetIdByPath", instance_counter_path,
                                                               math.Uuid(), False)
        spawner_entity.get_set_test(3, "Script Canvas Asset|Script Canvas Asset", instance_counter_script)

        # Assign a Vegetation Descriptor List asset to the Vegetation Asset List component
        descriptor_asset = asset.AssetCatalogRequestBus(
            bus.Broadcast, "GetAssetIdByPath", os.path.join("Assets", "VegDescriptorLists", "flower_pink.vegdescriptorlist"), math.Uuid(),
            False)
        hydra.get_set_test(spawner_entity, 2, "Configuration|External Assets", descriptor_asset)

        # Resize the Box Shape component
        new_box_dimensions = math.Vector3(16.0, 16.0, 16.0)
        box_dimensions_path = "Box Shape|Box Configuration|Dimensions"
        hydra.get_set_test(spawner_entity, 1, box_dimensions_path, new_box_dimensions)

        # 3) Create a surface to plant on
        dynveg.create_surface_entity("Planting Surface", entity_position, 128.0, 128.0, 1.0)

        # 4) Verify instance counts are accurate
        general.idle_wait(3.0)  # Allow a few seconds for instances to spawn
        num_expected_instances = 20 * 20
        box = azlmbr.shape.ShapeComponentRequestsBus(bus.Event, 'GetEncompassingAabb', spawner_entity.id)
        num_found = azlmbr.areasystem.AreaSystemRequestBus(bus.Broadcast, 'GetInstanceCountInAabb', box)
        self.log(f"Expected {num_expected_instances} instances - Found {num_found} instances")
        self.test_success = self.test_success and num_found == num_expected_instances

        # 5) Move the default Camera entity for testing in the launcher
        cam_position = math.Vector3(512.0, 500.0, 35.0)
        search_filter = entity.SearchFilter()
        search_filter.names = ["Camera"]
        search_entity_ids = entity.SearchBus(bus.Broadcast, 'SearchEntities', search_filter)
        components.TransformBus(bus.Event, "MoveEntity", search_entity_ids[0], cam_position)

        # 6) Save and export to engine
        general.save_level()
        general.idle_wait(1.0)
        general.export_to_engine()
        general.idle_wait(1.0)


test = TestDynamicSliceInstanceSpawnerExternalEditor()
test.run()

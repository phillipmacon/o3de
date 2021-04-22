"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
"""

import os
import sys
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from ActorSplitsAfterDamage import Tests

def run():
    from ActorSplitsAfterDamage import run as internal_run
    from editor_python_test_tools.utils import Constants

    def ShearDamage(target_id, position):
        normal = azlmbr.object.construct('Vector3', 1.0, 0.0, 0.0)
        azlmbr.destruction.BlastFamilyDamageRequestBus(azlmbr.bus.Event, "Shear Damage", target_id,
                                                       position, normal, Constants.DAMAGE_MIN_RADIUS,
                                                       Constants.DAMAGE_MAX_RADIUS, Constants.DAMAGE_AMOUNT)

    internal_run(ShearDamage)


if __name__ == "__main__":
    run()

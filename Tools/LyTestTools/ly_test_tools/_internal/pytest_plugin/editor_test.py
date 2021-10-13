"""
Copyright (c) Contributors to the Open 3D Engine Project.
For complete copyright and license terms please see the LICENSE at the root of this distribution.

SPDX-License-Identifier: Apache-2.0 OR MIT

Utility for specifying an Editor test, supports seamless parallelization and/or batching of tests.
"""

import pytest
import inspect

__test__ = False

def pytest_addoption(parser):
    # type (argparse.ArgumentParser) -> None
    """
    Options when running tests in batches or parallel.
    :param parser: The ArgumentParser object
    :return: None
    """
    parser.addoption("--no-editor-batch", action="store_true", help="Don't batch multiple tests in single editor")
    parser.addoption("--no-editor-parallel", action="store_true", help="Don't run multiple editors in parallel")
    parser.addoption("--editors-parallel", type=int, action="store", help="Override the number editors to run at the same time")

def pytest_pycollect_makeitem(collector, name, obj):
    # type (PyCollector, str, object) -> Collector
    """
    Create a custom custom item collection if the class defines pytest_custom_makeitem function. This is used for
    automtically generating test functions with a custom collector.
    :param collector: The Python test collector
    :param name: Name of the collector
    :param obj: The custom collector, normally an EditorTestSuite.EditorTestClass object
    :return: Returns the custom collector
    """
    if inspect.isclass(obj):
        for base in obj.__bases__:
            if hasattr(base, "pytest_custom_makeitem"):
                return base.pytest_custom_makeitem(collector, name, obj)

@pytest.hookimpl(hookwrapper=True)
def pytest_collection_modifyitems(session, items, config):
    # type (Session, list, Config) -> None
    """
    Add custom modification of items. This is used for adding the runners into the item list.
    :param session: The Pytest Session
    :param items: The test case functions
    :param config: The Pytest Config object
    :return: None
    """
    all_classes = set()
    for item in items:
        all_classes.add(item.instance.__class__)

    yield

    for cls in all_classes:
        if hasattr(cls, "pytest_custom_modify_items"):
            cls.pytest_custom_modify_items(session, items, config)


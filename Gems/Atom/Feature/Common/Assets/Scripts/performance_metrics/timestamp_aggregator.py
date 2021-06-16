#!/usr/bin/env python
"""
All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
its licensors.

For complete copyright and license terms please see the LICENSE at the root of this
distribution (the "License"). All use of this software is governed by the License,
or, if provided, by the license below or the license accompanying this file. Do not
remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
"""

from genericpath import isdir
from argparse import ArgumentParser
import json
from pathlib import Path
import time

# this allows us to add additional data if necessary, e.g. frame_test_timestamps.json
is_timestamp_file = lambda file: file.name.startswith('frame') and file.name.endswith('_timestamps.json')
ns_to_ms = lambda time: time / 1e6

def main(logs_dir):
    frame_count = 0
    frame_time_total = 0
    frame_time_max = 0
    pass_stats = {}

    print(f'Analyzing frame timestamp logs in {logs_dir}')

    # go through files in alphabetical order (remove sorted() if not necessary)
    for file in sorted(logs_dir.iterdir(), key=lambda file: len(file.name)):
        if file.is_dir() or not is_timestamp_file(file):
            continue

        data = json.loads(file.read_text())
        entries = data['ClassData']['timestampEntries']

        frame_time = 0
        for entry in entries:
            name = entry['passName']
            time_ns = entry['timestampResultInNanoseconds']
            pass_entry = pass_stats.get(name, {'max': 0, 'total': 0})

            pass_entry['max'] = max(time_ns, pass_entry['max'])
            pass_entry['total'] += time_ns
            pass_stats[name] = pass_entry

            frame_time += time_ns
        frame_time_total += frame_time

        frame_name = file.name.split('_')[0]
        print(f'- Total time for frame {frame_name}: {ns_to_ms(frame_time)}ms')

        frame_time_max = max(frame_time, frame_time_max)
        frame_count += 1

    if frame_count < 1:
        print(f'No logs were found in {base_dir}')
        exit(1)

    frame_avg = frame_time_total / frame_count
    print(f'Avg time across {frame_count} frames: {ns_to_ms(frame_avg)}ms')
    print(f'Max frame time: {ns_to_ms(frame_time_max)}ms')
    print('Pass statistics:')
    for name, stat in pass_stats.items():
        avg_ms = ns_to_ms(stat['total'] / frame_count)
        max_ms = ns_to_ms(stat['max'])
        print(f'- {name}: {avg_ms}ms avg, {max_ms}ms max')

if __name__ == '__main__':
    parser = ArgumentParser(description='Gathers statistics from a group of pass timestamp logs')
    parser.add_argument('path', help='Path to the directory containing the pass timestamp logs')
    args = parser.parse_args()

    base_dir = Path(args.path)
    if not base_dir.exists():
        raise FileNotFoundError('Invalid path provided')
    main(base_dir)

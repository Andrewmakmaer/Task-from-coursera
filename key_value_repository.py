from __future__ import print_function
import argparse
import json
import os
import tempfile


def list_printer(p_list):
    for i in range(len(p_list)):
        if i != len(p_list) - 1:
            print(p_list[i] + ",", end=' ')
        else:
            print(p_list[i])


parser = argparse.ArgumentParser()
parser.add_argument("--key", help="key", required=True)
parser.add_argument("--value", help="value")
args = parser.parse_args()
key_value_map = dict()

storage_path = os.path.join(tempfile.gettempdir(), 'storage.data')

if not args.value:
    try:
        with open(storage_path) as f:
            key_value_map = json.load(f)

        if not args.key in key_value_map:
            print("")
        else:
            list_printer(key_value_map[args.key])

    except:
        with open(storage_path, 'w') as f:
            print("")
else:
    try:
        with open(storage_path, 'r') as f:
            key_value_map = json.load(f)

        if args.key in key_value_map:
            key_value_map[args.key].append(args.value)
        else:
            key_value_map[args.key] = [args.value]

        with open(storage_path, 'w') as f:
            json.dump(key_value_map, f)
    except:
        with open(storage_path, 'w') as f:
            key_value_map[args.key] = [args.value]
            json.dump(key_value_map, f)
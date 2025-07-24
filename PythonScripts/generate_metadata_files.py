#!/usr/bin/env python3

# Run python generate.py inputs [path(s) to qml files]. 
# This script will place filename.metadata.json and filename.history.json next to wherever the qml file is.
    # The history file will be an empty array, the metadata file will have only the timestamp fields set.
    # Use populate_fake_metadata.py to then fill these files with fake data.

import os
import json
import argparse
from datetime import datetime

# Default metadata template
DEFAULT_METADATA = {
    "tags": [],
    "creator": "",
    "last_edited_by": "",
    "category": "",
    "description": "",
    "version": "1.0",
    "created": "",   # will be set to now
    "updated": ""
}

def write_json_files_for_qml(qml_path: str):
    base_name = os.path.splitext(os.path.basename(qml_path))[0]
    asset_dir = os.path.dirname(qml_path)

    # Construct paths for .metadata.json and .history.json
    metadata_path = os.path.join(asset_dir, f"{base_name}.metadata.json")
    history_path = os.path.join(asset_dir, f"{base_name}.history.json")

    now = datetime.utcnow().isoformat() + "Z"
    metadata = DEFAULT_METADATA.copy()
    metadata["created"] = now
    metadata["updated"] = now

    with open(metadata_path, "w", encoding="utf-8") as f:
        json.dump(metadata, f, indent=4)
    print(f"✅ Created metadata: {metadata_path}")

    with open(history_path, "w", encoding="utf-8") as f:
        json.dump([], f, indent=4)
    print(f"✅ Created empty history: {history_path}")

def find_qml_files(path: str):
    if os.path.isdir(path):
        return [os.path.join(path, f) for f in os.listdir(path) if f.lower().endswith(".qml")]
    elif os.path.isfile(path) and path.lower().endswith(".qml"):
        return [path]
    else:
        return []

def main():
    parser = argparse.ArgumentParser(description="Generate initial metadata and history JSON next to each .qml asset file")
    parser.add_argument("inputs", nargs="+", help="Paths to .qml files or folders")
    args = parser.parse_args()

    all_qml_files = []
    for inp in args.inputs:
        all_qml_files.extend(find_qml_files(inp))

    if not all_qml_files:
        print("⚠️  No .qml files found.")
        return

    for qml in all_qml_files:
        print(f"Processing: {qml}")
        try:
            write_json_files_for_qml(qml)
        except Exception as e:
            print(f"  ❌ Error: {e}")

if __name__ == "__main__":
    main()

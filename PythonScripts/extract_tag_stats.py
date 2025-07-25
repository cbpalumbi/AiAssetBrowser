#!/usr/bin/env python3

import os
import json
import argparse
from collections import defaultdict, Counter

def extract_tags_from_file(path):
    try:
        with open(path, "r", encoding="utf-8") as f:
            metadata = json.load(f)
        tags = metadata.get("tags", [])
        category = metadata.get("category", "Uncategorized")
        return tags, category
    except Exception as e:
        print(f"⚠️  Skipping {path} due to error: {e}")
        return [], "Uncategorized"

def find_metadata_files(root_path):
    metadata_files = []
    for dirpath, _, filenames in os.walk(root_path):
        for filename in filenames:
            if filename.endswith(".metadata.json"):
                metadata_files.append(os.path.join(dirpath, filename))
    return metadata_files

def collect_tag_statistics(metadata_paths):
    all_tags = Counter()
    tags_by_category = defaultdict(Counter)

    for path in metadata_paths:
        tags, category = extract_tags_from_file(path)
        all_tags.update(tags)
        tags_by_category[category].update(tags)

    return all_tags, tags_by_category

def save_json(path, data):
    with open(path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4)

def main():
    parser = argparse.ArgumentParser(description="Extract project-wide and category-wide tag statistics.")
    parser.add_argument("root", help="Root folder containing asset metadata files")
    parser.add_argument("--output", default="tag_stats.json", help="Where to save the output tag stats")
    args = parser.parse_args()

    metadata_files = find_metadata_files(args.root)
    print(f"🔍 Found {len(metadata_files)} metadata files")

    all_tags, tags_by_category = collect_tag_statistics(metadata_files)

    result = {
        "existing_tags": dict(all_tags),
        "tags_by_category": {cat: dict(tags) for cat, tags in tags_by_category.items()}
    }

    save_json(args.output, result)
    print(f"✅ Saved tag stats to: {args.output}")

if __name__ == "__main__":
    main()

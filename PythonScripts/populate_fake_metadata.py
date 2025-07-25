#!/usr/bin/env python3

import os
import json
import random
import argparse
from datetime import datetime, timedelta
from faker import Faker

faker = Faker()

def load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)

def save_json(path, data):
    with open(path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=4)

import random
from datetime import datetime

def generate_fake_metadata(base_metadata, creator, last_edited_by):
    adjectives = ["ancient", "worn", "ornate", "mystic", "cracked", "heavy", "lightweight", "legendary", "bloodstained"]
    materials = ["iron", "leather", "bone", "steel", "obsidian", "silver", "gold", "wooden", "cloth"]
    types = ["helmet", "sword", "shield", "gauntlet", "duck statue", "armor piece", "amulet", "crossbow", "cloak"]
    purposes = [
        "used in arena duels",
        "worn by royal guards",
        "for ceremonial purposes",
        "enchanted by forest druids",
        "forged by mountain clans",
        "retrieved from ancient ruins",
        "found in a cursed tomb",
        "infused with dark magic",
    ]

    metadata = base_metadata.copy()
    metadata["creator"] = creator
    metadata["last_edited_by"] = last_edited_by
    metadata["category"] = random.choice(["Character", "Environment", "Prop", "Effect"])

    # Generate game-themed templated description
    metadata["description"] = (
        f"A {random.choice(adjectives)} {random.choice(materials)} {random.choice(types)} "
        f"{random.choice(purposes)}."
    )

    metadata["tags"] = random.sample(
        ["lowpoly", "animated", "VR", "scifi", "fantasy", "weapon", "urban", "rigged", "medieval", "magical", "armor"],
        k=random.randint(3, 5)
    )

    # Optional: set timestamps to now
    now = datetime.utcnow().isoformat() + "Z"
    metadata["created"] = now
    metadata["updated"] = now

    return metadata


def generate_fake_history_entries(existing_metadata, num_entries=10, base_created=None):
    entries = []
    last_metadata = existing_metadata.copy()

    # Get or create creator
    creator = existing_metadata.get("creator", "").strip()
    if not creator:
        creator = faker.name()

    if base_created is None:
        base_created = datetime.utcnow() - timedelta(days=random.randint(60, 90))

    # -- Created entry --
    created_entry = {
        "timestamp": base_created.isoformat() + "Z",
        "user": creator,
        "action": "created",
        "changed_keys": list(existing_metadata.keys()),
        "data": {**existing_metadata, "creator": creator, "last_edited_by": creator},
        "comment": "Initial asset creation"
    }
    entries.append(created_entry)
    last_metadata = created_entry["data"]

    # -- Edit entries --
    for i in range(1, num_entries):
        delta = timedelta(days=(i * (90 // num_entries)))
        timestamp = (base_created + delta).isoformat() + "Z"

        # 60% chance of creator, otherwise new user
        user = creator if random.random() < 0.6 else faker.name()

        updated_metadata = generate_fake_metadata(last_metadata, creator, user)
        changed_keys = [
            key for key in updated_metadata
            if updated_metadata[key] != last_metadata.get(key)
        ]

        entry = {
            "timestamp": timestamp,
            "user": user,
            "action": random.choice(["edit", "bulk_update", "tag_update", "metadata_cleanup"]),
            "changed_keys": changed_keys,
            "data": updated_metadata,
            "comment": faker.sentence(nb_words=8)
        }

        entries.append(entry)
        last_metadata = updated_metadata

    return entries, last_metadata, base_created

def process_asset(base_path):
    if not base_path.endswith(".metadata.json"):
        return

    history_path = base_path.replace(".metadata.json", ".history.json")
    if not os.path.exists(history_path):
        print(f"WARNING: Skipping {base_path}, no corresponding history file.")
        return

    try:
        with open(base_path, "r", encoding="utf-8") as f:
            raw_data = f.read()
            if not raw_data.strip():
                print(f"WARNING: Empty metadata file, skipping: {base_path}. Run generate_metadata_files.py first.")
                return
            metadata = json.loads(raw_data)
    except (IOError, OSError) as e:
        print(f"ERROR: Error reading metadata file {base_path}: {e}")
        return
    except json.JSONDecodeError as e:
        print(f"ERROR: JSON decode error in metadata file {base_path}: {e}")
        return

    original_created = metadata.get("created", None)
    if not original_created:
        created_time = datetime.utcnow() - timedelta(days=random.randint(60, 90))
        metadata["created"] = created_time.isoformat() + "Z"
    else:
        created_time = datetime.fromisoformat(original_created.rstrip("Z"))

    fake_entries, latest_metadata, created_time = generate_fake_history_entries(
        metadata, num_entries=random.randint(5, 12), base_created=created_time
    )
    latest_metadata["created"] = metadata["created"]
    latest_metadata["updated"] = fake_entries[-1]["timestamp"]

    save_json(base_path, latest_metadata)
    print(f"✅ Updated metadata: {base_path}")

    save_json(history_path, fake_entries)
    print(f"📝 Replaced history with {len(fake_entries)} fake entries: {history_path}")

def find_metadata_files(root_path):
    if os.path.isdir(root_path):
        return [os.path.join(root_path, f) for f in os.listdir(root_path) if f.endswith(".metadata.json")]
    elif root_path.endswith(".metadata.json"):
        return [root_path]
    else:
        return []

def main():
    parser = argparse.ArgumentParser(description="Populate metadata and history JSON files with fake but realistic data.")
    parser.add_argument("paths", nargs="+", help="Paths to metadata.json files or folders")
    args = parser.parse_args()

    all_metadata = []
    for path in args.paths:
        all_metadata.extend(find_metadata_files(path))

    if not all_metadata:
        print("⚠️  No metadata files found.")
        return

    for path in all_metadata:
        process_asset(path)

if __name__ == "__main__":
    main()

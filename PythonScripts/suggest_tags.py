#!/usr/bin/env python3

# Uses Google Gemini to suggest tags for an asset based on the existing metadata file and summary of tag usage project-wide.
    # Will look for tag usage summary by default at ./tag_stats.json. You can produce this file with extract_tag_stats.py.

# suggest_tags.py ../../AssetBrowserProjectAssets/Duck2/Duck2.metadata.json

# suggest_tags.py ../../AssetBrowserProjectAssets/Duck2/Duck2.metadata.json --tag_stats tag_stats.json

import os
import json
import argparse
import re
import google.generativeai as genai
from dotenv import load_dotenv

load_dotenv()

def load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)

def init_gemini(api_key):
    genai.configure(api_key=api_key)
    model = genai.GenerativeModel("gemini-2.5-flash-lite")
    return model

def build_prompt(asset_metadata, tag_stats):
    category = asset_metadata.get("category", "Uncategorized")
    description = asset_metadata.get("description", "")
    existing_tags = asset_metadata.get("tags", [])

    all_tags = tag_stats.get("existing_tags", {})
    category_tags = tag_stats.get("tags_by_category", {}).get(category, {})

    prompt = f"""
You are an asset tagging assistant. Your job is to assign useful, concise tags to digital 3D assets based on the description, category, and common tag usage in the project.

## Asset Info:
- Category: {category}
- Description: "{description}"
- Existing tags (if any): {existing_tags}

## Project-wide Tags:
{json.dumps(all_tags, indent=2)}

## Tags used frequently in category '{category}':
{json.dumps(category_tags, indent=2)}

Please return a list of 3 to 5 tags that:
- Are appropriate for this asset
- Prefer tags already used in the project
- Are consistent with tags commonly used for assets in the same category

Return ONLY a JSON array of strings like:
["tag1", "tag2", "tag3"]
"""

    return prompt.strip()

def suggest_tags(asset_path, tag_stats_path, api_key):
    metadata = load_json(asset_path)

    if not os.path.exists(tag_stats_path):
        print(f"WARNING: No tag stats file found at {tag_stats_path}")
        tag_stats = {}  
    else:
        tag_stats = load_json(tag_stats_path)
    
    prompt = build_prompt(metadata, tag_stats)

    model = init_gemini(api_key)
    response = model.generate_content(prompt)

    try:
        # Remove markdown formatting if present
        cleaned = re.sub(r"^```(?:json)?|```$", "", response.text.strip(), flags=re.MULTILINE)

        suggestions = json.loads(cleaned)
        print("Existing tags: ", metadata.get("tags", []))
        print("Suggested tags: ", suggestions)
        # if isinstance(suggestions, list):
        #     print(f"Suggested tags: {suggestions}")
        #     metadata["tags"] = suggestions
        #     with open(asset_path, "w", encoding="utf-8") as f:
        #         json.dump(metadata, f, indent=4)
        #     print(f"✅ Updated metadata with new tags: {asset_path}")
        # else:
        #     raise ValueError("Expected a JSON list")
    except Exception as e:
        print(f"ERROR: Failed to parse response:\n{response.text}\n\nError: {e}")

def main():
    parser = argparse.ArgumentParser(description="Suggest tags for a 3D asset using Gemini.")
    parser.add_argument("metadata_file", help="Path to the .metadata.json file for a single asset")
    parser.add_argument("--tag_stats", default="tag_stats.json", help="Path to tag statistics JSON")
    args = parser.parse_args()

    if not os.getenv("GEMINI_API_KEY"):
        print("Error: Missing Gemini API key. Set in an env var called GEMINI_API_KEY")
        return
    api_key = os.getenv("GEMINI_API_KEY")

    suggest_tags(args.metadata_file, args.tag_stats, api_key)

if __name__ == "__main__":
    main()

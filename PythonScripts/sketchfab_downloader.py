# sketchfab_downloader.py
import requests, os, time, json, zipfile, io
from tqdm import tqdm
from dotenv import load_dotenv

load_dotenv()

SEARCH_QUERY = "robot"
MAX_MODELS = 1
OUTPUT_DIR = "sketchfab_downloads"

def search_models(api_key, query, max_models):
    headers = {"Authorization": f"Token {api_key}"}
    base_url = "https://api.sketchfab.com/v3/search"
    params = {
        "type": "models",
        "q": query,
        "downloadable": "true",
        "file_format": "glb",
        "archives_flavours": "false"
    }

    print("🔍 Sending search request to Sketchfab...")

    models = []
    url = base_url
    while url and len(models) < max_models:
        # Only pass params on the first request
        resp = requests.get(url, headers=headers, params=params if url == base_url else None)
        if resp.status_code != 200:
            raise Exception(f"Search failed: {resp.status_code}")
        data = resp.json()

        for model in data['results']:
            if model.get('isDownloadable') and model.get('license') in ('CC0', 'CC-BY'):
                models.append(model)
                print(f"✔️  Added: {model.get('name', 'Unnamed')}")
                if len(models) >= max_models:
                    break

        url = data.get('next')
        params = None  # Avoid re-passing the same query params on paginated URLs

    print(f"✅ Found {len(models)} valid downloadable models.")
    return models


def download_and_extract_glb(api_key, model):
    uid = model['uid']
    title = model['name'].replace(" ", "_")
    print(f"🔽 Downloading: {title}")
    
    # Get download URL
    headers = {"Authorization": f"Token {api_key}"}
    dl_url = f"https://api.sketchfab.com/v3/models/{uid}/download"
    dl_resp = requests.get(dl_url, headers=headers)
    if dl_resp.status_code != 200:
        print(f"❌ Failed to get download link for {uid}")
        return

    dl_info = dl_resp.json()
    gltf_url = dl_info.get('gltf', {}).get('url')
    if not gltf_url:
        print(f"⚠️ No glTF archive found for {uid}")
        return

    # Download ZIP archive
    zip_resp = requests.get(gltf_url)
    if zip_resp.status_code != 200:
        print(f"❌ Failed to download glTF ZIP for {uid}")
        return

    # Extract archive
    extract_dir = os.path.join(OUTPUT_DIR, f"{title}_{uid}")
    os.makedirs(extract_dir, exist_ok=True)
    with zipfile.ZipFile(io.BytesIO(zip_resp.content)) as zf:
        zf.extractall(path=extract_dir)

    # Save metadata
    meta_path = os.path.join(extract_dir, "metadata.json")
    with open(meta_path, "w") as f:
        json.dump({
            "uid": uid,
            "title": model["name"],
            "description": model.get("description"),
            "tags": [t["name"] for t in model.get("tags", [])],
            "author": model["user"]["displayName"],
            "license": model["license"]
        }, f, indent=2)

    time.sleep(1)  # Rate limit

def main():
    api_key = os.getenv("SKETCHFAB_API_KEY")
    if not api_key:
        print("❌ Error: Missing Sketchfab API key. Set it in an env var called SKETCHFAB_API_KEY")
        return

    os.makedirs(OUTPUT_DIR, exist_ok=True)

    print("🔍 Searching Sketchfab...")
    models = search_models(api_key, SEARCH_QUERY, MAX_MODELS)
    print(f"✅ Found {len(models)} downloadable models.")
    return
    for model in tqdm(models):
        try:
            download_and_extract_glb(api_key, model)
        except Exception as e:
            print(f"❌ Error downloading model {model['uid']}: {e}")

if __name__ == "__main__":
    main()

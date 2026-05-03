#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
FRAMES=/tmp/cg_frames
mkdir -p "$REPO_ROOT/docs"

make_gif() {
    local binary="$1" output="$2"
    echo "[+] recording $(basename $output .gif)..."
    rm -rf "$FRAMES" && mkdir -p "$FRAMES"
    "$binary" --record "$FRAMES"
    ffmpeg -y \
        -framerate 30 \
        -pattern_type glob -i "$FRAMES/frame_*.ppm" \
        -vf "fps=20,scale=720:-1:flags=lanczos,split[s0][s1];[s0]palettegen=max_colors=128[p];[s1][p]paletteuse=dither=bayer" \
        "$output" 2>/dev/null
    echo "[OK] wrote $output ($(du -sh $output | cut -f1))"
}

BUILD="$REPO_ROOT/build"
for b in triangle-motions house-transforms; do
    [[ ! -f "$BUILD/$b" ]] && { echo "Binary not found: $BUILD/$b — build first."; exit 1; }
done

make_gif "$BUILD/triangle-motions" "$REPO_ROOT/docs/triangles.gif"
make_gif "$BUILD/house-transforms" "$REPO_ROOT/docs/transforms.gif"

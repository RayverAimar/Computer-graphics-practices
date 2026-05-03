#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BINARY="$REPO_ROOT/build/triangle-motions"
FRAMES_DIR=/tmp/triangle_frames
OUT_GIF="$REPO_ROOT/docs/demo.gif"

if [[ ! -f "$BINARY" ]]; then
    echo "Binary not found: $BINARY"
    echo "Run: cmake -B build && cmake --build build -j"
    exit 1
fi

echo "Rendering frames..."
rm -rf "$FRAMES_DIR" && mkdir -p "$FRAMES_DIR"
"$BINARY" --record "$FRAMES_DIR"

echo "Building GIF..."
mkdir -p "$(dirname "$OUT_GIF")"
ffmpeg -y \
    -framerate 30 \
    -pattern_type glob -i "$FRAMES_DIR/frame_*.ppm" \
    -vf "fps=20,scale=720:-1:flags=lanczos,split[s0][s1];[s0]palettegen=max_colors=128[p];[s1][p]paletteuse=dither=bayer" \
    "$OUT_GIF"

echo "Done → $OUT_GIF"

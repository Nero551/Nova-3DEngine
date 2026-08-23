#!/bin/sh
set -e

ROOT="$(git rev-parse --show-toplevel)"

echo "🔨 Building..."

export VCPKG_DISABLE_METRICS=1

cmake --preset default -S "$ROOT"

cmake --build --preset default -j4

echo "✅ Built."
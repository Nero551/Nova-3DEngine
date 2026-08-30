#!/bin/sh
set -e

ROOT="$(git rev-parse --show-toplevel)"

echo "📦 Checking Submodules..."

    echo "📥 Initializing Submodules..."

    git -C "$ROOT" submodule update --init --recursive

    echo "🔧 Bootstrapping vcpkg..."
    "$VCPKG/bootstrap-vcpkg.sh"
fi

echo "✅ Submodules Initialized"
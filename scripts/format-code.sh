#!/usr/bin/sh

ROOT="$(git rev-parse --show-toplevel)"

files=$(find "$ROOT/src" -type f \( -name "*.cpp" -o -name "*.hpp" \))
count=$(printf '%s\n' "$files" | wc -l)

echo "🎨 Formatting $count C++ files..."

find "$ROOT/src" -type f \( -name "*.cpp" -o -name "*.hpp" \) \
    -exec clang-format -i {} +

echo "✅ Formatted $count C++ files."
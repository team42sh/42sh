#!/bin/bash
set -e

TAG="$1"

if [[ -z "$TAG" ]]; then
  echo "❌ Usage: $0 v0.0.1"
  exit 1
fi

echo "👉 Creating isolated tag $TAG without touching main"

CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
TEMP_BRANCH="temp-tag-$TAG"

# Create a temp branch from current HEAD
git checkout -b "$TEMP_BRANCH"

# Create .version
echo "$TAG" > .version
git add -f .version
git commit -m "Add .version for $TAG"

# Tag it
git tag "$TAG"

# Push only the tag (not the temp branch)
git push origin refs/tags/"$TAG"

# Cleanup
git checkout "$CURRENT_BRANCH"
git branch -D "$TEMP_BRANCH"

echo "✅ Tag $TAG created!"
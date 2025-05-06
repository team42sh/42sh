#!/bin/bash

# Configuration
VERSION_FILE="./.version"
HEADER_FILE="./include/build_infos.h"
ALLOW_AUTO_UPDATE=0 # No auto-update (0) or auto-update (1)
SHOW_UPDATES=1 # Don't show new releases (0) or show updates (1)

# Determine Version
VERSION="UNKWN"
if [ -f "$VERSION_FILE" ]; then
    VERSION=$(tr -d '\r\n' < "$VERSION_FILE")
else
    echo "Warning: $VERSION_FILE not found, using \"$VERSION\""
fi

# Check for Header File
if [ ! -f "$HEADER_FILE" ]; then
    echo "Error: $HEADER_FILE not found."
    exit 1
fi

# Perform In-place Edits
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    sed -i '' \
        -e "s/^    #define VERSION \".*\"/    #define VERSION \"$VERSION\"/" \
        -e "s/^    #define ALLOW_AUTO_UPDATE [01]/    #define ALLOW_AUTO_UPDATE $ALLOW_AUTO_UPDATE/" \
        -e "s/^    #define SHOW_UPDATES [01]/    #define SHOW_UPDATES $SHOW_UPDATES/" \
        "$HEADER_FILE"
else
    # Linux and others
    sed -i \
        -e "s/^    #define VERSION \".*\"/    #define VERSION \"$VERSION\"/" \
        -e "s/^    #define ALLOW_AUTO_UPDATE [01]/    #define ALLOW_AUTO_UPDATE $ALLOW_AUTO_UPDATE/" \
        -e "s/^    #define SHOW_UPDATES [01]/    #define SHOW_UPDATES $SHOW_UPDATES/" \
        "$HEADER_FILE"
fi

echo "Updated $HEADER_FILE:"
echo "  - VERSION = $VERSION"
echo "  - ALLOW_AUTO_UPDATE = $ALLOW_AUTO_UPDATE"
echo "  - SHOW_UPDATES = $SHOW_UPDATES"

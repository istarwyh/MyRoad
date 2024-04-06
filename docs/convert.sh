#!/bin/bash

# Convert Obsidian files to HTML
obsidianhtml convert -i config.yaml

# Navigate to output/html directory
cd output/html || exit
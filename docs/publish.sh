#!/bin/bash

git commit -am "modify"

git push

# Convert Obsidian files to HTML
obsidianhtml convert -i config.yaml

# Navigate to output/html directory
cd output/html || exit

# Initialize a Git repository
#git init

# Create and switch to the gh-pages branch
git checkout -b gh-pages

# Add all files to the staging area
git add .

# Commit the changes
git commit -am "gh-pages"

# Add the remote origin
git remote add origin https://github.com/istarwyh/Myroad.git

# Check the current remote repositories
#git remote -v

# Force push the changes to the gh-pages branch
git push -f origin gh-pages
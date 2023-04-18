#!/bin/sh
# Check if there are any changes in the current branch
if [[ $(git status --porcelain) ]]; then
  echo "**************************************************************************************************"
  echo "Error: There are changes in the current branch that need to be resolved before switching branches."
  echo "**************************************************************************************************"
  echo "ABORTING doc-push."
  exit 1
fi

# If there are no changes, checkout the specified branch
git checkout gh-pages
git merge main -m merge-files-from-main
# Check if there were any merge conflicts
if [ $? -ne 0 ]; then
  echo "***********************************************************************************"
  echo "Merge conflicts found going from main to gh-pages branch. Stopping mid-stream here."
  echo "***********************************************************************************"
  echo "ABORTING doc-push."
  echo "NOTE: You are being left in the gh-pages branch on purpose to let you resolve this."
  exit 2
fi
doxygen
rm -rf gh-pages
mkdir gh-pages
mv docs/html/* gh-pages/
git add gh-pages
git commit -m doc-update
git push origin
git checkout main
echo "Doc update complete. New docs should be published online momentarily..."


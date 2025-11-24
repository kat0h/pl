#!/bin/bash

MERGED_DIR="merged"
REPOS=(
  "awk1:master"
  "awk2:master"
  "continuation:master"
  "lalr1:master"
  "lps-vm:master"
  "re:main"
  "scheme:master"
)

gio trash "$MERGED_DIR"
mkdir -p "$MERGED_DIR"
cd "$MERGED_DIR"

git init
git commit --allow-empty -m "Initial commit"

for REPO in "${REPOS[@]}"
do
  IFS=: read DIR BRANCH <<< "$REPO"
  echo "$DIR" "$BRANCH"
  git remote add "$DIR" "../$DIR"
  git fetch --all --tags
  git checkout "$DIR/$BRANCH"
  TMP_BRANCH="$DIR-$BRANCH"
  git checkout -b "$TMP_BRANCH"
  git filter-repo \
    --path-rename ":$DIR/" \
    --message-callback "return b\"[$DIR] \" + message" \
    --refs "$TMP_BRANCH" --force
  git checkout master
  git merge "$TMP_BRANCH" --no-ff --allow-unrelated-histories --no-edit
done

#!/bin/bash
git add .
read -p "Enter commit message [update]: " msg
msg="${msg:-update}"
git commit -m "$msg"
git push origin HEAD

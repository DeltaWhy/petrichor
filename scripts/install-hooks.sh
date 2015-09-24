#!/bin/sh
cd `dirname $0`
git config core.autocrlf input
ln -srf ./pre-commit.sh ../.git/hooks/pre-commit

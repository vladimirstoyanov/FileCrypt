#!/bin/sh

#$1 - comment
#$2 - path

if [ $# -ne 2 ]
then
  echo "Wrong input! please use the following input: "
        echo "1 arg - comment"
        echo "2 arg - path"
  exit 1
fi

cd $2

echo "git add --all"
git add --all

echo "git commit -m $1"
git commit -m "$1"

echo "git push origin"
git push origin

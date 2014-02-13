#!/bin/sh
 
git filter-branch -f --env-filter '
 
export GIT_AUTHOR_NAME="liang8305"
export GIT_AUTHOR_EMAIL="418094911@qq.com"
export GIT_COMMITTER_NAME="liang8305"
export GIT_COMMITTER_EMAIL="418094911@qq.com"
'


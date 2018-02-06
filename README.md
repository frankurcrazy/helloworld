Setup Git
---------

# user's info will be stored in ~/.gitconfig
git config --global user.name "..."
git config --global user.email "..."

# show user's info
git config --list

# add color on UI
git config --global color.ui true


Initial a repository
--------------------

# create a new local repository
mkdir foo_repos
cd ./foo_repos
git init


Clone a repository
------------------

# create a working copy of a repository
cd ~/bar_repos
git clone http://xxx.xxx.xxx/bar.git


Sync. to the remote
-------------------

# fetch and merge changes on the remote repository
git pull origin master


Update to the remote
--------------------

# send changes to the remote repository
git push origin master


Commands
--------

#  list the changed files
git status

# view all the merge conflicts
git diff
git diff HEAD

# add files to staging
git add -i file_name
git add *

# commit changes to head
git commit -m "comments"
git commit -a

# show the commit history
git log

# show the information of a remote repository
git remote show origin

# add a tag
git tag -l
git tag -a NAME -m DESCRIPTION
git push origin NAME

# fetch the lastest history
git reset --hard "tag name"


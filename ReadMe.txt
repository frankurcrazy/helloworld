[ Setup git ]

# User's info will be stored in ~/.gitconfig
git config --global user.name "..."
git config --global user.email "..."

# Show user's info
git config --list

# Add color on UI
git config --global color.ui true


[ SSH key ]

# Generate public/private key pair
cd ~/.ssh
ssh-keygen

# Copy the public key
cat id_rsa.pub

# add the key to GitHub account
#  -> Settings
#      -> SSH and GPG keys

# Convert the private key to .ppk format
puttygen id_rsa -o private_key_name.ppk


[ Initial a repository ]

# Create a new local repository
mkdir foo_repos
cd ./foo_repos
git init


[ Clone a repository ]

# Create a working copy of a repository
cd ~/bar_repos
git clone http://xxx.xxx.xxx/bar.git


[ Sync. to the remote ]

# Fetch and merge changes on the remote repository
git pull origin master


[ Update to the remote ]

# Send changes to the remote repository
git push origin master


[ Commands ]

# List the changed files
git status

# View all the merge conflicts
git diff
git diff HEAD

# Add files to staging
git add -i file_name
git add *

# Commit changes to head
git commit -m "comments"
git commit -a -m "comments"

# Show the commit history
git log

# Show the information of a remote repository
git remote show origin

# Add a tag
git tag -l
git tag -a tag_name -m "comments"
git push origin tag_name

# Fetch the lastest history
git reset --hard tag_name



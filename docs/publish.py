import os
from git import Repo
from git.exc import GitCommandError

path = 'output/html'
repo_url = 'https://github.com/istarwyh/Myroad.git'
# git init
# git checkout -b gh-pages
# git add .
# git commit -am "gh-pages"
# git remote -v
# git remote add origin https://github.com/istarwyh/Myroad.git
# git push -f origin gh-page
print(f"\033[32mStart publishing to your git repo: {repo_url}\nPlease wait...\033[0m")

try:
    repo = Repo.init(path)
    repo.create_remote('origin', url=repo_url)

    # Add all files in the 'output/html' directory to the staging area
    for file in os.listdir(path):
        repo.git.add(file)

    repo.git.commit('-m', 'automatically publish')

    try:
        repo.git.checkout('b', 'gh-pages')
        print('Checked out to branch gh-pages')
    except GitCommandError:
        try:
            repo.git.checkout('gh-pages')
        except GitCommandError:
            repo.git.checkout('HEAD', b='gh-pages')
            print('Created and checked out to branch gh-pages')

    repo.git.push('-f', 'origin', 'gh-pages')

    try:
        repo.git.checkout('b', 'master')
        print('\033[32mFinished publishing, back to branch master.\033[0m')
    except GitCommandError:
        repo.git.checkout('master')

except Exception as error:
    print(f"\033[31m{error}\033[0m")
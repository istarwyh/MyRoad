```shell
function cmt(){
        git commit -am "$1"
}

function cr(){
        b=`git branch --show-current`
        git push origin head:refs/for/${b} -o reviewer=11
}

function amend(){
        git add .
        git commit --amend
}

function cur(){
        git branch --show-current
}

function pull(){
		git pull --rebase
        b=`git branch --show-current`
        git checkout master
        git pull
        git checkout ${b}
        git merge master
}
```
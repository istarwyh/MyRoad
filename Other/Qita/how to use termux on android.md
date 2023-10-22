https://github.com/termux/termux-app#installation
termux-setup-storage
ssh github repository
pkg install vim
pkg install git

vim ~/.bashrc  
alias c="cd ~/storage/downloads/xiaohui/Myroad/docs/Vnote"
alias p="git commit -am 'from phone' && git push"
source ~/.bashrc
git config --global --add safe.directory /storage/emulated/0/Download/Xiaohui/Myroad
git config --global user.email "yihui-wang@qq.com"
git config --global user.name "istarwyh"
检查是否生效git config -l

#Command
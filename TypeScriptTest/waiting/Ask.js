
// 返回函数题
// 一个返回函数f(),但不会调用它,打印function f(){return scope;}
// 一个返回函数名并调用方法,打印局部变量scope的值"local scope"
var scope = "global scope";
function checkscope() {
    var scope = "local scope";
    // 内部函数嵌套
    function f() {
        return scope;
    }
    return f();
}
checkscope();
console.log(checkscope());

var scope = "global scope";
function checkscope() {
    var scope = "local scope";
    function f() {
        return scope;
    }
    return f;
}
checkscope()();
console.log(checkscope()());

// 处理递归导致堆栈溢出
var list = readlist();// todo:可能返回一个很大的列表
var nextItem = function () {
    var item = list.pop();
    if (item) {
        // 处理下一列表中的元素
        nextItem();
    }
};
nextItem();
// 减少调用栈的次数,采用“循环”换掉“递归”
// 1. 蹦床函数
function trampoline(f) {
    while (f && f instanceof Function) {
        f = f();
        return f;
    }
}
var nextItem = trampoline(nextItem());
// 2. 尾递归
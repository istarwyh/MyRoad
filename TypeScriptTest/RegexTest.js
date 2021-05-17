var str = "<d-button bsStyle=\"primary\">the book</d-button>";
var patt2 = /((d-[a-zA-Z0-9]*)\b)+(?=>)/g;
document.write(str.match(patt2) + "<br>");

var str3 = "<span class = \"read-count\">the problem:12345</span>";
var patt3 = /.+(?=<\/span>)/g;
document.write(str3.match(patt3) + "<br>");// document.write()不会把样式打出来
window.alert(str3.match(patt3));// 网页弹出,也可以把真正的匹配打出来
console.log(str3.match(patt3));// 网页上不会显示,在控制台上打印,这会把真正的所有的匹配都打出来
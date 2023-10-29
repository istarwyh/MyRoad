## 代码规范-设计模式
### 1. Visitor
visitor模式相当于将原本需要在多态子类中完成的方法放到统一的Visitor类中
### 2. SRP(Single Responsibility Program)

```javascript
const transform1 = (str) => {
    if(typeof str === "string"){
        return `${str.toUpperCase()}!`
    }
    return "Not a string";
}

const transform2 = (str) => {
    if(typeof str === "string"){
        return `${str.toLowerCase()}!`
    }
    return "Not a string";
}

```
有时候你觉得你的程序没有办法拆分成单一职责的,那有可能是语法工具还不够多.

有时候你觉得业务复杂，那是因为有太多业务逻辑耦合在了一起。我们需要不停地把业务逻辑分开，把公共逻辑抽象出来并合并。


```javascript
const { flow } = require("lodash/fp");
const toUpper = (str) => str.toUpperCase();
const toLower = (str) => str.toLowerCase();
const exclaim = (str) => `${str}!`;
const isString = (str) => (typeof str === "string" ? str : "Not a string");


const transform1 = flow(isString, toUpper, exclaim);
const transform2 = flow(isString, toLower, exclaim);


transform1("hello world"); // "HELLO WORLD !"
transform2("HELLO WORLD"); // "hello world !"
```
### 3. SPOT(Single Point of Truth)/DRY(Don't Repeat yourself)
尽量不要有重复的代码有时候会让代码难懂，而保证自己写的代码在需要的时候只需要改动一处，是更为实际的原则。
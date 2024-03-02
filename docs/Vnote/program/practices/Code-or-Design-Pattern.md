## 原则

### Brief
每个方法、函数都应该简短，原则上不应该超过 7 行。
###  SRP(Single Responsibility Program)

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
### SPOT(Single Point of Truth)/DRY(Don't Repeat yourself)
尽量不要有重复的代码。有时候一味追求不重复会让代码难懂，而保证自己写的代码在需要的时候只需要改动一处，是更为实际的原则。

### DIP(Dependency Inversive Principle)
DIP 强调模块之间不应该直接依赖，而应该通过抽象产生依赖。所谓抽象，一般就是接口。这样就可以做到高层模块不因底层模块而变化，从而降低系统的耦合度，使得使用这个原则的模块可以自闭环。

## 思想
### AOP
面向切面封装识别功能的收益，埋点、报表、同比环比等等。
面向切面做缓存

## 设计模式
### Visitor
visitor模式相当于将原本需要在多态子类中完成的方法放到统一的Visitor类中
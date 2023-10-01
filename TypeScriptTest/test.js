/**
 * 从test.ts转化来的
 */
var JsCar = /** @class */ (function () {
    function TsCar() {
        var _this = this;
        document.getElementById('carForwardButton').onclick = function (event) {
            _this.moveForward();
        };
    }
    TsCar.prototype.moveForward = function () {
        console.log('--------------car2 moved forward----------------');
    };
    return TsCar;
}());
;
new JsCar();
// ----------------------------------------------------------------------------------------

class Car {
    constructor() {
        this.turnOn = () => {
            document.getElementById('carForwardButton').onclick = (event) => {
                console.log('--------------car3 moved forward----------------')
            };
            return this;
        };
        // return {};否则就是返回空对象,this()将指向这个
    }
};
// 调用turnOn()时,this指向调用它的实例,即car
new Car().turnOn();

// ----------------------------------------------------------------------------------------
function showReapeat() {
    var str = 'JAVASCRIPT,'
    document.getElementById('showrepeat').innerHTML = str.repeat(3);
}

// ----------------------------------------------------------------------------------------
var data = [
    '2017-05-12',
    '2020-07-24',
    '2018-06-01',
    '2019-01-01'
]
data.sort((a, b) => {
    return a < b ? 1 : -1
});
console.log(data);

// ----------------------------------------------------------------------------------------
// 实现对String内置的repeat方法!!!可以改String的内置方法?!!原型链...
String.prototype.repeatify = String.prototype.repeatify || function (times) {
    let str = '-->';
    while (times > 0) {
        str += this;
        times--;
    }
    return str;
}
console.log('Change'.repeatify(3));
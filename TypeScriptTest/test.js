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
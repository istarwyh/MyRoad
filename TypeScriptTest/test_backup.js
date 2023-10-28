class Car {
    constructor() {
        this.turnOn = () => {
            document.getElementById('carForwardButton').onclick = (event) => {
                console.log('--------------car moved forward----------------')
            };
            return this;
        };
        // return {};否则就是返回空对象,this()将指向这个
    }
};
// 调用turnOn()时,this指向调用它的实例,即car,接着turn()内部再调用this.moveforward()时,this指向?所以去掉this
new Car().turnOn();
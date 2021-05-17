class TsCar {
    constructor() {
        document.getElementById('carForwardButton').onclick = (event) => {
            this.moveForward
            this.moveForward()
        };
    }
    moveForward(): void {
        console.log('--------------car moved forward----------------')
    }
};
// 调用turnOn()时,this指向调用它的实例,即car,接着turn()内部再调用this.moveforward()时,this指向?所以去掉this
new TsCar();
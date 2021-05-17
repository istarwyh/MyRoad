// 实现jsonObj转url queryString
function convertToParamStr(paramObj) {
    const requestParam = [];
    for (let attr in paramObj) {
        requestParam.push(`${attr}=${filter(paramObj[attr])}`);
    }
    return '?' + requestParam.join('&');
};
function filter(str) {
    str += '';
    str = str.replace(/%/g, '%25');
    str = str.replace(/\+/g, '%2B');
    str = str.replace(/ /g, '%20');
    str = str.replace(/\//g, '%2F');
    str = str.replace(/\?/g, '%3F');
    str = str.replace(/&/g, '%26');
    str = str.replace(/\=/g, '%3D');
    str = str.replace(/#/g, '%23');
    // str = str.replace(/%/g, '%25').replace(/\+/g, '%2B').replace(/ /g, '%20').replace(/\//g, '%2F').replace(/\//g, '%2F').replace(/\?/g, '%3F').replace(/&/g, '%26').replace(/\=/g, '%3D').replace(/#/g, '%23');
    return str;
}

let obj = {
    id: 1,
    name: '小明',
    age: 20,
    skills: ['HTML','CSS','JavaScript']
}
console.log(convertToParamStr(obj)) // id=1&name=小明&age=20
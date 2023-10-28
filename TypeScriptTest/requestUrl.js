const http = require('http')
const https = require('https')
var options1 = {
    hostname: 'nodejs.cn',
    port: 8080,
    path: '/learn/get-http-request-body-data-using-nodejs',
    method: 'GET'
};

var op2={
    hostname: 'gitee.com',
    port: 443,
    path: '/istarwyh/images/raw/master/1617025579_20210329214515706_12235.gif',
    method: 'GET'
};

var req = https.request(op2, function (res) {
    console.log("\u72B6\u6001\u7801: " + res.statusCode);
    res.on('data', function (data) {
        process.stdout.write(data);
    });
});
req.on('error', function (error) {
    console.error(error);
});
req.end();
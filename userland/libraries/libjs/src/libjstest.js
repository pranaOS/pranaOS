var exec = require('child_process').exec;

function libjstest() {
    exec('find ../libjs', function (error, stdout, stderr) {
        console.log('kernel stdout: ' + stdout);
        console.log('kernel stderr: ' + stderr);
        if (error !== null) {
            console.log('error: ' + error);
        }
    });
}
var exec = require('child_process').exec;

exec('find kernel', function(error, stdout, stderr) {
    console.log('kernel stdout: ' + stdout);
    console.log('kernel stderr: ' + stderr);
    if (error !== null) {
        console.log('error: ' + error);
    }
});

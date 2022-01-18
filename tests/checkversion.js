var exec = require('child_process').exec;

function checkversion() {
    exec('i686-elf-gcc --version', function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
            console.log('error: ' + error);
        }
    });

    exec('nasm --version', function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
            console.log('error: ' + error);
        }
    });

    exec('make --version', function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
            console.log('error: ' + error);
        }
    });

    exec('which ld', function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
            console.log('error: ' + error);
        }
    });

}

function main() {
    console.log('Running check version');
    checkversion();
}

main();
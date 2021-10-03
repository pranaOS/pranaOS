const PIPE_NAME = "pranaos"
const PIPE_PATH = "\\\\.\\pipe\\" + PIPE_NAME

const allowed = 'qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890`~!@#$%^&*()_+-=[]{}\\|;\'",./<>?:\t\r\n '

var client = require('net').connect(PIPE_PATH, function() {})

const domain = require('domain').create()
domain.on('error', function(err) {
	console.error(err)
})

function reconnect() {
	client.removeAllListeners()

	client.on('data', function(data) {
		let s = []
		for (let i = 0; i < data.length; i++) {
			if (data[i] == 0) continue
			const char = String.fromCharCode(data[i])
			if (allowed.indexOf(char) != -1)
				s.push(char)
		}
		process.stdout.write(s.join(''), 'ascii')
	})

	client.on('end', function() {})
	client.on('close', function() {
		reconnect()
	})

	client.on('error', function() {
		reconnect()
	})

	setTimeout(() => {
		client.connect(PIPE_PATH)
	}, 1)
}

reconnect()
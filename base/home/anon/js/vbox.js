const vmname = 'pranaOS'
const cmd = '"/Applications/VirtualBox.app/virtualbox" --nologo startvm ' + vmname

require('child_process').exec(command)

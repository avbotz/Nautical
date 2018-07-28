#!/usr/bin/python
import sys
sys.path.append('/usr/local/lib/python2.7/site-packages')
import pexpect
FILE = "images/"
REMOTE = "/home/user/Aquastorm/logs/modeling_img.png"
USER="user"
HOST="192.168.2.3"
PASS="marlin123"
COMMAND="scp -oPubKeyAuthentication=no %s@%s:%s %s" % (USER,HOST,REMOTE,FILE)

child = pexpect.spawn(COMMAND)
child.expect('password:')
child.sendline(PASS)
child.expect(pexpect.EOF)
#print child.before


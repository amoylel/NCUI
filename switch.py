#encoding=utf-8
# -*- coding: UTF-8 -*-

import urllib 
import urllib2 
import os
import tarfile
import _winreg
import shutil
import sys
import re
import fnmatch
import os.path
from optparse import OptionParser
import posixpath
import string
import getopt
import time
import node_cef_util

reload(sys)
sys.setdefaultencoding('utf8') # 允许中文字符串

print sys.argv


cef_version="" #cef 版本号
node_version="" #node版本号
rebuild = False #是否需要重新编译
props = './Microsoft.Cpp.Common.user.props' #配置文件
try:
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["cef=","node="])
except getopt.GetoptError:
	print 'cef, node, rebuild'
	sys.exit(2)
for opt, arg in opts:
	if opt == '-h':
		print 'cef, node, rebuild'
		sys.exit()
	elif opt == ("--cef"):
		cef_version = arg
	elif opt == ("--node"):
		node_version = arg

print "cef: " + cef_version		
print "node: " + node_version 


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
switch_only = False #只切换分支

props = './Microsoft.Cpp.Common.user.props' #配置文件
ncui_lib_dir = "./../NCUI-Library/" #库文件输出目录

try:
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["cef=","node=","rebuild", "switch"])
except getopt.GetoptError:
	print 'cef, node, rebuild, switch'
	sys.exit(2)
for opt, arg in opts:
	if opt == '-h':
		print 'cef, node, rebuild, switch'
		sys.exit()
	elif opt == ("--cef"):
		cef_version = arg
	elif opt == ("--node"):
		node_version = arg
	elif opt == ("--rebuild"):
		rebuild = True
	elif opt == ("--switch"):
		switch_only = True
print "cef: " + cef_version		
print "node: " + node_version
print "rebuild: " , rebuild
print "switch_only: " , switch_only


#使用内置
if cef_version == '' and node_version == '':
	default_command = '\"./resources/7z.exe \"' + " x ./NCUI-Library/NCUI-Library.7z " + " -aoa -o" + ncui_lib_dir
	print default_command
	os.system(default_command)
	
	#修改config	
	print "modify cef config"
	config = "\t\t<LibCefFolder>$(LibraryFolder)" 
	config += "cef_binary_3.2526.1373.gb660893_windows32" 
	config += "/</LibCefFolder>\n"
	print config
	node_cef_util.modifyProps(props, "</LibCefFolder>", config)
	
	#修改config	
	print "modify node config"
	config = "\t\t<NodeGypFolder>$(LibraryFolder)" 
	config += "node-v4.7.1" 
	config += "/</NodeGypFolder>\n"
	print config
	node_cef_util.modifyProps(props, "</NodeGypFolder>", config)
	
else: 
	cef_command = "python cef.py --version " + cef_version + " --cmake --decompress --build --copy --download "
	node_command = "python node.py --version " + node_version + " --decompress --build --copy --download "
 
	if switch_only == True:
		cef_command = "python cef.py --copy --version " + cef_version
		node_command = "python node.py --copy --version  " + node_version

	if rebuild == True:
		cef_command += " --rebuild"
		node_command += " --rebuild"
		
	if cef_version <> '':
		print "build cef with command: " + cef_command
		os.system(cef_command) 
	else:
		print "empty cef version skip build."

	if node_version <> '':
		print "build node with command: " + node_command
		os.system(node_command) 
	else:
		print "empty node version skip build."
	


	
	
cefPath = ncui_lib_dir +  node_cef_util.getLibCefDir(props) + "/"
nodePath = ncui_lib_dir + node_cef_util.getLibNodeDir(props) + "/"
srcPath = "./"

print "cefPath: "+ cefPath
print "nodePath: "+ nodePath

dstDir = "../NCUI-Out/Binary.UR32/"
#删除目录
if os.path.exists(dstDir) == True:
	shutil.rmtree(dstDir)
node_cef_util.copyCefResouces(cefPath, dstDir)
node_cef_util.copyNodeResources(nodePath, dstDir, True)
node_cef_util.copyNCUIResources(srcPath + "resources", dstDir, True, True, True, True, True) 
node_cef_util.copyNCUIWebZipResources(srcPath + "resources", dstDir) 
dstDir = "../NCUI-Out/Binary.UD32/"
#删除目录
if os.path.exists(dstDir) == True:
	shutil.rmtree(dstDir)
node_cef_util.copyCefResouces(cefPath, dstDir)
node_cef_util.copyNodeResources(nodePath, dstDir, True)
node_cef_util.copyNCUIResources(srcPath + "resources", dstDir, True, True, True, True, True) 
node_cef_util.copyNCUIWebZipResources(srcPath + "resources", dstDir) 

#encoding=utf-8
# -*- coding: UTF-8 -*-

import sys
import re
import os
import fnmatch
import os.path
from optparse import OptionParser
import posixpath
import string
import time
import tarfile
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
ncui_version_file = "./libNCUI/include/version.h" 

node = False 		#默认不使用nodejs
ppapi = False 		#默认不使用PPAPI Falsh
npapi = False		#默认不使用NPNPI Falsh
version = ""			#当前版本号
example = False 		#是否拷贝Demo
vc2015 = False			#是否拷贝vc2015运行库
web = False  			#是否拷贝web目录 
devtools = False		#是否拷贝devtools_resources.pak
try:
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["node","ppapi","npapi", "vc2015", "version", "example", "web", "devtools"])
except getopt.GetoptError:
	print 'node, ppapi, apapi, vc2015, version, example, web, devtools'
	sys.exit(2)
for opt, arg in opts:
	if opt == '-h':
		print 'node, flash'
		sys.exit()
	elif opt == ("--node"):
		node = True
	elif opt == ("--ppapi"):
		ppapi = True
	elif opt == ("--npapi"):
		npapi = True
	elif opt == ("--example"):
		example = True
	elif opt == ("--vc2015"):
		vc2015 = True
	elif opt == ("--web"):
		web = True
	elif opt == ("--devtools"):
		devtools = True
	elif opt == ("--version"):
		version = arg
		

if version == '':
	version = node_cef_util.getNCUIVersion(ncui_version_file)
	
print "version" + version
print "node: " , node		
print "ppapi: ", ppapi
print "npapi: ", npapi
print "vc2015: ", vc2015
print "web: ", web
print "devtools: ", devtools
print "example: ", example
  
cefPath = ncui_lib_dir +  node_cef_util.getLibCefDir(props) + "/"
nodePath = ncui_lib_dir + node_cef_util.getLibNodeDir(props) + "/"
outPath = "./../NCUI-Out/Install/" #输出目录
buildPath = "./../NCUI-Out/Binary.UR32/" #程序编译目录
srcPath = "./"

version = "NCUI-v"+ version
outFileTempDir = outPath + version + "/"
outFileNamePath = outPath + version +'.7z'
print "cefPath: "+ cefPath
print "nodePath: "+ nodePath


if os.path.exists(cefPath) == False:
	print cefPath + "not exists."
	exit()
	
if os.path.exists(nodePath) == False and node == True:
	print nodePath + "not exists."
	exit()
	
if os.path.exists(outFileTempDir) == True: 
	shutil.rmtree(outFileTempDir)
  
if os.path.exists(buildPath) == False:
	print buildPath + "not exists."
	exit()
	
if os.path.exists(outFileNamePath):
	os.remove(outFileNamePath)
	
node_cef_util.createDir(outPath)
node_cef_util.createDir(outFileTempDir)

if node:
	node_cef_util.copyNodeResources(nodePath, outFileTempDir, example)
	
node_cef_util.copyCefResouces(cefPath, outFileTempDir)
node_cef_util.copyNCUIResources(srcPath + "resources/", outFileTempDir, True, ppapi, npapi,  web, vc2015)  
node_cef_util.copyNCUIWebZipResources(srcPath + "resources/", outFileTempDir)
node_cef_util.copyNCUIBuildResources(buildPath, outFileTempDir, node, example)


devtools_resources = outFileTempDir + '/devtools_resources.pak'
if devtools == False:
	if os.path.exists(devtools_resources) == True:
		os.remove(devtools_resources)

compress_command = '\"./resources/7z.exe \"'
compress_command += ' a -t7Z -r -mx9 -m0=LZMA:d=27 -ms=2048m -mmt=2 '
compress_command += outFileNamePath + ' ' 
compress_command += outFileTempDir
print compress_command
os.system(compress_command)


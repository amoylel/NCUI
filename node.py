#!/usr/bin/env python
#coding=utf-8

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
	

url = '' #文件地址
decompress = False #是否压缩文件 
outputPath = './../NCUI-Library/'	#输出目录
clean = False
downloads = './../NCUI-Downloads/' #文件下载保存路径
tmp = downloads + 'tmp/'  #临时文件目录
tmpFolder = '' #文件解压后的保存路径
cmake = False
build = False #是否编译项目
vspath = ''
copy = False #是否复制文件
version = '', #url版本号
rebuild = False, #强制重新编译
props = './Microsoft.Cpp.Common.user.props'	#配置文件
sevenzip ="./resources/7z.exe"
download_url = False

try:
	opts, args = getopt.getopt(sys.argv[1:],"hi:o:",["url=","output=","decompress","clean","cmake","vspath","build","copy", "props", "version=", "rebuild", "7z", "download"])
except getopt.GetoptError:
	print 'cef.py url, output, decompress, clean, cmake, vspath, build, copy, props, version, rebuild, 7z, download'
	sys.exit(2)
for opt, arg in opts:
	if opt == '-h':
		print 'test.py -i <inputfile> -o <outputfile>, exit'
		sys.exit()
	elif opt == ("--decompress"):
		decompress = True
	elif opt == ("--clean"):
		clean = True
	elif opt == ("--cmake"):
		cmake = True
	elif opt == ("--vspath"):
		vspath = arg
	elif opt == ("--build"):
		build = True
	elif opt == ("--copy"):
		copy = True
	elif opt == ("--rebuild"):
		rebuild = True
	elif opt == ("--download"):
		download_url = True
	elif opt == ("--props"):
		props = arg
	elif opt == ("--version"):
		version = arg
	elif opt == ("--7z"):
		sevenzip = arg
	elif opt in ("-i", "--url"):
		url = arg
	elif opt in ("-o", "--output"):
		outputPath = arg

if version <> '':
	url = node_cef_util.getCefUrl(version)
	
print 'url ' + url
print 'outputPath ' + outputPath
print 'decompress ' , decompress
print 'clean ' , clean
print 'cmake ', cmake
print 'vspath ', vspath
print 'build ', build
print 'copy ', copy
print 'props '+ props
print 'version '+ version
print 'rebuild ', rebuild
print 'download ', download_url

if props <> '':
	props = os.path.abspath(props)
if outputPath <> '':
	outputPath = os.path.abspath(outputPath)
	outputPath += "/"
if sevenzip <> '':
	sevenzip = os.path.abspath(sevenzip)
if downloads <> '':
	downloads = os.path.abspath(downloads)
	downloads += "/"
if tmp <> '':
	tmp = os.path.abspath(tmp)
	tmp += "/"

print "props: " + props	
print "outputPath: " + outputPath
print "sevenzip: " + sevenzip
print "downloads: " + downloads
print "tmp: " + tmp
	
#解析url
if url == '':
	print 'url is empty, exit'
	sys.exit(1) 
	 
url = url.replace('\\', '/')
print url
arr = url.split('/')
print arr
if len(arr) < 2:
	print "invalid url, exit"
	exit()
	
#最后一段为文件
filename = arr[len(arr)-1]

tmpFolder = tmp
index = filename.find(".tar")
print index
if index <> -1:
	tmpFolder += filename[0:index]
else:
	print "invalid zip folder, exit"
	exit()
print tmpFolder	








#clean
if clean == True:
	print "delete tmp folder "+ tmpFolder
	shutil.rmtree(tmpFolder)
	print "finished, exit"
	exit()


#下载文件的保存目录
fullname = downloads + filename

#创建目录
node_cef_util.createDir(downloads)
node_cef_util.createDir(tmp)

#download file		
#这一段是和cef.py不同的地方
baseurl= url[0: url.find(filename)]

headerurl = baseurl + filename[0:index] + "-headers.tar.gz"	
nodeurl = baseurl + "win-x86/node.exe"

print "baseurl : "+ baseurl
print "url : "+ url
print "headerurl : "+ headerurl
print "nodeurl : "+ nodeurl


headerfullname = downloads + filename[0:index] + "-headers.tar.gz";
nodefullDistPath = tmpFolder +"/Release/win-x86/"

nodefullname = downloads + filename[0:index] + ".exe"

print "download url save path " + fullname

if download_url == True:
	if os.path.exists(fullname) == False:
		print "download: " + url
		urllib.urlretrieve(url, fullname)
		print "download success"
	else:
		print "file exists :" + fullname
		 
	if os.path.exists(headerfullname) == False:
		print "download: " + headerurl
		urllib.urlretrieve(headerurl, headerfullname)
		print "download success"
	else:
		print "file exists :" + fullname
		
	if os.path.exists(nodefullname) == False:
		print "download: " + nodeurl
		urllib.urlretrieve(nodeurl, nodefullname)
		print "download success"
	else:
		print "file exists :" + nodefullname
else:
	print "skip download url"
	
#decompress file	
if decompress == True: 
	print "node_cef_util.unzip file " + fullname + " dist folder " + tmp
	node_cef_util.unzip(sevenzip, fullname, tmp)
	print "node_cef_util.unzip file " + headerfullname + " dist folder " + tmp
	node_cef_util.unzip(sevenzip, headerfullname, tmp)
	if os.path.exists(nodefullDistPath) == False:
		os.makedirs(nodefullDistPath)  
	node_cef_util.CopyTo(os.path.abspath(nodefullname), os.path.abspath(nodefullDistPath + "node.exe")) 
			
else:
	print "skip node_cef_util.unzip file"
	
nodefullDistPath += "node.exe"	
print "nodefullDistPath : " + nodefullDistPath

print tmpFolder
node_cef_util.createDir(tmpFolder)
#切换工作目录
os.chdir(tmpFolder)


#cmake 



































































#rebuild
if rebuild == True:
	if os.path.exists("./Release/node.dll") == True:
		os.remove("./Release/node.dll")
	build = True


	
	
	
print os.getcwd() #获取当前工作目录路径
print os.path.abspath('.') #获取当前工作目录路径
	
#build
if build == True:
	if os.path.exists("./Release/node.dll") == True:
		print "no files need to be compiled."
	else:
		print " build node "
		os.system('vcbuild.bat vc2015 dll release x86') 
else:
	print "skip build node.sln"
  


#copy
if copy == True:
	#先检测有没有build, 如果有那么copy，如果没有，那么检测lib目录是否已经存在，如果不存在则copy 
	srcPath = "./"
	outputPath += "/" + filename[0:index] + "/"
	 
	if os.path.exists(outputPath) == True:
		print outputPath + " already exist"
		
	#没有编译程序且目录存在，那么不拷贝文件
	if build == False and os.path.exists(outputPath) == True:
		print "skip copy file"
	else: 
		print "copy files"
		node_cef_util.transfer(srcPath  + "include" , outputPath + "include", '.*')
		node_cef_util.transfer(srcPath  + "Release" , outputPath + "ia32", 'node.lib')
		node_cef_util.transfer(srcPath  + "Release" , outputPath + "ia32", 'node.dll')
		node_cef_util.transfer(srcPath  + "Release/win-x86" , outputPath + "ia32", 'node.exe')

	

	
	
	
	
	
#修改config	
print "modify node config"
config = "\t\t<NodeGypFolder>$(LibraryFolder)" 
config += filename[0:index] 
config += "/</NodeGypFolder>\n"
print config
node_cef_util.modifyProps(props, "</NodeGypFolder>", config)

print "complete"
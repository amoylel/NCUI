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
keyword = "windows32_minimal"

index = filename.find(keyword)
 
if index == -1:
	print "not found minimal cef"
	keyword = "windows32"
	index = filename.find(keyword)
print index
if index <> -1:
	tmpFolder += filename[0:index + len(keyword)]
else:
	print "invalid zip folder, exit"
	exit()	
print "tmpFolder: " + tmpFolder	

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
#这一段是和node.py不同的地方

if download_url == True:
	print "download url save path " + fullname
	if os.path.exists(fullname) == False:
		print "download: " + url
		urllib.urlretrieve(url, fullname)
		print "download success"
	else:
		print "file exists :" + fullname
else:
	print "skip download url"


























	
	
	
	

#decompress file
if decompress == True: 
	print "node_cef_util.unzip file " + fullname + " dist folder " + tmp
	node_cef_util.unzip(sevenzip, fullname, tmp)
else:
	print "skip node_cef_util.unzip file"

print tmpFolder
node_cef_util.createDir(tmpFolder)
#切换工作目录
os.chdir(tmpFolder)










#cmake
if cmake == True:
	node_cef_util.modifyCmakeFiles()
	os.system('cmake -DUSE_SANDBOX:BOOL="0" -G "Visual Studio 14"')
else:
	print "skip call cmake "
	
print tmpFolder

libcef_dll_wrapper_path = "./libcef_dll/libcef_dll_wrapper.vcxproj"
if os.path.exists(libcef_dll_wrapper_path)<> True:
	libcef_dll_wrapper_path = "./libcef_dll_wrapper/libcef_dll_wrapper.vcxproj"

node_cef_util.modifyProject(libcef_dll_wrapper_path, "<TreatWarningAsError>", "<TreatWarningAsError>false</TreatWarningAsError>\n")

#查找vs path
if vspath == '':
	try:
		key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, r'Software\Microsoft\VisualStudio\14.0_Config')
		value,type = _winreg.QueryValueEx(key,"InstallDir")
		vspath = value
	except WindowsError:
		print "can not find vs path, exit"
		exit()
		
if vspath == '' :
	print "empty vs path, exit"
	exit()

if vspath.endswith('\\') <> True and  vspath.endswith('/') <> True:
	vspath += '\\'
	
devenv = vspath + 'devenv.exe'

command = """echo off
set IDE_DIR="""
command += vspath
command +="""
echo VC IDE DIR = %IDE_DIR%
echo -----------------------------------------------------------------------
echo start build 
for /f "tokens=1 delims=/ " %%j in ("%date%") do set d1=%%j
for /f "tokens=2 delims=/ " %%j in ("%date%") do set d2=%%j
for /f "tokens=3 delims=/ " %%j in ("%date%") do set d3=%%j
for /f "tokens=1 delims=: " %%j in ("%time%") do set t1=%%j
for /f "tokens=2 delims=: " %%j in ("%time%") do set t2=%%j
for /f "tokens=3 delims=:. " %%j in ("%time%") do set t3=%%j
set COMPILE_LOGFILE=%d1%-%d2%-%d3%_%t1%-%t2%-%t3%_compile.txt
echo log file = %COMPILE_LOGFILE%
echo compile [cef.sln - libcef_dll_wrapper - Release Win32]
"%IDE_DIR%devenv.exe" "%SOLUTION_DIR%cef.sln"  "Release|Win32" /project libcef_dll_wrapper /out %COMPILE_LOGFILE%
if %errorlevel% NEQ 0 goto myexit 
echo compile [cef.sln - libcef_dll_wrapper - Debug Win32]
"%IDE_DIR%devenv.exe" "%SOLUTION_DIR%cef.sln"  "Debug|Win32" /project libcef_dll_wrapper /out %COMPILE_LOGFILE%
if %errorlevel% NEQ 0 goto myexit 
echo ------------------ 
exit
:myexit
echo -----------------------------------------------------------------------
echo [build error] 
exit """

file = open('build.bat', 'w')
file.write(command)
file.close()

print "found devenv Path :" + devenv


#rebuild
if rebuild == True:
	if os.path.exists("./libcef_dll_wrapper/Release/libcef_dll_wrapper.lib") == True:
		os.remove("./libcef_dll_wrapper/Release/libcef_dll_wrapper.lib")
	if  os.path.exists("./libcef_dll_wrapper/Debug/libcef_dll_wrapper.lib") == True:
		os.remove("./libcef_dll_wrapper/Debug/libcef_dll_wrapper.lib")
		
	if os.path.exists("./libcef_dll/Release/libcef_dll_wrapper.lib") == True:
		os.remove("./libcef_dll/Release/libcef_dll_wrapper.lib")
	if  os.path.exists("./libcef_dll/Debug/libcef_dll_wrapper.lib") == True:
		os.remove("./libcef_dll/Debug/libcef_dll_wrapper.lib")
	build = True

#build
if build == True:
	if os.path.exists("./libcef_dll_wrapper/Release/libcef_dll_wrapper.lib") == True and  os.path.exists("./libcef_dll_wrapper/Debug/libcef_dll_wrapper.lib") == True:
		print "no files need to be compiled."
	elif os.path.exists("./libcef_dll/Release/libcef_dll_wrapper.lib") == True and  os.path.exists("./libcef_dll/Debug/libcef_dll_wrapper.lib") == True:
		print "no files need to be compiled."
	else:
		os.system('build.bat') 
else:
	print "skip build cef.sln"
  
  
#copy
if copy == True:
	#先检测有没有build, 如果有那么copy，如果没有，那么检测lib目录是否已经存在，如果不存在则copy
	srcPath = "./"
	outputPath += "/" + filename[0:index + len(keyword)] + "/"
 
	if os.path.exists(outputPath) == True:
		print outputPath + " already exist"
		
	#没有编译程序且目录存在，那么不拷贝文件
	if build == False and os.path.exists(outputPath) == True:
		print "skip copy file"
	else: 
		print "copy files" 
		node_cef_util.transfer(srcPath  + "include" , outputPath + "include", '.*')
		node_cef_util.transfer(srcPath  + "libcef_dll" , outputPath + "libcef_dll", '.cc')
		node_cef_util.transfer(srcPath  + "libcef_dll" , outputPath + "libcef_dll", '.h')
		node_cef_util.transfer(srcPath  + "libcef_dll/Debug" , outputPath + "libcef_dll_wrapper/Debug", '.lib')
		node_cef_util.transfer(srcPath  + "libcef_dll/Release" , outputPath + "libcef_dll_wrapper/Release", '.lib')
		node_cef_util.transfer(srcPath  + "libcef_dll_wrapper" , outputPath + "libcef_dll_wrapper", '.lib')
		node_cef_util.transfer(srcPath  + "Release" , outputPath + "bin", '.dll')
		node_cef_util.transfer(srcPath  + "Release" , outputPath + "bin", '.bin')
		node_cef_util.transfer(srcPath  + "Release" , outputPath + "bin", '.dat') 
		node_cef_util.transfer(srcPath  + "Release" , outputPath + "bin", 'libcef.lib') 
		node_cef_util.transfer(srcPath  + "Resources" , outputPath + "Resources", '.*')
	
#修改config	
print "modify cef config"
config = "\t\t<LibCefFolder>$(LibraryFolder)" 
config += filename[0:index + len(keyword)] 
config += "/</LibCefFolder>\n"
print config
node_cef_util.modifyProps(props, "</LibCefFolder>", config)

print "complete"
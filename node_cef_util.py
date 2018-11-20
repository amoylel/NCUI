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
 

reload(sys)
sys.setdefaultencoding('utf8') # 允许中文字符串
 


 
def getNCUIVersion(filename):
	file = open(filename, 'rb')
	strStart = "#define NCUI_VERSION"
 
	while True: 
		s = file.readline()
		if not s:      #等价于if s == "":
			break
		 
		start_pos = s.find(strStart)
		if start_pos <> -1:
			print s
			start_pos +=len(strStart) 
			str = s[start_pos:] 
			print str
			str = str.replace("\"", "")
			str = str.replace(" ", "")
			str = str.replace("\t", "")
			str = str.replace("\r", "")
			str = str.replace("\n", "")
			return str
	file.close()
	return "1.0.0.0"
	
def getNodeUrl(version):
	dict = {
		'4.7.1':'https://nodejs.org/download/release/v4.7.1/node-v4.7.1.tar.gz',
		'4x': 'https://nodejs.org/dist/latest-v4.x/node-v4.9.1.tar.gz',
		'6x': 'https://nodejs.org/dist/latest-v6.x/node-v6.14.4.tar.gz',
		'8x': 'https://nodejs.org/dist/latest-v8.x/node-v8.12.0.tar.gz',
		'10x': 'https://nodejs.org/dist/latest-v10.x/node-v10.13.0.tar.gz',
		}
	return dict.get(version, '')
	
def getCefUrl(version):
	dict = {
		'2272': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2272.32.gbda8dc7_windows32.7z',
		'2357': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2357.1291.g47e6d4b_windows32.7z',
		'2454': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2454.1344.g2782fb8_windows32.7z',
		'2526': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2526.1373.gb660893_windows32.7z',
		'2623': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2623.1401.gb90a3be_windows32.7z',
		'2704': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2704.1434.gec3e9ed_windows32_minimal.tar.bz2',
		'2743': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2743.1449.g90ba67d_windows32_minimal.tar.bz2',
		'2785': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2785.1485.g2b5c3a7_windows32_minimal.tar.bz2',
		'2840': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2840.1518.gffd843c_windows32_minimal.tar.bz2',
		'2883': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2883.1553.g80bd606_windows32_minimal.tar.bz2',
		'2924': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2924.1575.g97389a9_windows32_minimal.tar.bz2',
		'2987': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.2987.1601.gf035232_windows32_minimal.tar.bz2',
		'3029': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3029.1619.geeeb5d7_windows32_minimal.tar.bz2',
		'3071': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3071.1649.g98725e6_windows32_minimal.tar.bz2',
		'3112': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3112.1659.gfef43e0_windows32_minimal.tar.bz2',
		'3163': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3163.1671.g700dc25_windows32_minimal.tar.bz2',
		'3202': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3202.1694.gf061c23_windows32_minimal.tar.bz2',
		'3239': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3239.1723.g071d1c1_windows32_minimal.tar.bz2',
		'3282': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3282.1742.g96f907e_windows32_minimal.tar.bz2',
		'3325': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3325.1758.g9aea513_windows32_minimal.tar.bz2',
		'3359': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3359.1774.gd49d25f_windows32_minimal.tar.bz2',
		'3396': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3396.1786.gd3e36d0_windows32_minimal.tar.bz2',
		'3440': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3440.1806.g65046b7_windows32_minimal.tar.bz2',
		'3497': 'http://opensource.spotify.com/cefbuilds/cef_binary_3.3497.1841.g7f37a0a_windows32_minimal.tar.bz2',
	}
	return dict.get(version, '')
	
def getLibCefDirByUrl(url):
	if url == '':
		print 'url is empty, exit'
		return "" 
	 
	url = url.replace('\\', '/')
	print url
	arr = url.split('/')
	print arr
	if len(arr) < 2:
		print "invalid url, exit"
		return ""
		
	#最后一段为文件
	filename = arr[len(arr)-1] 
	
	keyword = "windows32_minimal"

	index = filename.find(keyword)
	 
	if index == -1:
		print "not found minimal cef"
		keyword = "windows32"
		index = filename.find(keyword)
	print index
	if index <> -1:
		return filename[0:index + len(keyword)]
	else:
		print "invalid zip folder, exit"
		return ""
		
def getLibNodeDirByUrl(url): 
	if url == '':
		print 'url is empty, exit'
		return ""
	 
	url = url.replace('\\', '/')
	print url
	arr = url.split('/')
	print arr
	if len(arr) < 2:
		print "invalid url, exit"
		return ""
		
	#最后一段为文件
	filename = arr[len(arr)-1]
 
	index = filename.find(".tar")
	print index
	if index <> -1:
		return filename[0:index]
	else:
		print "invalid zip folder, exit"
		return ""

def getLibCefDir(filename):
	file = open(filename, 'r')
	strStart = "<LibCefFolder>$(LibraryFolder)"
	strEnd = "</LibCefFolder>"
	while True: 
		s = file.readline()
		if not s:      #等价于if s == "":
			break
		start_pos = s.find(strStart)
		end_pos = s.find(strEnd)
		if start_pos <> -1 and end_pos <> -1:
			start_pos +=len(strStart) 
			return  s[start_pos:end_pos] 
	file.close();
	return ""
	 
def getLibNodeDir(filename):
	file = open(filename, 'r')
	strStart = "<NodeGypFolder>$(LibraryFolder)"
	strEnd = "</NodeGypFolder>"
	while True: 
		s = file.readline()
		if not s:      #等价于if s == "":
			break
		start_pos = s.find(strStart)
		end_pos = s.find(strEnd)
		if start_pos <> -1 and end_pos <> -1:
			start_pos +=len(strStart) 
			return  s[start_pos:end_pos] 
	file.close()
	return ""

def modifyFile(filename, item, newstr):
	print "modifyFile: " + filename
	str = ''
	file = open(filename, 'r')
	while True: 
		s = file.readline()
		if not s:      #等价于if s == "":
			break
		if s.find(item) <> -1:
			str += newstr
		else:
			str += s
	file.close();
	file = open(filename, 'w')
	file.write(str)
	file.close()
	
def modifyProject(filename, item, newstr):
	return modifyFile(filename, item, newstr)
	
def modifyProps(filename, item, newstr):
	return modifyFile(filename, item, newstr)

def modifyCmakeFiles():
	str = ''
	file = open('CMakeLists.txt', 'r')
	while True: 
		s = file.readline()
		if not s:      #等价于if s == "":
			break
		if s.find('add_subdirectory(tests') <> -1 or s.find('add_subdirectory(cef') <> -1:
			if s[0] <> '#':
				str += "#"
		str += s
	file.close();
	file = open('CMakeLists.txt', 'w')
	file.write(str)
	file.close()

def untar(fname, dirs):
    t = tarfile.open(fname)
    t.extractall(path = dirs) 
	
def unzip(exe, filename, outdir):
	if filename.endswith(".gz") or filename.endswith(".tar") or filename.endswith('.bz2'):
		untar(filename, outdir)
	else:
		command = exe
		command += " x "
		command += filename
		command += " -aoa -o"
		command += outdir
		print command
		os.system(command)
	
def createDir(p):
	if os.path.exists(p) == False:
		os.makedirs(p) 
		
def IsValidFile(file, ext):
	if ext == ".*":
		return True
	elif file.endswith(ext):
		return True
	else:
		return False

def CopyTo(srcFile, dstFile):
	print("CopyTo: ")
	print("src: " +  srcFile)
	print("dst: " + dstFile)
	dstPath= os.path.dirname( dstFile) 
	if os.path.exists(dstPath) == False:
		os.makedirs(dstPath)  
	os.system ("copy %s %s" % (srcFile, dstFile))
	
def transfer(srcDir, dstDir,  ext, currentDir = ""):  
	if currentDir == "":
		currentDir = srcDir
	print("current: " + os.path.abspath(currentDir)) 
	absCurrentDir=os.path.abspath(currentDir)
	if os.path.exists(absCurrentDir) == False:
		print "dir not exists: " + absCurrentDir
		return
	for file in os.listdir( currentDir): 
		path = os.path.join( currentDir,file)
		if os.path.isdir(path) == False: 
			arr = []
			if type(ext) <> type([]):
				arr.append(ext)
			else:
				arr = ext
			
			for x in arr: 
				if IsValidFile(file, x):  
					dstFile = path.replace(srcDir, dstDir)
					srcFile = os.path.abspath(path)
					dstFile = os.path.abspath(dstFile)
					CopyTo(srcFile, dstFile)
					break
		else: 
			dstSubDir = path.replace(srcDir, dstDir)
			dstSubDir = os.path.abspath(dstSubDir ) 
			transfer(srcDir, dstDir, ext, path)		
			
			
def copyCefResouces(cefPath, dstDir):
	transfer(cefPath  + "bin" , dstDir, '.dll')
	transfer(cefPath  + "bin" , dstDir, '.bin')
	transfer(cefPath  + "Resources", dstDir, '.*')
	return
	
def copyNodeResources(nodePath, dstDir, example):
	transfer(nodePath +  "ia32" , dstDir, '.dll')
	if example == True:
		transfer(nodePath +  "ia32" , dstDir, '.exe')
	return
def copyNCUIWebZipResources(recourcePath, dstDir):

	compress_command = '\"./resources/7z.exe \"'
	compress_command += ' a -tzip -r '
	compress_command += dstDir + "web/web.zip "
	compress_command += recourcePath + '/web/*' 
	
	print compress_command
	os.system(compress_command)
 
	return
	
def copyAllNCUIResources(recourcePath, dstDir):
	transfer(recourcePath, dstDir, '.*')
	return
	
def copyNCUIResources(recourcePath, dstDir, images, ppapi, npapi, web, vc2015):
	transfer(recourcePath, dstDir, ['zip.dll','zlib.dll','sqlite3.dll','main.js', 'manifest.json']) 
	transfer(recourcePath +"/skin", dstDir + "/skin", ['.*']) 
	if images == True:
		transfer(recourcePath + "/images", dstDir + "/images", ".*")
	if ppapi == True:
		transfer(recourcePath + "/plugins", dstDir + "/plugins", ["pepflashplayer.dll","manifest.json"])
	if npapi == True:
		transfer(recourcePath + "/plugins", dstDir + "/plugins", "NPSWF32_19_0_0_226.dll")
	if web == True:
		transfer(recourcePath + "/web", dstDir + "/web", ".*")
	if vc2015 == True:
		transfer(recourcePath + "/vc2015", dstDir , ".*")
	return
	
def copyNCUIBuildResources(buildPath, dstDir, node, example):
	transfer(buildPath, dstDir, ['libNCUI.dll', 'DuiLib.dll', 'libExt.dll','startup.dll', 'NCUI.exe'])
	copy_node = node
	if example == True:
		copy_node = True 
	if copy_node == True:
		transfer(buildPath, dstDir, ['ncui.node', 'node_runner.dll'])
	if example == True:
		copy_node = True
		transfer(buildPath, dstDir, ['ncui-dev.node', 'libResource.dll', 'libTaskTest.dll', 'NCUI-DEV.exe'])
	return
	
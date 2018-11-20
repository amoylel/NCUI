#-*- coding:utf-8 -*-
__author__ = 'luguolei'

import os
import sys
import re
import traceback
import codecs

_convertExt = [r'.*\.h$',r'.*\.c$',r'.*\.cpp$',r'.*\.hpp$',r'.*\.cpp$', r'.*\.tmpl$']

splt = os.path.sep

if splt == '\\':
    splt='\\\\'

_ignore = [r'.*'+splt+r'\.svn$',
           r'.*'+splt+r'\.git$']

_converted = []
_failed = []
_failedToWrite = []
_passed = [] #already utf8 with bom

_defaultEncoding = 'cp936'
_convertTo = ('utf8',codecs.BOM_UTF8)
_logFile = None
def WriteLog(s):
    cs = s
    if isinstance(cs, unicode):
        cs = s.encode('utf-8')
    global _logFile
    if _logFile is None:
        absFile = os.path.abspath('toutf8.log')
        _logFile = open(absFile, 'wb')
        _logFile.write(codecs.BOM_UTF8)
    _logFile.write(cs)
    if not s.endswith('\n'):
        _logFile.write("\n")
    _logFile.write("\n")
    print s

def CloseLog():
    if not _logFile:
        _logFile.close()

def Usage():
    print "Usage:"
    print "python toutf8.py <-d <directory or file>>"
    print """Example:
    python toutf8.py -d c:\myproject
    The command will convert *.c *.cpp and *.h files into utf-8 unless the file contain a valid BOM.
    all directory and file start with '.' will be ignored.
    a log file named 'converted.log' will be generated.
    """


def ParseParam(argv):
    def _Parse(mark, v):
        if mark not in v or v.index(mark) >= len(argv)-1:
            return []
        out = []
        for s in argv[argv.index(mark)+1:]:
            if s.startswith('-'):
                break;
            out.append(s)
        return out

    dir = _Parse('-d', argv)
    if not dir:
        print 'Invalid -d parameter'
        Usage()
        sys.exit(-1)
    if len(dir) != 1:
        print 'invalid -d parameter.'
        Usage()
        sys.exit(-1)
    return dir[0]

def DetectEncoding(path):
    assert os.path.isfile(path)
    supported = {codecs.BOM_UTF8:'utf8',
                 codecs.BOM_UTF16_BE:'UTF-16BE',
                 codecs.BOM_UTF16_LE:'UTF-16LE',
                 codecs.BOM_UTF32_BE:'UTF-32BE',
                 codecs.BOM_UTF32_LE:'UTF-32LE'}
    f = open(path,'rb')
    head = f.read(4)
    f.close()
    encoding = None
    bomlen = 0
    for k in supported:
        if k == head[0:len(k)]:
            encoding = supported[k]
            bomlen = len(k)
            break;
    if encoding is None:
        encoding = _defaultEncoding
    return encoding,bomlen

def ConvertFile(path):

    assert os.path.isfile(path)
    encoding,bomlen = DetectEncoding(path)
    if encoding == _convertTo[0]:
        _passed.append(path)
        return
    f = open(path,'rb')
    content = f.read()
    f.close()
    assert len(content) >= bomlen
    if bomlen > 0:
        content = content[bomlen:]

    encoded = None
    try:
        decoded = content.decode(encoding)
        encoded = decoded.encode(_convertTo[0])
    except:
        _failed.append(path)
        ex = traceback.format_exc()
        WriteLog("Ignore file:" + path + " because of error:")
        WriteLog(ex)
        return
    #delete original file
    os.remove(path)
    try:
        fw = open(path,'wb')
        fw.write(_convertTo[1])
        fw.write(encoded)
        fw.close()
        _converted.append(path)
    except:
        _failedToWrite.append(path)
        ex = traceback.format_exc()
        WriteLog("*Failed to create file: " + path + " because of error:")
        WriteLog(ex)


def SearchDirForFiles(start, matches=None, ignores=None, recursive=True):
    u"""搜索一个目录，返回符合要求的文件的一个生成器(generator).

    函数从指定的目录开始搜索符合要求的文件，最后，返回符合要求的文件绝对目录的全名。

    如果指定了matches和ignores参数(两组正则表达式)，函数首先使用ignores中的条件检
    查文件的绝对名称，如果被ignores中的任何一条表达式匹配，便忽略此文件。然后，再
    使用matches中的表达式匹配绝对文件名，被匹配的文件被存入列表准备返回。

    例如::
        SearchDirForFiles(ur'c:\')     #返回C盘下的所有文件(这可能需要一些时间)
        SearchDirForFiles(ur'c:\',recursive=False) #只返回根目录的文件，不搜索子目录
        SearchDirForFiles(ur'c:\',     #返回所有扩展名为.py或.pyw的文件，但是
            matches=(r'.*\.pyw?$',),   #不包括在一个子目录名为dtea下的。
            matches=(r'.*\\dtea$',))
        SearchDirForFiles(ur'c:\',     #返回所有扩展名为.py或.pyw的文件，但是
            matches=(r'.*\.pyw?$',),   #不包括在一个子目录名为dtea下的所有，以及
            matches=(r'.*\\dtea$', r'.*\\test_.*.pyw?$'))  #文件名前面是test_开头的.
        ...

    考虑到在搜索一些顶级目录时，可能有大量的返回结果，考虑到对内存友好一些，函数以生成器的
    形式，返回一个迭代对象，典型的使用方法为::
        for f in SearchDirForFiles(....):
            Process(f)
    不过，如果你确实需要获得这个文件的列表::
        lst = [f for f in SearchDirForFiles(...)]

    @attention: 你不可能对一个生成器使用像len()这样的函数. 因此，在迭代完成之前，你不可
    能知道有多少元素会输出.

    @param start: 起始目录，搜索的范围包括起始目录. 最好使用unicode字符串
    @type start: unicode,str
    @param matches: 匹配选项，一个包含一条或多条正则表达式的list或tuple. 如果指定
    为None，则表示所有文件都匹配(除了被ignores匹配的之外)。
    @type matches: list,tuple
    @param ignores: 忽略选项，一个包含一条或多条正则表达式的list或tuple. 如果为
    None, 则不忽略match的文件.
    @param recursive: 是否搜索子目录. 如果为False，则只搜索start目录下的文件.
    @type recursive: bool
    @return: 搜索到的文件的绝对路径名.
    @rtype: unicode list

    """

    # 编译正则表达式
    if matches is not None:
        reMatch = [re.compile(m.lower()) for m in matches]
    else:
        reMatch = []

    if ignores is not None:
        reIgnore = [re.compile(ig.lower()) for ig in ignores]
    else:
        reIgnore = []

    def _Match(fileName):
        u"""如果文件名匹配，返回True；否则False."""
        if len(reMatch) == 0:
            return True

        for r in reMatch:
            if r.match(fileName.lower()) is not None:
                return True
        return False

    def _Ignore(fileName):
        u"""如果文件名应该被忽略，返回True；否则False."""
        if len(reIgnore) == 0:
            return False

        for r in reIgnore:
            if r.match(fileName.lower()) is not None:
                return True
        return False

    # 即使起始目录不是中文，子目录也有可能是中文的，为了防止在返回中文目录时处理
    # 出错，将起始目录用unicode字符串表示，这样搜索结果也会是unicode的
    if isinstance(start,str):
        ustart = start.decode('utf-8')
    elif isinstance(start,unicode):
        ustart = start
    else:
        raise TypeError(u"start参数要求是str或unicode类型.")

    # 非递归的情况，只用列出起始目录下的文件
    if not recursive:
        for f in os.listdir(ustart):
            full = os.path.join(ustart,f)
            if (not _Ignore(full)) and _Match(full) and os.path.isfile(full):
                yield full
    else:
    # 递归的情况遍历
        for curdir, subdirs, files in os.walk(ustart):
            # 先检查是否需要忽略当前目录
            if _Ignore(curdir):
                # 不再搜索当前目录下的文件和子目录
                while len(subdirs)>0:
                    del subdirs[0]
                continue

            for f in files:
                full = os.path.join(curdir,f)
                if (not _Ignore(full)) and _Match(full):
                    yield full
                    #完成一个子目录的迭代


def SearchFiles(start):
    return [f for f in SearchDirForFiles(start,_convertExt,_ignore)]

def AreYouSure(path):
    if isinstance(path, (unicode,str)):
        displayPath = path
        if isinstance(path,unicode):
            displayPath = path.encode(sys.getfilesystemencoding())
        answer = raw_input("About to convert file "+displayPath+" to utf-8(with BOM), are you sure?[yes/no]:")
    else:
        answer = raw_input("About to convert file "+str(len(path))+" files to utf-8(with BOM), are you sure?[yes/no]:")
    if answer.strip().lower() != 'yes':
        print 'Canceled.'
        sys.exit(0)

def Convert(path):
    absPath = os.path.abspath(path)
    if isinstance(absPath, str):
        absPath = absPath.decode(sys.getfilesystemencoding())
    if os.path.isfile(absPath):
        AreYouSure(absPath)
        ConvertFile(absPath) # single file
        return
    allFiles = SearchFiles(absPath)
    AreYouSure(allFiles)
    for f in allFiles:
        ConvertFile(f)


if __name__=='__main__':
    path = ParseParam(sys.argv)
    Convert(path)
    WriteLog("------------------------------------------------")
    WriteLog("----------------Files converted-----------------")
    if not _converted:
        WriteLog("None.")
    for f in _converted:
        WriteLog(f)
    WriteLog("--------Files passed(Already UTF-8 with BOM)-----")
    if not _passed:
        WriteLog("None.")
    else:
        for f in _passed:
            WriteLog(f)
    WriteLog("--------------Files failed to convert------------")
    if not _failed:
        WriteLog("None.")
    else:
        for f in _failed:
            WriteLog(f)
    WriteLog("******Files failed to create(original file has been deleted)********\n******      !!!       The file has been lost  !!!           ********")
    if not _failedToWrite:
        WriteLog("None.")
    else:
        for f in _failedToWrite:
            WriteLog(f)

    CloseLog()

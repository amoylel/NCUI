import sys
import re
import os
import fnmatch
import os.path
from optparse import OptionParser
import posixpath
import string


reload(sys)
sys.setdefaultencoding('utf8') 

 
os.system("python js2c.py ../BrowserWindowSkin  BrowserWindow.xml NativeWindow.xml RenderView.xml MessageBox.xml MenuWindow.xml MenuElement.xml MenuElementMisc.xml NotifyWindow.xml SplashWindow.xml NCUI.js 404.html")
{
  'targets': [
    {
      'target_name': 'ncui',
      'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
	   'include_dirs': [    
						'./',
						'./nan',
                       '../libNCUI',
                       '../third_party/amo',
					   'D:/SDK/boost_1_63_0'],
					   'libraries':[ 
					   'D:/SDK/boost_1_63_0/stage/lib/libboost_program_options-vc140-mt-s-1_63.lib',
					   'D:/SDK/boost_1_63_0/stage/lib/libboost_random-vc140-mt-s-1_63.lib',
					   'D:/SDK/boost_1_63_0/stage/lib/libboost_system-vc140-mt-s-1_63.lib',
					   'D:/SDK/boost_1_63_0/stage/lib/libboost_thread-vc140-mt-s-1_63.lib',
					   'D:/SDK/boost_1_63_0/stage/lib/libboost_date_time-vc140-mt-s-1_63.lib',
					   'D:/SDK/boost_1_63_0/stage/lib/libboost_chrono-vc140-mt-s-1_63.lib',
					   'D:/SDK/boost_1_63_0/stage/lib/libboost_filesystem-vc140-mt-s-1_63.lib'
					   ], 
      'sources': [ 
	    'binding.cc',
    'module/ConstructorMgr.cpp',
    'dllmain.cpp',
    'module/IPCNodeV8Handler.cpp',
    'example/myobject.cc',
    'ipc/NodeMessageEmitter.cpp',
    'handler/NodeProcessHandler.cpp',
    'module/NodeTypeConvertor.cpp',
    'stdafx.cpp'
	]
    }
  ]
}

{
    'target_defaults': {
        'conditions': [
            ['OS=="linux"', {
                'defines': [
                    '__unix__',
                    '_LINUX',
                    '__LINUX__',
                    '__linux__',
                ],
                'cflags': [
                    '-Wall',
                    '-std=c++11',
                    '-fPIC',
                    '-Wunused-function',
                    '-Wunused',
                    '-fpermissive',
                ],
                'dflags': [
                    '-fpermissive',
                ],
                'link_settings': {
                    'libraries': [
                        '-lboost_filesystem',
                        '-lboost_date_time',
                        '-lboost_system',
                        '-lboost_thread',
                        '-lboost_chrono',
                        '-lboost_locale',
                        '-lboost_regex',
                        '-lboost_program_options',
                        '-ldl',
                        '-lrt',                     
                        '-lpthread'
                    ]
                }
            }],


            ['OS=="win"', {
                'defines': [
                    'WIN32'
                ],
                'msvs_configuration_attributes': {
                    'CharacterSet': '1'
                },
                'msvs_settings': {
                    'VCCLCompilerTool': {
                        'WarningLevel': '4',
                        'Detect64BitPortabilityProblems': 'true'
                    }
                }
            }],
            ['OS=="mac"', {
                'defines': [
                    '__unix__',
                    '_MACOS'
                ],
                'cflags': [
                    '-Wall'
                ]
            }]
        ],

        'configurations': {
            'Debug': {
                'defines': [
                    '_DEBUG'
                ],
                'conditions': [
                    ['OS=="linux" or OS=="freebsd" or OS=="openbsd"', {
                        'cflags': [
                            '-g'
                        ]
                    }],
                    ['OS=="win"', {
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'Optimization': '0',
                                'MinimalRebuild': 'true',
                                'BasicRuntimeChecks': '3',
                                'DebugInformationFormat': '4',
                                'RuntimeLibrary': '3'

                            },
                            'VCLinkerTool': {
                                'GenerateDebugInformation': 'true',
                                'LinkIncremental': '2'
                            }
                        }
                    }],
                    ['OS=="mac"', {
                        'xcode_settings': {
                            'GCC_GENERATE_DEBUGGING_SYMBOLS': 'YES'
                        }
                    }]
                ]
            },

            'Release': {
                'conditions': [
                    ['OS=="linux" or OS=="freebsd" or OS=="openbsd"', {
                        'cflags': [
                            '-O3'
                        ]
                    }],
                    ['OS=="win"', {
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'Optimization': '2',

                                'RuntimeLibrary': '0'
                            }
                        }
                    }],
                    ['OS=="mac"', {
                        'xcode_settings': {
                            'GCC_GENERATE_DEBUGGING_SYMBOLS': 'NO',
                            'GCC_OPTIMIZATION_LEVEL': '3',


                            'GCC_STRICT_ALIASING': 'YES'
                        }
                    }]
                ]
            }
        },
        'include_dirs': [
            './../../',
            './../../third_party',
            './../../include',
            './',
            './third_party',
            './include',
            './../../third_party/amo',
    './../../Plugin',
    './../../',
    './../../Plugin/include',
    
        ]
    }

}
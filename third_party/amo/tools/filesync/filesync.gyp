{ 
'includes': [ 
    'common.gyp',
    'filesync.gypi', 
  ],
  'targets': [
        {
            'target_name': 'filesync',
            'product_name': 'filesync',
            'type': 'executable',
            'sources': [
                '<@(source_code_filesync)',
            ],
            'dependencies': [
                
            ]
        },
    
    ]
}
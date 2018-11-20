{ 
'includes': [ 
    'common.gyp',
    'sln2gyp.gypi', 
  ],
  'targets': [
        {
            'target_name': 'sln2gyp',
            'product_name': 'sln2gyp',
            'type': 'executable',
            'sources': [
                '<@(source_code_sln2gyp)',
            ],
            'dependencies': [
                
            ]
        },
    
    ]
}
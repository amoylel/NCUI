{ 
'includes': [ 
    'common.gyp',
    'file2c.gypi', 
  ],
  'targets': [
        {
            'target_name': 'file2c',
            'product_name': 'file2c',
            'type': 'executable',
            'sources': [
                '<@(source_code_file2c)',
            ],
            'dependencies': [
                
            ]
        },
    
    ]
}
{ 
'includes': [ 
    'common.gyp',
    'jsonfmt.gypi', 
  ],
  'targets': [
        {
            'target_name': 'jsonfmt',
            'product_name': 'jsonfmt',
            'type': 'executable',
            'sources': [
                '<@(source_code_jsonfmt)',
            ],
            'dependencies': [
                
            ]
        },
    
    ]
}
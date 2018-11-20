{ 
'includes': [ 
    'common.gyp',
    'examples.gypi', 
  ],
  'targets': [
        {
            'target_name': 'examples',
            'product_name': 'examples',
            'type': 'executable',
            'sources': [
                '<@(source_code_examples)',
            ],
            'dependencies': [
                
            ]
        },
    
    ]
}
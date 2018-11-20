{ 
'includes': [ 
    'common.gyp',
    'amo.gypi', 
  ],
  'targets': [
        {
            'target_name': 'amo',
            'product_name': 'amo',
            'type': 'none',
            'sources': [
                '<@(source_code_amo)',
            ],
            'dependencies': [
                
            ]
        },
    
    ]
}
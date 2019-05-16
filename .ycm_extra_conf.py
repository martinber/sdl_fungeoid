def Settings( **kwargs ):
    return {
        'flags': [
            '-x', 'c',
            '-std=c11',
            '-Wall',
            '-Wextra',
            '-pedantic',
            '-Werror'
        ],
    }

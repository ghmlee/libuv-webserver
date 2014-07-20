{
  'includes': [ 'common.gypi' ],
  'targets': [
    {
      'target_name': 'webserver',
      'type': 'executable',
      'sources': [
        'webserver.c',
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
        './deps/http-parser/http_parser.gyp:http_parser'
      ],
    }
  ],
}
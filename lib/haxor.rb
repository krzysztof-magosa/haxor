# 3rd party
require 'json'
require 'io/console'
require 'scanf'
require 'digest'
require 'ostruct'

require_relative 'haxor/consts'
require_relative 'haxor/header'
require_relative 'haxor/utils'

# compiler
require_relative 'haxor/compiler/unit'
require_relative 'haxor/compiler/core'

# linker
require_relative 'haxor/linker'

# tokens
require_relative 'haxor/token/base'
require_relative 'haxor/token/data'
require_relative 'haxor/token/int64'
require_relative 'haxor/token/label'
require_relative 'haxor/token/pointer'
require_relative 'haxor/token/cmd'

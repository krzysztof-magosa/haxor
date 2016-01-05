# 3rd party
require 'json'
require 'io/console'
require 'scanf'
require 'digest'
require 'ostruct'

require_relative 'haxor/consts'
require_relative 'haxor/header'
require_relative 'haxor/utils'

# vm
require_relative 'haxor/vm/core'
require_relative 'haxor/vm/subsystem'
require_relative 'haxor/vm/mem'
require_relative 'haxor/vm/stack'
require_relative 'haxor/vm/os'
require_relative 'haxor/vm/registers'
require_relative 'haxor/vm/cpu'

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

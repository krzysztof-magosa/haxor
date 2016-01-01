# 3rd party
require 'json'
require 'io/console'
require 'scanf'
require 'digest'

require_relative 'haxor/consts'
require_relative 'haxor/header'

# vm
require_relative 'haxor/vm/core'
require_relative 'haxor/vm/subsystem'
require_relative 'haxor/vm/mem'
require_relative 'haxor/vm/stack'
require_relative 'haxor/vm/os'
require_relative 'haxor/vm/registers'
require_relative 'haxor/vm/cpu/core'
require_relative 'haxor/vm/cpu/unit/base'
require_relative 'haxor/vm/cpu/unit/logical'
require_relative 'haxor/vm/cpu/unit/arithmetic'
require_relative 'haxor/vm/cpu/unit/jumps'
require_relative 'haxor/vm/cpu/unit/various'
require_relative 'haxor/vm/cpu/unit/transfer'

# compiler
require_relative 'haxor/compiler/unit'
require_relative 'haxor/compiler/core'
require_relative 'haxor/compiler/component/base'
require_relative 'haxor/compiler/component/logical'
require_relative 'haxor/compiler/component/arithmetic'
require_relative 'haxor/compiler/component/jumps'
require_relative 'haxor/compiler/component/various'
require_relative 'haxor/compiler/component/transfer'
require_relative 'haxor/compiler/component/data'
require_relative 'haxor/compiler/component/other'

# linker
require_relative 'haxor/linker'

# tokens
require_relative 'haxor/token/base'
require_relative 'haxor/token/data'
require_relative 'haxor/token/int64'
require_relative 'haxor/token/label'
require_relative 'haxor/token/pointer'
require_relative 'haxor/token/cmd'

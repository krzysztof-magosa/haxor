# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'haxor/consts'

Gem::Specification.new do |spec|
  spec.name          = "haxor"
  spec.version       = Haxor::Consts::GEM_VERSION
  spec.authors       = ["Krzysztof Magosa"]
  spec.email         = ["krzysztof@magosa.pl"]

  spec.summary       = "Educational Assembly Language running on custom VM"
  spec.description   = "Please follow to GitHub repository for more information."
  spec.homepage      = "https://github.com/krzysztof-magosa/haxor"
  spec.license       = "BSD-3"

  spec.files         = `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  spec.bindir        = "bin"
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.10"
  spec.add_development_dependency "rake", "~> 10.0"
end

# frozen_string_literal: true

Dir.glob('test_*.rb', base: __dir__).each do |f|
  p f
  require_relative f
end

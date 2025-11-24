# frozen_string_literal: true

require_relative '../main'
require_relative 'calc_lex'

Calc = Grammer.new(
  vn: Set[:S, :E, :T, :F],
  vt: Set['(', ')', '*', '+', 'i', '-', '/', :EOF],
  s: :S,
  p: Set[
    Rule.new(:S, [:E],           ->(v) { v[0] }),
    Rule.new(:E, [:E, '+', :T],  ->(v) { v[0] + v[2] }),
    Rule.new(:E, [:E, '-', :T],  ->(v) { v[0] - v[2] }),
    Rule.new(:E, [:T],           ->(v) { v[0] }),
    Rule.new(:T, [:T, '*', :F],  ->(v) { v[0] * v[2] }),
    Rule.new(:T, [:T, '/', :F],  ->(v) { v[0] / v[2] }),
    Rule.new(:T, [:F],           ->(v) { v[0] }),
    Rule.new(:F, ['(', :E, ')'], ->(v) { v[1] }),
    Rule.new(:F, ['i'],          ->(v) { v[0] })
  ],
  precedence: []
)

def repl
  calc = generate_lalr1_parser Calc
  calc.print_table
  loop do
    print 'calc> '
    input = $stdin.gets
    break if input.nil?

    prompt = input.chomp
    next if prompt.empty?

    lex = Lexer.new prompt
    result = calc.parse lex, false
    case result
    in [:accept, n]
      p n
    in [:error, reason]
      p reason
      p Lexer.new(prompt).to_a
    end
  end
end

repl if __FILE__ == $PROGRAM_NAME

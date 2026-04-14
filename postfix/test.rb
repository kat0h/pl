require_relative './main'

# given a, b, c, x, calculates ax^2 + bx + c pp.12
# P = [postfix, 4, 4, nget, 5, nget, mul, mul, swap, 4, nget, mul, add, add]
# args = [3, 4, 5, 2]
# an absolute value program pp.12
# P = [postfix, 1, 1, nget, 0, lt, [0, swap, sub], [], sel, exec]
# args = [-7]

test_cases = [
  # pp.9
  {
    p: [postfix, 0, 1, 2, 3],
    args: [],
    v_expected: 3
  }, {
    p: [postfix, 0, 1, 2, 3, pop],
    args: [],
    v_expected: 2
  }, {
    p: [postfix, 0, 1, 2, swap, 3, pop],
    args: [],
    v_expected: 1
  }, {
    p: [postfix, 0, 1, swap],
    args: [],
    v_expected: error
  }, {
    p: [postfix, 0, 1, pop, pop],
    args: [],
    v_expected: error
  },
  # pp.10 (上部の例)
  {
    p: [postfix, 2],
    args: [3, 4],
    v_expected: 3
  }, {
    p: [postfix, 2, swap],
    args: [3, 4],
    v_expected: 4
  }, {
    p: [postfix, 3, pop, swap],
    args: [3, 4, 5],
    v_expected: 5
  },
  # pp.11 (Semantics 1.4.2)
  {
    p: [postfix, 2, swap],
    args: [3],
    v_expected: error # Wrong number of arguments.
  }, {
    p: [postfix, 1, pop],
    args: [4, 5],
    v_expected: error # Wrong number of arguments.
  }, {
    p: [postfix, 1, 4, sub],
    args: [3],
    v_expected: -1
  }, {
    p: [postfix, 1, 4, add, 5, mul, 6, sub, 7, div],
    args: [3],
    v_expected: 4
  }, {
    p: [postfix, 5, add, mul, sub, swap, div],
    args: [7, 6, 5, 4, 3],
    v_expected: -20
  }, {
    p: [postfix, 3, 4000, swap, pop, add],
    args: [300, 20, 1],
    v_expected: 4020
  }, {
    p: [postfix, 2, add, 2, div],
    args: [3, 7],
    v_expected: 5
  }, {
    p: [postfix, 1, 3, div],
    args: [17],
    v_expected: 5
  }, {
    p: [postfix, 1, 3, rem],
    args: [17],
    v_expected: 2
  }, {
    p: [postfix, 1, 4, lt],
    args: [3],
    v_expected: 1
  }, {
    p: [postfix, 1, 4, lt],
    args: [5],
    v_expected: 0
  }, {
    p: [postfix, 1, 4, lt, 10, add],
    args: [3],
    v_expected: 11
  }, {
    p: [postfix, 1, 4, mul, add],
    args: [3],
    v_expected: error # Not enough numbers to add.
  }, {
    p: [postfix, 2, 4, sub, div],
    args: [4, 5],
    v_expected: error # Divide by zero.
  },
  # pp.12 (nget の基本動作)
  {
    p: [postfix, 2, 1, nget],
    args: [4, 5],
    v_expected: 4
  }, {
    p: [postfix, 2, 2, nget],
    args: [4, 5],
    v_expected: 5
  },
  # pp.12 (nget のエラーケース)
  {
    p: [postfix, 2, 3, nget],
    args: [4, 5],
    v_expected: error # Index 3 is too large.
  }, {
    p: [postfix, 2, 0, nget],
    args: [4, 5],
    v_expected: error # Index 0 is too small.
  }, {
    p: [postfix, 1, [2, mul], 1, nget],
    args: [3],
    v_expected: error # Value at index 1 is not a number but an executable sequence.
  },
  # pp.12 (nget の応用例)
  {
    p: [postfix, 1, 1, nget, mul],
    args: [5],
    v_expected: 25 # A squaring program.
  }, {
    p: [postfix, 4, 4, nget, 5, nget, mul, mul, swap, 4, nget, mul, add, add],
    args: [3, 4, 5, 2],
    v_expected: 25 # Given a, b, c, x, calculates ax^2 + bx + c.
  }
]

test_cases.each_with_index do |c, i|
  p = c[:p]
  args = c[:args]
  v_expected = c[:v_expected]
  v_actual = eval_postfix(p, args)
  print "Test case #{i + 1}"
  if v_actual == v_expected
    puts ' Passed'
    next
  else
    puts ' Failed'
  end

  puts "P:          #{p}"
  puts "args:       #{args}"
  puts "v_expected: #{v_expected}"
  puts "v_actual:   #{v_actual}"
  throw 'assert equal'
end

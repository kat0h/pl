#    args
# P ------> v
#

# Commands
# N
# add sub div mul rem 
# lt gt eq
# pop
# swap
# sel
# nget
# (C1 ... Cn)
# exec

def eval(p, args)
  argc = p[1]
  cont = p[2..]
  stack = args.reverse
  while op = cont.shift
    case op
    when Integer
      stack.push op
    when Array
      stack.push op
    when :add
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 + v1
    when :sub
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 - v1
    when :mul
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 * v1
    when :div
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 / v1
    when :rem
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 % v1
    when :lt
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 < v1 ? 1 : 0
    when :gt
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 > v1 ? 1 : 0
    when :eq
      v1 = stack.pop
      v2 = stack.pop
      stack.push v2 == v1 ? 1 : 0
    when :pop
      stack.pop
    when :swap
      v1 = stack.pop
      v2 = stack.pop
      stack.push v1
      stack.push v2
    when :sel
      v1 = stack.pop
      v2 = stack.pop
      v3 = stack.pop
      stack.push v3 != 0 ? v2 : v1
    when :nget
      idx = stack.pop
      stack.push stack[stack.size - idx]
    when :exec
      cont.unshift *stack.pop
    end
  end
  stack.last
end

def postfix = :postfix
def add = :add
def sub = :sub
def mul = :mul
def div = :div
def rem = :rem
def lt = :lt
def gt = :gt
def eq = :eq
def pop = :pop
def swap = :swap
def sel = :sel
def nget = :nget
def exec = :exec

# given a, b, c, x, calculates ax^2 + bx + c pp.12
# P = [:postfix, 4, 4, :nget, 5, :nget, :mul, :mul, :swap, 4, :nget, :mul, :add, :add]
# args = [3,4,5,2]

# an absolute value program pp.12
# P = [postfix, 1, 1, nget, 0, lt, [0, swap, sub], [], sel, exec]
# args = [-7]
p eval(P, args)

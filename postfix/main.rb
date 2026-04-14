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

def eval_postfix(p, args)
  cont = p[2..]
  return :error if args.size != p[1]

  stack = args.reverse
  while (op = cont.shift)
    case op
    when Integer
      stack.push op
    when Array
      stack.push op
    when :add
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if (!v1 in Integer) || (!v2 in Integer)

      stack.push v2 + v1
    when :sub
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if (!v1 in Integer) || (!v2 in Integer)

      stack.push v2 - v1
    when :mul
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if (!v1 in Integer) || (!v2 in Integer)

      stack.push v2 * v1
    when :div
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if !v1.is_a?(Integer) || !v2.is_a?(Integer) || v1.zero?

      # n = qd + r において、q = truncate(n/d) となる挙動
      stack.push (v2 / v1.to_f).truncate

    when :rem
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if !v1.is_a?(Integer) || !v2.is_a?(Integer) || v1.zero?

      # 被除数 v2 の符号に合わせる余り (remainder)
      stack.push v2.remainder(v1)
    when :lt
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if !(v1 in Integer) || !(v2 in Integer)

      stack.push v2 < v1 ? 1 : 0
    when :gt
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if !(v1 in Integer) || !(v2 in Integer)

      stack.push v2 > v1 ? 1 : 0
    when :eq
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      return :error if !(v1 in Integer) || !(v2 in Integer)

      stack.push v2 == v1 ? 1 : 0
    when :pop
      return :error if stack.empty?

      stack.pop
    when :swap
      return :error if stack.size < 2

      v1 = stack.pop
      v2 = stack.pop
      stack.push v1
      stack.push v2
    when :sel
      (return :error) if stack.size < 3
      v1 = stack.pop
      v2 = stack.pop
      v3 = stack.pop
      return :error unless v3 in Integer

      stack.push v3 != 0 ? v2 : v1
    when :nget
      idx = stack.pop
      return :error if (!idx in Integer) || (0 >= idx) || (idx > stack.size)

      stack.push stack[stack.size - idx]
    when :exec
      return :error if stack.empty?

      seq = stack.pop
      return :error unless seq in Array

      cont.unshift(*seq)
    end
  end
  return :error if stack.empty? || !(stack.last in Integer)

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
def error = :error

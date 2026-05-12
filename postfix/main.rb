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

def sos(seq, stack)
  op = seq.shift
  case op
  when Integer
    stack.unshift op
  when Array
    stack.unshift op
  when :add
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if (!v1 in Integer) || (!v2 in Integer)

    stack.unshift v1 + v2
  when :sub
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if (!v1 in Integer) || (!v2 in Integer)

    stack.unshift v1 - v2
  when :mul
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if (!v1 in Integer) || (!v2 in Integer)

    stack.unshift v1 * v2
  when :div
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if !v1.is_a?(Integer) || !v2.is_a?(Integer) || v1.zero?

    # n = qd + r において、q = truncate(n/d) となる挙動
    stack.unshift (v1 / v2.to_f).truncate

  when :rem
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if !v1.is_a?(Integer) || !v2.is_a?(Integer) || v1.zero?

    # 被除数 v2 の符号に合わせる余り (remainder)
    stack.push v2.remainder(v1)
  when :lt
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if !(v1 in Integer) || !(v2 in Integer)

    stack.unshift v1 < v2 ? 1 : 0
  when :gt
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if !(v1 in Integer) || !(v2 in Integer)

    stack.push v1 > v2 ? 1 : 0
  when :eq
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    return :error if !(v1 in Integer) || !(v2 in Integer)

    stack.push v1 == v2 ? 1 : 0
  when :pop
    return :error if stack.empty?

    stack.shift
  when :swap
    return :error if stack.size < 2

    v2 = stack.shift
    v1 = stack.shift
    stack.unshift v2
    stack.unshift v1
  when :sel
    (return :error) if stack.size < 3
    v3 = stack.shift
    v2 = stack.shift
    v1 = stack.shift
    return :error unless v1 in Integer

    stack.unshift v1 != 0 ? v2 : v3
  when :nget
    idx = stack.shift
    return :error if (!idx in Integer) || (0 >= idx) || (idx > stack.size)

    stack.unshift stack[idx-1]
  when :exec
    throw "Not implemented"
    return :error if stack.empty?
    return :error unless seq in Array
  end
  return seq, stack
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

def prg2s(prg)
  if prg.instance_of?(Array)
    "(#{prg.map{prg2s(it)}.join(" ")})"
  else
    prg.to_s 
  end
end
def seq2s(seq)
  "[#{seq.map{prg2s(it)}.join(",")}]"
end

def proof_seq(seq, stack)
  if seq.empty?
    return "[]", stack
  end
  if seq[0] == exec
    qrest = seq2s(seq[1..])
    qexecs = seq2s(stack)
    tree, sd = proof_seq(stack.first, stack[1..])
    return "rule(
      name: [[exec]],
      [exec . #{qrest} $->>^(\"#{qexecs}\")_Q$ S''],
      #{tree},
      [;]
    )", [111111]
  else
    cq = seq2s(seq)
    s = seq2s(stack)
    seqd, stackd = sos(seq, stack)
    q = seq2s(seqd)
    sd = seq2s(stackd)
    tree, stackdd = proof_seq(seqd, stackd)
    return "rule(
      name: [[non-exec]],
      [#{cq} $->>^(\"#{s}\")_Q$ #{seq2s(stackdd)}],
      [$chevron.l \"#{cq}\", \"#{s}\" chevron.r => chevron.l \"#{q}\", \"#{sd}\" chevron.r$ ; ],
      #{tree}
    )", stackdd
  end
end
def proof_postfix(prg, args)
  seq = prg[2..]
  sargs = args.to_s
  tree, stack = proof_seq(seq, args)
  return "
    #set page(width: 1200cm, height: 1200cm)
    #import \"@preview/curryst:0.3.0\": rule, proof-tree
    #proof-tree(
      rule(
        name: [[prog]],
        [#{prg2s(prg)} $->>^(#{sargs})_P$ #{stack.first}],
        #{tree}
      ),
    )"
end

puts proof_postfix [postfix, 2, [2, [3, mul, add], exec], 1, swap, exec, sub], [4, 5]

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
def command_is_value?(command) = command.is_a?(Integer) || command.is_a?(Array)

DEBUG = true
def eval_postfix_ver2(p, args)
  puts "Input:   \t #{p}" if DEBUG
  puts "Args:   \t #{args}\n\n" if DEBUG
  # check arguments
  argc_expected = p[1]
  argc_actual = args.size
  return error if argc_expected != argc_actual

  seq = args.reverse + p[2..] # 評価途中の値
  loop do
    break unless (i = seq.find_index { |command| !command_is_value?(command) })

    puts "Step: #{seq}" if DEBUG

    command = seq[i]
    case command
    when nget
      v_index = seq[i - 1]
      return error if (i - v_index - 1) < 0 || v_index < 1

      v_i = seq[i - v_index - 1]
      return error unless v_i.is_a?(Integer)

      seq[(i - 1)..i] = [v_i]
    when swap
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      seq[(i - 2)..i] = [v1, v2]
    when exec
      exe_seq = seq[i - 1]
      return error unless exe_seq.is_a?(Array)

      seq[(i - 1)..i] = exe_seq
    when mul
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer)

      seq[(i - 2)..i] = [v2 * v1]
    when sub
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer)

      seq[(i - 2)..i] = [v2 - v1]
    when add
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer)

      seq[(i - 2)..i] = [v2 + v1]
    when div
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer) || v1.zero?

      seq[(i - 2)..i] = [(v2 / v1.to_f).truncate]
    when rem
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer) || v1.zero?

      seq[(i - 2)..i] = [v2.remainder(v1)]
    when lt
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer)

      seq[(i - 2)..i] = [v2 < v1 ? 1 : 0]
    when gt
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer)

      seq[(i - 2)..i] = [v2 > v1 ? 1 : 0]
    when eq
      return error if i < 2

      v2, v1 = seq[(i - 2)..(i - 1)]
      return error if !v1.is_a?(Integer) || !v2.is_a?(Integer)

      seq[(i - 2)..i] = [v2 == v1 ? 1 : 0]
    when pop
      return error if i.zero?

      seq[(i - 1)..i] = []
    when sel
      return error if i < 3

      v3, v2, v1 = seq[(i - 3)..(i - 1)]
      return error if !v3.is_a?(Integer)

      seq[(i - 3)..i] = [v3 != 0 ? v2 : v1]
    else
      p "undefined command: #{command}"
      return error
    end
  end

  return error unless seq.last.is_a?(Integer)

  seq.last
end
# 42 # Calculates b - a * b^2

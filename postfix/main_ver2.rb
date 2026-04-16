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

DEBUG = true
def eval_postfix_ver2(p, args)
  puts "Input:   \t #{p}" if DEBUG
  puts "Args:   \t #{args}\n\n" if DEBUG
  # check arguments
  argc_expected = p[1]
  argc_actual = args.size
  return error if argc_expected != argc_actual

  seq = args.reverse + p[2..]
  loop do
    finish = true
    seq.each_with_index do |c, i|
      next if c.is_a?(Integer) || c.is_a?(Array)

      case c
      when nget
        v_index = seq[i - 1]
        return error if v_index.nil? || !v_index.is_a?(Integer)

        seq.delete_at v_index
        return error if seq[i - 1 - v_index].nil?

        seq[i - 1] = seq[i - 1 - v_index]
        finish = false
        break
      when swap
        seq.delete_at i
        return error if i < 2

        tmp = seq[i - 1]
        seq[i - 1] = seq[i - 2]
        seq[i - 2] = tmp
        finish = false
        break
      when exec
        seq[(i - 1)..i] = seq[i - 1]
        finish = false
        break
      else
        puts "Undefined command \"#{c}\""
        return error
      end
    end
    puts "Step: #{seq}" if DEBUG
    break if finish
  end
end

v = eval_postfix_ver2 [postfix, 2, [mul, sub], [1, nget, mul], 4, nget, swap, exec, swap, exec], [-10, 2]
puts '結果↓'
puts v
# 42 # Calculates b - a * b^2

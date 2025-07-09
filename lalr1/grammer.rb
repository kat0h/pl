require_relative "lr_n"

class Grammer
  attr_reader :vn, :vt, :s, :p, :precedence
  def initialize(vn:, vt:, s:, p:, precedence:)
    @vn = vn
    @vt = vt
    @s = s
    @p = p
    @precedence = precedence
  end
  def inspect
    ret = "{\nVn: {#{vn.to_a.map(&:to_s).join(", ")}}\n"
    ret << "  Vt: {#{vt.to_a.map(&:to_s).join(", ")}}\n"
    ret << "  S: #{s},\n"
    ret << "  P: {\n"
    p.each do |it| ret << "    " << it.inspect << ",\n" end
    ret << "  }\n"
    ret << "  precedence: #{precedence}\n"
    ret << "}"
  end
end

class Rule
  attr_reader :l, :r, :act
  def initialize(l, r, act)
    @l = l
    @r = r
    @act = act
  end
  def inspect = "#{l} → #{r.join(" ")}"
  def to_lr1(dot, ls) = LR1.new(l, r, dot, ls, act)
  def eql?(other) = self.class == other.class && l == other.l && r == other.r
  def hash = [l, r].hash
end

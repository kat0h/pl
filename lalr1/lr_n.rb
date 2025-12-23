class LR0
  attr_accessor :l, :r, :dot, :act
  def initialize l, r, dot, act
    @l = l
    @r = r
    @dot = dot
    @act = act
  end
  def inspect
    ret = "[#{@l} → "
    @r.each_index do |i|
      ret << (i == @dot ? "・" : "") + "#{@r[i]}" + (i != @r.size - 1 ? " " : "")
    end
    ret << "・" if @dot == @r.size
    ret << "]"
  end

  def eql?(other)
    self.class == other.class &&
      @l == other.l &&
      @r == other.r &&
      @dot == other.dot
  end

  def hash
    [@l, @r, @dot].hash
  end
end

class LR1 < LR0
  attr_accessor :ls
  def initialize l, r, dot, ls, act
    super(l, r, dot, act)
    @ls = ls
  end
  def inspect
    ret = "[#{l} → "
    r.each_index do |i|
      ret << (i == dot ? "・" : "") + "#{r[i]}" + (i != r.size - 1 ? " " : "")
    end
    ret << "・" if dot == r.size
    ret << ", #{ls}]"
  end
  def complete? = r.size == dot
  def coreeql?(lr1) = l == lr1.l && r == lr1.r && dot == lr1.dot
  def to_rule = Rule.new(l, r, act)
  def lr0 = LR0.new(l, r, dot, act)
  def kernel?(g) = l == g.s || !dot.zero?
  def eql?(other)
    self.class == other.class &&
      @l == other.l &&
      @r == other.r &&
      @dot == other.dot &&
      @ls == other.ls
  end
  def hash = [@l, @r, @dot, @ls].hash
end

def printLR1Set set, kernel_only=nil # LR(1)項集合を見易く表示するやつ
  q = if kernel_only.nil?
    set.to_a
  else
    set.to_a.select{it.kernel?(kernel_only)}
  end
  while !q.empty?
    i=q.shift;ls=[i.ls]
    q.reject!{i.coreeql?(_1)?ls<<_1.ls: false}
    ret="[#{i.l} → "
    i.r.each_index do
      ret<<(_1==i.dot ? "・":"")+"#{i.r[_1]}"+(_1!=i.r.size-1?" ":"")
    end
    ret<< " ・" if i.dot==i.r.size
    ret<<", #{ls.map{_1==:EOF ??$:_1}.join("/")}]"
    puts ret
  end
end

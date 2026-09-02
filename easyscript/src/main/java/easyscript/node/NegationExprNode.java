package easyscript.node;

import com.oracle.truffle.api.dsl.Fallback;
import com.oracle.truffle.api.dsl.NodeChild;
import com.oracle.truffle.api.dsl.Specialization;

@NodeChild("value")
public abstract class NegationExprNode extends EasyScriptExprNode {
  @Specialization(rewriteOn = ArithmeticException.class)
  protected int negateInt(int value) {
    return Math.negateExact(value);
  }
  @Specialization(replaces = "negateInt")
  protected double negateDouble(double value) {
    return -value;
  }
  @Fallback
  protected double negateNonNumber(Object value) {
    // doubleでもintでもないなら符号を反転させることはできない．
    return Double.NaN;
  }
}

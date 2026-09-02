package easyscript.node;

import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.UnexpectedResultException;

public final class DoubleLiteralExprNode extends EasyScriptExprNode {
  private final double value;
  // コンストラクタ
  public DoubleLiteralExprNode(double value) {
    this.value = value;
  }
  @Override
  public double executeDouble(VirtualFrame frame) {
    return this.value;
  }
  @Override
  public Object executeGeneric(VirtualFrame frame) {
    return this.executeDouble(frame);
  }
  @Override
  public int executeInt(VirtualFrame frame) throws UnexpectedResultException {
    throw new UnexpectedResultException(this.value);
  }
}

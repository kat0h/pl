package easyscript;
import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.UnexpectedResultException;

public final class DoubleLiteralNode extends EasyScriptNode {
  private final double value;
  // コンストラクタ
  public DoubleLiteralNode(double value) {
    this.value = value;
  }
  @Override
  public double executeDouble(VirtualFrame frame) {
    return this.value;
  }
  @Override
  public Object executeGeneric(VirtualFrame frame) {
    return this.value;
  }
  @Override
  public int executeInt(VirtualFrame frame) throws UnexpectedResultException {
    throw new UnexpectedResultException(this.value);
  }
}

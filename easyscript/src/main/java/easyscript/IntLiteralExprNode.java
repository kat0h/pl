package easyscript;
import com.oracle.truffle.api.frame.VirtualFrame;

public final class IntLiteralExprNode extends EasyScriptExprNode {
  private final int value;
  // コンストラクタ
  public IntLiteralExprNode(int value) {
    this.value = value;
  }
  @Override
  // EasyScriptNodeで抽象メソッドを定義しているので
  public int executeInt(VirtualFrame frame) {
    return this.value;
  }
  @Override
  public double executeDouble(VirtualFrame frame) {
    return this.value;
  }
  @Override
  public Object executeGeneric(VirtualFrame frame) {
    return this.value;
  }
}

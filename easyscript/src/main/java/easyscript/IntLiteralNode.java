package easyscript;
import com.oracle.truffle.api.frame.VirtualFrame;

public final class IntLiteralNode extends EasyScriptNode {
  private final int value;
  // コンストラクタ
  public IntLiteralNode(int value) {
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

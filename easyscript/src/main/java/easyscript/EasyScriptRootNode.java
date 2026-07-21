package easyscript;
import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.RootNode;

public final class EasyScriptRootNode extends RootNode {
  @Child
  private EasyScriptNode exprNode;

  public EasyScriptRootNode(EasyScriptNode exprNode) {
    super(null); // ここは，まだnullのまま
    this.exprNode = exprNode;
  }
  // rootは直接呼び出すのではなく，calltargetを使って実行する
  @Override
  public Object execute(VirtualFrame frame) {
    return this.exprNode.executeInt(frame);
  }
}

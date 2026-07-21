package easyscript;
import com.oracle.truffle.api.frame.VirtualFrame;

public final class AdditionNode extends EasyScriptNode {
  // 変数に対して注釈を付けることにより，
  // leftnode/rightnodeがastの子ノードであることをTruffleに教える
  // SuppressWarningsはIDEの警告を止めるだけ
  @Child @SuppressWarnings("FieldMayBeFinal")
  private EasyScriptNode leftNode, rightNode;

  public AdditionNode(EasyScriptNode left, EasyScriptNode right) {
    this.leftNode = left;
    this.rightNode = right;
  }
  @Override
  public int executeInt(VirtualFrame frame) {
    int lv = this.leftNode.executeInt(frame);
    int rv = this.rightNode.executeInt(frame);
    return lv + rv;
  }
}

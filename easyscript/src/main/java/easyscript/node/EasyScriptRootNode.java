package easyscript.node;

import easyscript.EasyScriptTruffleLanguage;
import easyscript.runtime.Undefined;

import java.util.List;
import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.RootNode;

public final class EasyScriptRootNode extends RootNode {
  @Children
  private EasyScriptStmtNode[] stmtNodes;

  public EasyScriptRootNode(EasyScriptTruffleLanguage truffleLanguage, List<EasyScriptStmtNode> stmtNodes) {
    super(truffleLanguage);
    this.stmtNodes = stmtNodes.toArray(EasyScriptStmtNode[]::new);
  }
  // rootは直接呼び出すのではなく，calltargetを使って実行する
  @Override
  public Object execute(VirtualFrame frame) {
    Object ret = Undefined.INSTANCE;
    for (EasyScriptStmtNode stmtNode : this.stmtNodes) {
      ret = stmtNode.executeStatement(frame);
    }
    return ret;
  }
}

package easyscript;

import com.oracle.truffle.api.frame.VirtualFrame;

public abstract class EasyScriptStmtNode extends EasyScriptNode {
  public abstract Object executeStatement(VirtualFrame frame);
}

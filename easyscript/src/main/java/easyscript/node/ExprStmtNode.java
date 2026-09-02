package easyscript.node;

import com.oracle.truffle.api.frame.VirtualFrame;

public final class ExprStmtNode extends EasyScriptStmtNode {
  @SuppressWarnings("FieldMayBeFinal")
  @Child
  private EasyScriptExprNode expr;
  public ExprStmtNode(EasyScriptExprNode expr) {
    this.expr = expr;
  }
  @Override
  public Object executeStatement(VirtualFrame frame) {
    return this.expr.executeGeneric(frame);
  }
}

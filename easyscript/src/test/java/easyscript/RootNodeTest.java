package easyscript;

import com.oracle.truffle.api.CallTarget;
// テストのためのライブラリ
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;


class RootNodeTest {
  @Test
  void add_12_34() {
    EasyScriptNode exprNode = new AdditionNode(
        new IntLiteralNode(12),
        new IntLiteralNode(34));
    var rootNode = new EasyScriptRootNode(exprNode);
    CallTarget callTarget = rootNode.getCallTarget(); //calltargetを取得
    Object result = callTarget.call();
    assertEquals(46, result);
  }
}

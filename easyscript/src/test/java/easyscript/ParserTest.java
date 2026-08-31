package easyscript;

import com.oracle.truffle.api.CallTarget;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

class ParserTest {
  @Test
  void parses_and_executes_EasyScript_code_correctly() {
    EasyScriptNode exprNode = EasyScriptTruffleParser.parse("1 + 2 + 3.0 + 4");
    var rootNode = new EasyScriptRootNode(exprNode);
    CallTarget callTarget = rootNode.getCallTarget();
    Object result = callTarget.call();
    assertEquals(10.0, result);
  }
}

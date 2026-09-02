package easyscript;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.Set;

import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.Value;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class GlobalVarTest {
  private Context context;
  @BeforeEach
  void setUp() {
    this.context = Context.create();
  }
  @AfterEach
  void tearDown() {
    this.context.close();
  }
  @Test
  void test1() {
    Value result = this.context.eval("ezs", "" +
        "var a = 1; " +
        "let b = 2 + 3; " +
        "const c = 4 + 5.0; " +
        "(a = a + b + a) + a"
    );

    assertEquals(14, result.asInt());
    Value globalBindings = this.context.getBindings("ezs");
    assertFalse(globalBindings.isNull());
    assertTrue(globalBindings.hasMembers());
    assertTrue(globalBindings.hasMember("a"));
    Value a = globalBindings.getMember("a");
    assertEquals(7, a.asInt());
    assertEquals(Set.of("a", "b", "c"), globalBindings.getMemberKeys());
  }
}

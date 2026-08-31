package easyscript;

import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.Value;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

class PolyglotTest {
    @Test
    void runs_JavaScript_code_correctly() {
        Context context = Context.create();
        Value result = context.eval("js",
                "function sub13(x) { return x - 13; } sub13(25)");
        assertEquals(12, result.asInt());
    }
    @Test
    void runs_EasyScript_code_correctly() {
      Context context = Context.create();
      Value result = context.eval("ezs", "1+2.0+3");
      assertEquals(6.0, result.asDouble());
    }
}

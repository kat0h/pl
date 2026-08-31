package easyscript;

import com.oracle.truffle.api.dsl.ImplicitCast;
import com.oracle.truffle.api.dsl.TypeCast;
import com.oracle.truffle.api.dsl.TypeCheck;
import com.oracle.truffle.api.dsl.TypeSystem;

@TypeSystem
public abstract class EasyScriptTypeSystem {
  // intは常にdoubleにcastできる
  // 暗黙的に
  @ImplicitCast
  public static double castIntToDouble(int value) {
    return value;
  }
  // TypeCheck
  // TypeCast
}

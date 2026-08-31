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
  // @TypeCheck(double.class)
  // public static boolean isDouble(Object value) {
  //   return value instanceof Double || value instanceof Integer;
  // }
  // TypeCast
  // @TypeCast(double.class)
  // public static double asDouble(Object value) {
  //   if (value instanceof Integer) {
  //     return ((Integer) value).doubleValue();
  //   } else {
  //     return (double) value;
  //   }
  // }
}

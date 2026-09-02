package easyscript;
import com.oracle.truffle.api.dsl.Fallback;
import com.oracle.truffle.api.dsl.NodeChild;
import com.oracle.truffle.api.dsl.Specialization;

@NodeChild("leftNode")
@NodeChild("rightNode")
public abstract class AdditionExprNode extends EasyScriptExprNode {
  @Specialization(rewriteOn = ArithmeticException.class)
  protected int addInts(int leftValue, int rightValue) {
    return Math.addExact(leftValue, rightValue);
  }
  @Specialization(replaces = "addInts")
  protected double addDoubles(double leftValue, double rightvalue) {
    return leftValue + rightvalue;
  }
  @Fallback
  protected double addWithUndefined(Object leftValue, Object rightValue) {
    return Double.NaN;
  }
}

// これだけで↓と同じことができる？？？？
// 初期状態はなに？

// public final class AdditionNode extends EasyScriptNode {
//   // 変数に対して注釈を付けることにより，
//   // leftnode/rightnodeがastの子ノードであることをTruffleに教える
//   // SuppressWarningsはIDEの警告を止めるだけ
//   @Child @SuppressWarnings("FieldMayBeFinal")
//   private EasyScriptNode leftNode, rightNode;
//   private enum SpecializationState { UNINITIALIZED, INT, DOUBLE }
//
//   // 部分評価中にこのメンバがfinalであると扱える
//   @CompilerDirectives.CompilationFinal
//   private SpecializationState specializationstate;
//
//   public AdditionNode(EasyScriptNode left, EasyScriptNode right) {
//     this.leftNode = left;
//     this.rightNode = right;
//     this.specializationstate = SpecializationState.UNINITIALIZED;
//   }
//   @Override
//   public int executeInt(VirtualFrame frame) throws UnexpectedResultException {
//     int leftValue;
//     try {
//       // fast path
//       leftValue = this.leftNode.executeInt(frame);
//     } catch (UnexpectedResultException e) {
//       // slow path
//       this.activateDoubleSpecialization();
//       double leftDouble = (double) e.getResult();
//       // executeDoubleは例外を発生しない = slowpath
//       throw new UnexpectedResultException(leftDouble + this.rightNode.executeDouble(frame));
//     }
//     int rightValue;
//     try {
//       rightValue = this.rightNode.executeInt(frame);
//     } catch (UnexpectedResultException e) {
//       this.activateDoubleSpecialization();
//       double rightDouble = (double) e.getResult();
//       throw new UnexpectedResultException(leftValue + rightDouble);
//     }
//     try {
//       // overflow検知機能付き
//       return Math.addExact(leftValue, rightValue);
//     } catch (ArithmeticException e) {
//       // overflowを検知したら...
//       this.activateDoubleSpecialization();
//       throw new UnexpectedResultException((double) leftValue + (double) rightValue);
//     }
//   }
//   @Override
//   public double executeDouble(VirtualFrame frame) {
//     double lv = this.leftNode.executeDouble(frame);
//     double rv = this.rightNode.executeDouble(frame);
//     return lv + rv;
//   }
//   public Object executeGeneric(VirtualFrame frame) {
//     if (this.specializationstate == SpecializationState.INT) {
//       try {
//         return this.executeInt(frame);
//       } catch (UnexpectedResultException e) {
//         // 2回もactivateDoubleSpecialization()を呼び出すことにならない？
//         // this.activateDoubleSpecialization();
//         // 要らないと思うので，コメントアウトする
//         return e.getResult();
//       }
//     }
//     if (this.specializationstate == SpecializationState.DOUBLE) {
//       return this.executeDouble(frame);
//     }
//     // uninitialized case
//     Object lv = this.leftNode.executeGeneric(frame);
//     Object rv = this.rightNode.executeGeneric(frame);
//     // ここに到達している場合，部分評価されていたとしてもそれを破棄する
//     CompilerDirectives.transferToInterpreterAndInvalidate();
//     return this.executeAndSpecialize(lv, rv);
//   }
//   private Object executeAndSpecialize(Object leftValue, Object rightValue) {
//     if (leftValue instanceof Integer && rightValue instanceof Integer) {
//       try {
//         // intのstateに移れるなら
//         int result = Math.addExact((int) leftValue, (int) rightValue);
//         this.activateIntSpecialization();
//         return result;
//       } catch (ArithmeticException e) {
//         // overflowしちゃうなら，doubleのstateに
//       }
//     }
//     this.activateDoubleSpecialization();
//     return convertToDouble(leftValue) + convertToDouble(rightValue);
//   }
//   private void activateIntSpecialization() {
//     this.specializationstate = SpecializationState.INT;
//   }
//   private void activateDoubleSpecialization() {
//     this.specializationstate = SpecializationState.DOUBLE;
//   }
//   private static double convertToDouble(Object value) {
//     // Integerとは？
//     if (value instanceof Integer) {
//       return ((Integer) value).doubleValue();
//     }
//     return (double) value;
//   }
// }

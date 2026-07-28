package easyscript;

import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.Node;
import com.oracle.truffle.api.nodes.UnexpectedResultException;

public abstract class EasyScriptNode extends Node {
  // UnexpectedResultExceptionを投げる可能性のあるメソッドであることを指定
  public abstract int executeInt(VirtualFrame frame) throws UnexpectedResultException;
  public abstract double executeDouble(VirtualFrame frame);
  public abstract Object executeGeneric(VirtualFrame frame);
}

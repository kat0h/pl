package easyscript;


import com.oracle.truffle.api.dsl.TypeSystemReference;
import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.UnexpectedResultException;

@TypeSystemReference(EasyScriptTypeSystem.class)
public abstract class EasyScriptExprNode extends EasyScriptNode {
  public abstract int executeInt(VirtualFrame frame) throws UnexpectedResultException;
  public abstract double executeDouble(VirtualFrame frame) throws UnexpectedResultException;
  public abstract Object executeGeneric(VirtualFrame frame);
}

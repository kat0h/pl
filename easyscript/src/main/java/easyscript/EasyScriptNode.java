package easyscript;

import com.oracle.truffle.api.dsl.TypeSystemReference;

import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.Node;
import com.oracle.truffle.api.nodes.UnexpectedResultException;

// なにこれ
// このTypeSystemを使ってねという注釈 継承したクラスにも継承されるので
@TypeSystemReference(EasyScriptTypeSystem.class)
public abstract class EasyScriptNode extends Node {
  // UnexpectedResultExceptionを投げる可能性のあるメソッドであることを指定
  public abstract int executeInt(VirtualFrame frame) throws UnexpectedResultException;
  public abstract double executeDouble(VirtualFrame frame);
  public abstract Object executeGeneric(VirtualFrame frame);
}

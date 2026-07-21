package easyscript;

import com.oracle.truffle.api.frame.VirtualFrame;
import com.oracle.truffle.api.nodes.Node;

public abstract class EasyScriptNode extends Node {
  public abstract int executeInt(VirtualFrame frame);
}

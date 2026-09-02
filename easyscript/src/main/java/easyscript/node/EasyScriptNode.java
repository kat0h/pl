package easyscript.node;

import easyscript.EasyScriptLanguageContext;
import com.oracle.truffle.api.nodes.Node;

public abstract class EasyScriptNode extends Node {
  protected final EasyScriptLanguageContext currentLanguageContext() {
    return EasyScriptLanguageContext.get(this);
  }
}

package easyscript.node;

import easyscript.EasyScriptException;

import com.oracle.truffle.api.dsl.NodeField;
import com.oracle.truffle.api.dsl.Specialization;

@NodeField(name = "name", type = String.class)
public abstract class GlobalVarReferenceExprNode extends EasyScriptExprNode {
  protected abstract String getName();

  @Specialization
  protected Object readVariable() {
    String varialeId = this.getName();
    var value = this.currentLanguageContext().globalScopeObject.getVariable(varialeId);
    if (value == null) {
      throw new EasyScriptException(this, "'" + varialeId + "' is not defined");
    }
    return value;
  }
}

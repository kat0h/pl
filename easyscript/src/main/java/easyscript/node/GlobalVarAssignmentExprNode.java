package easyscript.node;

import easyscript.EasyScriptException;

import com.oracle.truffle.api.dsl.NodeChild;
import com.oracle.truffle.api.dsl.NodeField;
import com.oracle.truffle.api.dsl.Specialization;

@NodeChild(value = "assignmentExpr")
@NodeField(name = "name", type = String.class)
public abstract class GlobalVarAssignmentExprNode extends EasyScriptExprNode {
  protected abstract String getName();
  @Specialization
  protected Object assignVariable(Object value) {
    String variableId = this.getName();
    if (!this.currentLanguageContext().globalScopeObject.updateVariable(variableId, value)) {
      throw new EasyScriptException(this, "" + variableId + "' is not defined");
    }
    return value;
  }
}
